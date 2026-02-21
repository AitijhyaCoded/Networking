#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>   // For sleep()

#define MAX 32
#define TIMEOUT 2     // Timeout in seconds

int TOTAL_SEQ;
int WINDOW_SIZE;

int SF = 0;   // First unacknowledged
int SN = 0;   // Next to send

int m;

int ack[MAX] = {0};
int received[MAX] = {0};

int lost_position;

int receiver(int seq_no, int position);
void sender();

int main() {

    printf("Enter number of sequence bits (m): ");
    scanf("%d", &m);

    TOTAL_SEQ = pow(2, m);
    WINDOW_SIZE = pow(2, m - 1);   // Selective Repeat rule

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
                    printf("Sender: Waiting for ACK...\n");
                    sleep(TIMEOUT);   // 🔥 Real timeout
                    printf("Timeout! Resending Frame %d\n\n", SN);

                    // Retransmit only this frame
                    if (receiver(seq_no, position)) {
                        ack[SN] = 1;
                        printf("Sender: ACK received after retransmission for Seq %d\n",
                               seq_no);
                    }
                }
            }

            SN++;
            position++;
        }

        // Slide window forward
        while (SF < total_frames && ack[SF] == 1) {
            SF++;
        }

        SN = SF;
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