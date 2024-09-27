#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h> // For printf function
#include <stdlib.h> // For size_t

// Constants
#define XOR_KEY 0xAA  // XOR key
#define CAESAR_SHIFT 1 // Shift value for Caesar cipher

// Function declarations
void XOR_encode(const char* message, unsigned char* encoded_message);
void XOR_decode(const unsigned char* encoded_message, char* decoded_message);
void caesar_encode(const unsigned char* message, unsigned char* encoded_message);
void caesar_decode(const unsigned char* encoded_message, char* decoded_message);
void print_encoded_message(const unsigned char* encoded_message);
void print_original_hex(const char* message);
void print_decoded_hex(const char* decoded_message);

#endif // ENCODER_H
