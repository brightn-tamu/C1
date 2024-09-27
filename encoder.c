//encoder.c
#include <stdio.h>
#include <string.h>

#define XOR_KEY 0xAA  // XOR key
#define CAESAR_SHIFT 1 // Shift value for Caesar cipher

// Encoder function
void XOR_encode(const char* message, unsigned char* encoded_message) {
    int i = 0;
    while (message[i] != '\0') {
        // Perform XOR
        encoded_message[i] = ((unsigned char)message[i]) ^ XOR_KEY;
        i++;
    }
    encoded_message[i] = '\0'; // Null-terminate the encoded message
}

// Decoder function
void XOR_decode(const unsigned char* encoded_message, char* decoded_message) {
    int i = 0;
    while (encoded_message[i] != '\0') {
        // Reverse XOR
        decoded_message[i] = (char)((encoded_message[i]) ^ XOR_KEY);
        i++;
    }
    decoded_message[i] = '\0'; // Null-terminate the decoded message
}

// Caesar Cipher Encoder function (Shift characters by CAESAR_SHIFT)
void caesar_encode(const unsigned char* message, unsigned char* encoded_message) {
    int i = 0;
    while (message[i] != '\0') {
        // Shift characters by CAESAR_SHIFT
        encoded_message[i] = message[i] + CAESAR_SHIFT;
        i++;
    }
    encoded_message[i] = '\0'; // Null-terminate the encoded message
}

// Caesar Cipher Decoder function (Reverse the shift)
void caesar_decode(const unsigned char* encoded_message, char* decoded_message) {
    int i = 0;
    while (encoded_message[i] != '\0') {
        // Reverse the shift by CAESAR_SHIFT
        decoded_message[i] = encoded_message[i] - CAESAR_SHIFT;
        i++;
    }
    decoded_message[i] = '\0'; // Null-terminate the decoded message
}

// Print the encoded message as hexadecimal values
void print_encoded_message(const unsigned char* encoded_message) {
    printf("Encoded message (hex): ");
    for (int i = 0; encoded_message[i] != '\0'; i++) {
        printf("0x%02x, ", encoded_message[i]);
    }
    printf("0x00\n"); // End with 0x00 for the null terminator
}

// Print the original message in hexadecimal values
void print_original_hex(const char* message) {
    printf("Original message (hex): ");
    for (int i = 0; message[i] != '\0'; i++) {
        printf("0x%02x, ", (unsigned char)message[i]);
    }
    printf("0x00\n"); // End with 0x00 for the null terminator
}

// Print the decoded message as hexadecimal values
void print_decoded_hex(const char* decoded_message) {
    printf("Decoded message (hex): ");
    for (int i = 0; decoded_message[i] != '\0'; i++) {
        printf("0x%02x, ", (unsigned char)decoded_message[i]);
    }
    printf("0x00\n"); // End with 0x00 for the null terminator
}

int main() {
    char message[256];
    unsigned char xor_encoded_message[256];
    unsigned char caesar_encoded_message[256];
    char decoded_message[256];

    printf("Enter the message to encode: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Remove newline character

    // Display the original message in hexadecimal format
    print_original_hex(message);

    // XOR Encode the message
    XOR_encode(message, xor_encoded_message);

    // Display the XOR encoded message in hexadecimal format
    printf("XOR Encoded ");
    print_encoded_message(xor_encoded_message);

    // Caesar Encode (after XOR encoding)
    caesar_encode(xor_encoded_message, caesar_encoded_message);

    // Display the XOR + Caesar encoded message in hexadecimal format
    printf("XOR + Caesar Encoded ");
    print_encoded_message(caesar_encoded_message);

    // Caesar Decode (first reverse Caesar encoding)
    caesar_decode(caesar_encoded_message, xor_encoded_message);

    // XOR Decode (reverse XOR after Caesar decode)
    XOR_decode(xor_encoded_message, decoded_message);

    // Display the decoded message in hexadecimal format
    printf("XOR + Caesar Decoded ");
    print_decoded_hex(decoded_message);

    // Print the decoded message
    printf("Decoded message (string): %s\n", decoded_message);

    return 0;
}