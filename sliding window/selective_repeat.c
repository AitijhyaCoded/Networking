#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 32   // Maximum sequence space allowed

int TOTAL_SEQ;
int WINDOW_SIZE;

int SF = 0;   // First unacknowledged
int SN = 0;   // Next to send

int m;

int ack[MAX] = {0};        // ACK status
int received[MAX] = {0};   // Receiver buffer

int lost_position;

// Function prototypes
int receiver(int seq_no, int position);
void sender();

int main() {

    printf("Enter number of sequence bits (m): ");
    scanf("%d", &m);

    TOTAL_SEQ = pow(2, m);
    WINDOW_SIZE = pow(2, m - 1);   // SR rule

    printf("Total Sequence Numbers = %d\n", TOTAL_SEQ);
    printf("Window Size (Selective Repeat) = %d\n\n", WINDOW_SIZE);

    srand(time(0));
    sender();

    return 0;
}

// ------------------- Sender -------------------
void sender() {

    int total_frames = TOTAL_SEQ;

    while (SF < total_frames) {

        // Random lost frame inside window
        lost_position = rand() % WINDOW_SIZE + 1;

        printf("\n(New Window: Frame %d to %d)\n",
               SF, SF + WINDOW_SIZE - 1);
        printf("Simulated Lost Position in Window: %d\n\n",
               lost_position);

        int position = 1;

        while (SN < SF + WINDOW_SIZE && SN < total_frames) {

            int seq_no = SN % TOTAL_SEQ;

            if (ack[SN] == 0) {

                printf("Sender: Sending Frame %d (Seq %d)\n",
                       SN, seq_no);

                if (receiver(seq_no, position)) {
                    ack[SN] = 1;
                    printf("Sender: ACK received for Seq %d\n",
                           seq_no);
                } else {
                    printf("Sender: Frame %d lost! Will resend later.\n",
                           SN);
                }
            }

            SN++;
            position++;
        }

        // Slide window only when base frame acknowledged
        while (SF < total_frames && ack[SF] == 1) {
            SF++;
        }

        SN = SF;  // Reset for retransmission
    }

    printf("\nAll frames transmitted successfully!\n");
}

// ------------------- Receiver -------------------
int receiver(int seq_no, int position) {

    if (position == lost_position) {
        printf("Receiver: Frame with Seq %d lost!\n", seq_no);
        return 0;
    }

    if (received[seq_no] == 0) {
        printf("Receiver: Frame with Seq %d received and buffered\n",
               seq_no);
        received[seq_no] = 1;
    }

    printf("Receiver: Sending ACK %d\n", seq_no);
    return 1;
}