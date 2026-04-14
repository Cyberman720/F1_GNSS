#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "capture_synced.pio.h"

#define DATA_PIN 2
#define CLOCK_PIN 3
#define BUF_SIZE 4096

uint32_t buffer[BUF_SIZE];




void setup_pio(PIO pio, int sm) {
    // Load the PIO program
    uint offset = pio_add_program(pio, &capture_synced_program);
    // Configure the PIO state machine
    pio_sm_config c = capture_synced_program_get_default_config(offset);
    // Set DATA pin for input
    sm_config_set_in_pins(&c, DATA_PIN);
    // Set CLOCK pin for WAIT instruction
    sm_config_set_jmp_pin(&c, CLOCK_PIN);
    // Set both pins as input
    pio_sm_set_consecutive_pindirs(pio, sm, DATA_PIN, 1, false);
    pio_sm_set_consecutive_pindirs(pio, sm, CLOCK_PIN, 1, false);
    // Configure shift with autopush every 32 bits
    sm_config_set_in_shift(&c, true, true, 32);
    // Optionally, join the RX FIFO for deeper buffer
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    // Initialize state machine
    pio_sm_init(pio, sm, offset, &c);
}



// Function to setup DMA
void setup_dma(int dma_chan, PIO pio, int sm) {
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
    // Configure DMA for fixed read increment (FIFO) and writable memory buffer
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);
    // Link the DMA to PIO RX FIFO
    channel_config_set_dreq(&cfg, DREQ_PIO0_RX0);
    // Configure DMA channel to transfer data from PIO RX FIFO to memory
    dma_channel_configure(
        dma_chan,
        &cfg,
        buffer,            // Destination memory buffer
        &pio->rxf[sm],     // Source (PIO RX FIFO)
        BUF_SIZE,          // Number of transfers (32-bit words)
        false              // Don't start yet
    );
}




// push to serial bit by bit. 
void publish_buffer() {
    // Use the standard output to send data via serial (USB to PC)
    printf("Buffer filled, publishing data...");
    // Publish the captured data
    for (int i = 0; i < BUF_SIZE; ++i) {
        printf("%i", buffer[i]);
    }
}



int main() {
    // Initialize standard IO
    stdio_init_all();
    // Set up PIO and DMA
    PIO pio = pio0;
    int sm = 0;  // State machine number
    // Set up PIO
    setup_pio(pio, sm);
    // Set up DMA
    int dma_chan = dma_claim_unused_channel(true);
    setup_dma(dma_chan, pio, sm);
    // Start DMA
    dma_start_channel_mask(1u << dma_chan);
    // Start the PIO state machine
    pio_sm_set_enabled(pio, sm, true);


    // Infinite loop for continuous operation
    while (true) {
        // Wait for DMA to finish (buffer full)
        dma_channel_wait_for_finish_blocking(dma_chan);
        // Once DMA finishes, buffer is full — publish the data
        publish_buffer();
        // Restart the DMA for the next capture
        dma_channel_cleanup(dma_chan);
        dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
        dma_channel_configure(
            dma_chan,
            &cfg,
            buffer,            // Reset the buffer
            &pio->rxf[sm],     // Continue reading from the same FIFO
            BUF_SIZE,          // Number of transfers
            false              // Don't start yet
        );
        dma_start_channel_mask(1u << dma_chan);  // Restart DMA
        // Reset the PIO state machine to capture the next batch of data
        pio_sm_restart(pio, sm);  // Reset PIO state machine
        pio_sm_set_enabled(pio, sm, true);
    }

    return 0;
}
