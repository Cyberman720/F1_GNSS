using namespace std;
/* 
The objective of this file is to locally generate each satalite's goldcode.
Scope wise, I will be focusing on the 32 GPS L1 sats.
*/


int shift(code_register, code_feedback, code_output) {
    /* this function takes in our goldcode taps, then performs the required shift and XOR functions to make it work*/
    int shift_size_output = sizeof(code_output);
    int out[shift_size_output] = {};
    for (i_output = 0; i_output < shift_size_output; i_output++) {
        int out[i_output] = {code_register[i_output]};
    };
    //modulo 2 summing
    if (shift_size_output > 1;) {
        int shift_output = (out[0] + out[1]) % 2;
    };
    else {
        int shift_output = out[0];
    };
    // feedback
    int shift_size_feedback = sizeof(code_feedback);
    int shift_feedback = 0;
    for (i_feedback = 0; i_feedback < shift_size_feedback; i_feedback++) {
        shift_feedback += code_register[i_feedback];
    };
    shift_feedback = shift_feedback % 2;
    // shift set one to the right
    for (i = n - 1; i >= 0; i--)
    {
        code_feedback[i + 1] = code_feedback[i];
    }
    code_feedback[0] = shift_feedback


	return code_feedback;
}

int prn() {
    // sat number is index + 1 (as indexes start at 0 but sats start at 1)
    int SV_table[32][2] = {
        {2, 6 },
        {3, 7},
        {4, 8},
        {5, 9},
        {1, 9},
        {2, 10},
        {1, 8},
        {2, 9},
        {3, 10},
        {2, 3},
        {3, 4},
        {5, 6},
        {6, 7},
        {7, 8},
        {8, 9},
        {9, 10},
        {1, 4},
        {2, 5},
        {3, 6},
        {4, 7},
        {5, 8},
        {6, 9},
        {1, 3},
        {4, 6},
        {5, 7},
        {6, 8},
        {7, 9},
        {8, 10},
        {1, 6},
        {2, 7},
        {3, 8},
        {4, 9},
    };
    //initiate the shift registers
    int G1[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int G2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int coarse_acquisition[1023] = {};

    for ()
    return coarse_acquisition;
}


int main() {

	return 0;
}