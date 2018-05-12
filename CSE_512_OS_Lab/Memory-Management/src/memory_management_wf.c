/*
Copyright 2018 Atikur Rahman Chitholian

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************\
* Assignment number                : 07                 *
* Date of receiving assignment     : 2018-04-01         * 
* Date of submitting assignment    : 2018-05-18         * 
*  Name                            : Atikur Rahman      * 
*  J = 11                                               * 
*  K = 11                                               * 
*  L = 28                                               * 
*  M = 14                                               * 
*  N = 64                                               * 
*  Problem number: 11, 12                               * 
*   (Memory Management schemes: WORST FIT).             *
\*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants.
#define J 11
#define K 11
#define L 28
#define N 64
#define FILE_IN "input_file_for_wf.in"
#define FILE_OUT "output_file_for_wfmm.out"

FILE *input_file, *output_file;
int i, j, k;
long long total_hole_size, total_process_size, total_unallocated;
// Structure to hold a hole-info.
struct HOLE {
    int total, available;
};

// Declare required function prototypes.
int generateInput();

int runWorstFit();

// Entry of execution.
int main(int argc, char **argv) {
    // Make sure input generation was successful.
    if (generateInput() == 0) {
        if (runWorstFit() != 0)
            fprintf(stderr, "Warning! Worst Fit returned non-zero status.\n");
        printf("\n\t<===0===0===>\n Task complete, summary written to the output file:"
                       "\n\t1. \"%s\".\n",
               FILE_OUT);
    } else {
        fprintf(stderr, "Error! Cannot generate input, exiting.\n");
        return 1;
    }
    return 0;
}

// Function to generate input data.
int generateInput() {
    // Set seed for random.
    srand((unsigned) time(NULL));
    // open input_file in writing mode.
    input_file = fopen(FILE_IN, "w");

    // generate N inputs.
    printf("Generating %d input data.\n", N);
    for (i = 0; i < N; ++i) {
        printf("Iteration %d of %d running...\n", i + 1, N);

        // Generate K number of memory holes with random size from 5 x ‘L’ to 35 x ‘L’.
        printf("Generating %d memory holes with random size from %d to %d... ", K, 5 * L, 35 * L);
        for (j = 0; j < K; ++j) {
            int size = ((rand() % 31) + 5) * L;
            fprintf(input_file, "%d ", size);
        }
        fprintf(input_file, "\n"); // put an EOL.
        printf("Done.\n");

        // Generate J number of processes with random size from 10 x ‘L’ to 20 x ‘L’.
        printf("Generating %d processes with random size from %d to %d... ", J, 10 * L, 20 * L);
        for (j = 0; j < J; ++j) {
            int size = ((rand() % 11) + 10) * L;
            fprintf(input_file, "%d ", size);
        }
        fprintf(input_file, "\n\n"); // put a blank line meaning "end of current input".
        printf("Done.\n");
        printf("Iteration %d complete.\n", i + 1);
    }

    // close input_file.
    fclose(input_file);
    printf("Input generation complete.\n");
    return 0;
}

// function to test Worst Fit scheme.
int runWorstFit() {
    printf("Running Worst Fit scheme.\n0%%.");
    // open input_file in reading mode.
    input_file = fopen(FILE_IN, "r");
    // open output_file in writing mode.
    output_file = fopen(FILE_OUT, "w");
    // initialize all stats to zero.
    total_hole_size = total_process_size = total_unallocated = 0;
    // run N times.
    for (i = 1; i <= N; ++i) {
        struct HOLE holes[K];
        int process;
        // input K memory holes.
        for (j = 0; j < K; j++) {
            // input total size.
            fscanf(input_file, "%d", &holes[j].total);
            // set total size to available.
            holes[j].available = holes[j].total;

            total_hole_size += holes[j].total;
        }
        // input J processes and map to holes.
        fprintf(output_file, "=========== Iteration %2d =============\n", i);
        fprintf(output_file, "%-5s\t%-8s\t%-8s\t%-10s\t%-8s\n", "PID", "P_SIZE", "HOLE_ID", "HOLE_SIZE", "AVAILABLE");
        fprintf(output_file, "------------------------------------------\n");

        for (j = 1; j <= J; j++) {
            fscanf(input_file, "%d", &process);
            total_process_size += process;

            fprintf(output_file, "%-5d\t%-8d\t", j, process);
            // Search all K holes to fit the process.
            short fitted = 0;
            int max_size = -0xff, max_hole;
            for (k = 0; k < K; k++) {
                // if process fits, store worst option.
                if (holes[k].available >= process && holes[k].available > max_size) {
                    max_size = holes[k].available;
                    max_hole = k;
                    fitted = 1;
                }
            }
            if (fitted) {
                holes[max_hole].available -= process;
                fprintf(output_file, "%-8d\t%-10d\t%-8d\n", max_hole + 1, holes[max_hole].total,
                        holes[max_hole].available);
            } else {
                fprintf(output_file, "=> NOT ALLOCATED\n");
                total_unallocated++;
            }
        }

        fprintf(output_file, "\n");
        printf("\r%d%%.", i * 100 / N);
    }
    printf("\nWriting summary... ");
    fprintf(output_file, "\n\n========== Summary ==========\n");
    fprintf(output_file, "Average Process Size : %.2f\n", total_process_size * 1.0 / (N * J));
    fprintf(output_file, "Average Hole Size    : %.2f\n", total_hole_size * 1.0 / (N * K));
    fprintf(output_file, "Average unallocated  : %.2f%%.\n", total_unallocated * 100.0 / (N * J));
    // close files.
    fclose(input_file);
    fclose(output_file);
    printf("Done.\n");
    return 0;
}
