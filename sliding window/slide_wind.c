#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TIMEOUT 2

int TOTAL_FRAMES;
int WINDOW_SIZE;

int SF = 0;   // First unacknowledged (window base)
int SN = 0;   // Next frame to send
int RN = 0;   // Receiver expects

int receiver(int frame_no);
void sender();

int main() {

    printf("Enter Total Frames: ");
    scanf("%d", &TOTAL_FRAMES);

    printf("Enter Window Size: ");
    scanf("%d", &WINDOW_SIZE);

    srand(time(0));

    sender();

    return 0;
}

// ------------------- Sender -------------------
void sender() {

    while (SF < TOTAL_FRAMES) {

        printf("\nWindow: ");
        for (int i = SF; i < SF + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            printf("%d ", i);
        }
        printf("\n");

        int error_occurred = 0;

        // Send full window
        for (SN = SF; SN < SF + WINDOW_SIZE && SN < TOTAL_FRAMES; SN++) {

            printf("Sender: Sending Frame %d\n", SN);

            if (!receiver(SN)) {
                error_occurred = 1;
            }
        }

        if (error_occurred) {
            printf("\nSender: Waiting for ACK...\n");
            sleep(TIMEOUT);

            printf("Timeout! Go-Back to Frame %d\n", RN);

            // Slide only up to last correctly received frame
            SF = RN;
        }
        else {
            // All frames successful
            SF = RN;
        }
    }

    printf("\nAll frames transmitted successfully!\n");
}

// ------------------- Receiver -------------------
int receiver(int frame_no) {

    int random = rand() % 3;  // 33% loss chance

    if (random == 0) {
        printf("Receiver: Frame %d lost!\n", frame_no);
        return 0;
    }

    if (frame_no == RN) {
        printf("Receiver: Frame %d received correctly\n", frame_no);
        printf("Receiver: Sending ACK %d\n", frame_no + 1);
        RN++;
        return 1;
    }
    else {
        printf("Receiver: Out-of-order frame %d discarded\n", frame_no);
        return 0;
    }
}