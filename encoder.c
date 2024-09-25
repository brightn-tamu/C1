#include <stdio.h>
#include <string.h>

#define XOR_KEY 0xAA  // XOR key (same as in the server)
#define SHIFT_VALUE 3 // Number of positions to shift (same as in the server)

// Encoder function
void encode_message(char* message, char* encoded_message) {
    int i = 0;
    while (message[i] != '\0') {
        // Perform XOR first and then shift left by SHIFT_VALUE
        encoded_message[i] = (message[i] ^ XOR_KEY) << SHIFT_VALUE;
        i++;
    }
    encoded_message[i] = '\0'; // Null-terminate the encoded message
}

// Print the encoded message as hexadecimal values
void print_encoded_message(const char* encoded_message) {
    printf("Encoded message: ");
    for (int i = 0; encoded_message[i] != '\0'; i++) {
        printf("0x%02x, ", (unsigned char)encoded_message[i]);
    }
    printf("0x00\n"); // End with 0x00 for the null terminator
}

int main() {
    char message[256];
    char encoded_message[256];

    printf("Enter the message to encode: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Remove newline character

    // Encode the message
    encode_message(message, encoded_message);

    // Display the encoded message in hexadecimal format
    print_encoded_message(encoded_message);

    return 0;
}
