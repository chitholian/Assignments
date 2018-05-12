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
*  O = 06                                               *
*  Problem number: 0, 3, 6, 8                           *
*   (CPU Scheduling).             *
\*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants.
#define J 11
#define K 11
#define L 28
#define N 64
#define O 6
#define FILE_IN "input_file.in"
#define FILE_OUT_PRIORITY "output_file_priority.out"

FILE *input_file, *output_file;
int i, j, k;
long long total_hole_size, total_process_size, total_unallocated;
char priorities[] = "ATIKURRAHMANCHITHOLIAN"; // A=1,B=2,...
int priority_mod = 22;                        // length of priorities[] char array.
// Structure to hold a process-info.
struct PROCESS
{
    int PID, burst_time, arrival_time, remaining_time;
};
// Declare required function prototypes.
int generateInput();
int runPriorityScheduling();
// Entry of execution.
int main(int argc, char **argv)
{
    // Make sure input generation was successful.
    if (generateInput() == 0)
    {
        if (runPriorityScheduling() != 0)
            fprintf(stderr, "Warning! runPriorityScheduling() returned non-zero status.\n");

        printf("\n\t<===0===0===>\n Task complete, summary written to the output file:"
               "\n\t1. \"%s\".\n",
               FILE_OUT_PRIORITY);
    }
    else
    {
        fprintf(stderr, "Error! Cannot generate input, exiting.\n");
        return 1;
    }
    return 0;
}

// Function to generate input data.
int generateInput()
{
    // Set seed for random.
    srand((unsigned)time(NULL));
    // open input_file in writing mode.
    input_file = fopen(FILE_IN, "w");

    // generate N inputs.
    printf("Generating %d input data.\n", N);
    for (i = 0; i < N; ++i)
    {
        printf("Iteration %d of %d running...\n", i + 1, N);

        // Generate J random burst time.
        printf("Generating %d random burst times from %d to %d and arrival time from %d to %d... ",
               J, 1, K, 0, L);
        for (j = 0; j < K; ++j)
        {
            int burst_time = (rand() % K) + 1, arrival_time = rand() % (L + 1);
            fprintf(input_file, "%d %d    ", burst_time, arrival_time);
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

int runPriorityScheduling()
{
    printf("Running priority scheduling.\n0%%.");
    // open input_file in reading mode.
    input_file = fopen(FILE_IN, "r");
    // open output_file in writing mode.
    output_file = fopen(FILE_OUT_PRIORITY, "w");
    // initialize all stats to zero.
    int total_burst_time = 0, total_turn_around_time = 0, total_waiting_time = 0;
    // run N times.
    for (i = 1; i <= N; ++i)
    {
        int turn_around_time = 0, waiting_time = 0;

        struct PROCESS processes[J];
        int total_time = 0, current_time = 0;
        char gantt[0xffff];
        // input J processes.
        for (j = 0; j < J; j++)
        {
            // input total size.
            fscanf(input_file, "%d %d", &processes[j].burst_time, &processes[j].arrival_time);
            processes[j].remaining_time = processes[j].burst_time;
            total_burst_time += processes[j].burst_time;
        }

        for (j = 1; j <= J; j++)
        {
            int min_priority = 0xff, turn = 0;
            for (k = 0; k < J; k++) {
                if (processes[turn].remaining_time == 0 || (processes[k].remaining_time > 0 && (processes[k].arrival_time < processes[turn].arrival_time ||
                        (processes[k].arrival_time == processes[turn].arrival_time && priorities[(k + 1) % priority_mod] < min_priority)))) {
                    min_priority = priorities[(k + 1) % priority_mod];
                    turn = k;
                }
            }
            while (current_time < processes[turn].arrival_time)
            {
                //fprintf(output_file, "*");
                current_time++;
                gantt[total_time++] = '|';
            }
            waiting_time += (current_time - processes[turn].arrival_time);
            //fprintf(output_file, "*");
            gantt[total_time++] = '|';
            int z = processes[turn].burst_time - 1;
            for (int k = 1; k <= z; ++k)
            {
                // fprintf(output_file, " ");
                gantt[total_time++] = ' ';
            }
            // fprintf(output_file, "%2d", turn + 1);
            sprintf(&gantt[total_time++], "%02d", turn + 1);
            //printf("%s\n", gantt);
            total_time++;
            for (int k = 1; k <= processes[turn].burst_time - 1; ++k)
            {
                // fprintf(output_file, " ");
                gantt[total_time++] = ' ';
            }
            processes[turn].remaining_time -= processes[turn].burst_time;

            current_time += processes[turn].burst_time;
            turn_around_time += (current_time - processes[turn].arrival_time);
        }
        //fprintf(output_file, "*\n");
        gantt[total_time++] = '|';
        gantt[total_time] = '\0';
        fprintf(output_file, "\nIteration %d:\n1. Average waiting time: %.2f\n2. Average turn around time: %.2f\n",
                i, waiting_time * 1.0 / J, turn_around_time * 1.0 / J);
        for (j = 0; j < total_time; j++) {
            fprintf(output_file, "_");
        }
        fprintf(output_file, "\n%s\n", gantt);
        for (j = 0; j < total_time; j++) {
            fprintf(output_file, "-");
        }
        fprintf(output_file, "\n");
        total_waiting_time += waiting_time;
        total_turn_around_time += turn_around_time;
        printf("\r%d%%.", i * 100 / N);
    }
    printf("\nWriting summary... ");
    fprintf(output_file, "\n\n========== Summary ==========\n");
    fprintf(output_file, "Average waiting time    : %.2f\n", total_waiting_time * 1.0 / (N * J));
    fprintf(output_file, "Average turnaround time : %.2f\n", total_turn_around_time * 1.0 / (N * J));
    fprintf(output_file, "Average CPU burst time  : %.2f.\n", total_burst_time * 1.0 / (N * J));

    // close files.
    fclose(input_file);
    fclose(output_file);
    printf("Done.\n");
    return 0;
}
