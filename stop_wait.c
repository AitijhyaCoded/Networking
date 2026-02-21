#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 5

// Function prototypes
int sender(int frame_no);
int receiver(int frame_no);

// Global variable to simulate ACK
int ack_received = 0;

int main() {
    int frame = 0;

    srand(time(0));  // Seed random generator

    while (frame < MAX_FRAMES) {
        printf("\nSender: Sending Frame %d\n", frame);

        if (sender(frame)) {   // If frame successfully delivered
            frame++;           // Move to next frame
        } else {
            printf("Sender: Timeout! Resending Frame %d\n", frame);
        }
    }

    printf("\nAll frames sent successfully!\n");
    return 0;
}

// ------------------- Sender Function -------------------
int sender(int frame_no) {
    ack_received = 0;

    // Send frame to receiver
    ack_received = receiver(frame_no);

    if (ack_received == 1) {
        printf("Sender: ACK received for Frame %d\n", frame_no);
        return 1;
    } else {
        return 0;  // Timeout or error
    }
}

// ------------------- Receiver Function -------------------
int receiver(int frame_no) {

    int random = rand() % 2;  // 0 or 1

    if (random == 0) {
        printf("Receiver: Frame %d received successfully\n", frame_no);
        printf("Receiver: Sending ACK %d\n", frame_no+1);
        return 1;  // ACK sent
    } else {
        printf("Receiver: Frame %d lost or corrupted!\n", frame_no);
        return 0;  // No ACK (simulate loss)
    }
}