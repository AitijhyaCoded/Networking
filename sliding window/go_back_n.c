#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>   // For sleep()

#define MAX_FRAMES 5
#define TIMEOUT 2     // Timeout in seconds

// Function prototypes
int sender(int frame_no);
int receiver(int frame_no);

// Global variable
int ack_received = 0;

int main() {
    int frame = 0;

    srand(time(0));

    while (frame < MAX_FRAMES) {

        printf("\nSender: Sending Frame %d\n", frame);

        if (sender(frame)) {
            frame++;   // Move to next frame
        } else {
            printf("Sender: Waiting for ACK...\n");
            sleep(TIMEOUT);   // 🔥 Real timeout delay
            printf("Sender: Timeout! Resending Frame %d\n", frame);
        }
    }

    printf("\nAll frames sent successfully!\n");
    return 0;
}

// ------------------- Sender -------------------
int sender(int frame_no) {

    ack_received = receiver(frame_no);

    if (ack_received == 1) {
        printf("Sender: ACK received for Frame %d\n", frame_no);
        return 1;
    }

    return 0;   // No ACK
}

// ------------------- Receiver -------------------
int receiver(int frame_no) {

    int random = rand() % 2;  // 50% loss chance

    if (random == 0) {
        printf("Receiver: Frame %d received successfully\n", frame_no);
        printf("Receiver: Sending ACK %d\n", frame_no + 1);
        return 1;
    } else {
        printf("Receiver: Frame %d lost or corrupted!\n", frame_no);
        return 0;
    }
}