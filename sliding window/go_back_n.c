#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>   
#define TIMEOUT 2

int TOTAL_FRAMES;
int WINDOW_SIZE;

int SF = 0;   // First unacknowledged
int SN = 0;   // Next to send
int RN = 0;   // Receiver expects

int m;

int lost_position;  // position inside window to lose

// Function prototypes
int receiver(int frame_no, int position);
void sender();

int main() {

    printf("Enter number of sequence bits (m): ");
    scanf("%d", &m);

    TOTAL_FRAMES = pow(2, m);
    WINDOW_SIZE = TOTAL_FRAMES - 1;

    printf("Total Sequence Numbers = %d\n", TOTAL_FRAMES);
    printf("Window Size = %d\n\n", WINDOW_SIZE);

    srand(time(0));
    sender();

    return 0;
}

// ------------------- Sender -------------------
void sender() {

    while (SF < TOTAL_FRAMES) {

        lost_position = rand() % WINDOW_SIZE + 1;

        printf("\n(New Window: Frame %d to %d)\n", SF, SF + WINDOW_SIZE - 1);
        printf("Simulated Lost Position in Window: %d\n\n", lost_position);

        int position = 1;

        while ((SN < SF + WINDOW_SIZE) && (SN < TOTAL_FRAMES)) {

            int seq_no = SN % TOTAL_FRAMES;

            printf("Sender: Sending Frame %d (Seq %d)\n", SN, seq_no);

            int ack = receiver(seq_no, position);

            if (ack == 1) {
                printf("Sender: ACK received for Seq %d\n", seq_no);
                SF++;
            } else {

                printf("Sender: Waiting for ACK...\n");
                sleep(TIMEOUT);   // 🔥 Proper timeout delay

                printf("Sender: Timeout occurred! Go-Back from Frame %d\n", SF);
                SN = SF;
                break;
            }

            SN++;
            position++;
        }
    }

    printf("\nAll frames transmitted successfully!\n");
}

// ------------------- Receiver -------------------
int receiver(int frame_no, int position) {

    if (position == lost_position) {
        printf("Receiver: Frame with Seq %d lost (chosen by rand)!\n", frame_no);
        return 0;
    }

    if (frame_no == (RN % TOTAL_FRAMES)) {
        printf("Receiver: Frame with Seq %d received correctly\n", frame_no);
        printf("Receiver: Sending ACK %d\n", frame_no + 1);
        RN++;
        return 1;
    } else {
        printf("Receiver: Out-of-order frame %d discarded\n", frame_no);
        return 0;
    }
}
