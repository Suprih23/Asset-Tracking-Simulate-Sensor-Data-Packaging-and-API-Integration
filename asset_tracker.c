#include "asset_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Helper function to convert a byte array to a hex string.
static void bytes_to_hex(const uint8_t *bytes, size_t len, char *hex_str) {
    for (size_t i = 0; i < len; ++i) {
        sprintf(hex_str + (i * 2), "%02x", bytes[i]);
    }
}

// Helper function to convert a hex character to its integer value.
static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1; // Invalid character
}

// Helper function to convert a hex string to a byte array.
static bool hex_to_bytes(const char *hex_str, uint8_t *bytes, size_t max_len, size_t *out_len) {
    size_t hex_len = strlen(hex_str);
    if (hex_len % 2 != 0) return false; // Must be even length

    *out_len = hex_len / 2;
    if (*out_len > max_len) return false; // Buffer too small

    for (size_t i = 0; i < *out_len; ++i) {
        int high = hex_char_to_int(hex_str[i * 2]);
        int low = hex_char_to_int(hex_str[i * 2 + 1]);
        if (high == -1 || low == -1) {
            return false; // Invalid hex character
        }
        bytes[i] = (high << 4) | low;
    }
    return true;
}


void generate_random_sensor_data(SensorData *data) {
    // Generate random temperature between -20.0 and 50.0
    data->temperature = ((float)rand() / RAND_MAX) * 70.0f - 20.0f;
    // Generate random latitude between -90.0 and 90.0 
    data->location.latitude = ((float)rand() / RAND_MAX) * 180.0f - 90.0f;
    // Generate random longitude between -180.0 and 180.0 
    data->location.longitude = ((float)rand() / RAND_MAX) * 360.0f - 180.0f;
}

bool package_data_to_hex(const SensorData *data, char *payload_buffer, size_t buffer_size) {
    uint8_t binary_payload[10]; // 2 bytes for temp, 4 for lat, 4 for lon
    
    // For a compact payload, convert floats to scaled integers.
    // Temperature: scale by 100, store as 16-bit signed integer. Range: -2000 to 5000
    int16_t temp_scaled = (int16_t)(data->temperature * 100.0f);
    
    // GPS: scale by 10000, store as 32-bit signed integers.
    int32_t lat_scaled = (int32_t)(data->location.latitude * 10000.0f);
    int32_t lon_scaled = (int32_t)(data->location.longitude * 10000.0f);

    // Pack into binary buffer (network byte order - big-endian)
    binary_payload[0] = (temp_scaled >> 8) & 0xFF;
    binary_payload[1] = temp_scaled & 0xFF;
    
    binary_payload[2] = (lat_scaled >> 24) & 0xFF;
    binary_payload[3] = (lat_scaled >> 16) & 0xFF;
    binary_payload[4] = (lat_scaled >> 8) & 0xFF;
    binary_payload[5] = lat_scaled & 0xFF;
    
    binary_payload[6] = (lon_scaled >> 24) & 0xFF;
    binary_payload[7] = (lon_scaled >> 16) & 0xFF;
    binary_payload[8] = (lon_scaled >> 8) & 0xFF;
    binary_payload[9] = lon_scaled & 0xFF;
    
    // Ensure buffer is large enough for the hex string (2 chars per byte + null terminator)
    if (buffer_size < sizeof(binary_payload) * 2 + 1) {
        return false;
    }

    // Convert binary payload to hex string
    bytes_to_hex(binary_payload, sizeof(binary_payload), payload_buffer);
    
    return true;
}

bool parse_downlink_to_interval(const char *hex_message, uint32_t *interval) {
    uint8_t byte_buffer[4];
    size_t bytes_decoded;

    // Decode hex string into bytes. Handle invalid hex as an API failure case.
    if (!hex_to_bytes(hex_message, byte_buffer, sizeof(byte_buffer), &bytes_decoded)) {
        fprintf(stderr, "Error: Invalid hex string provided for downlink message.\n");
        return false;
    }

    if (bytes_decoded != 4) {
        fprintf(stderr, "Error: Downlink message must be 4 bytes (8 hex characters) long.\n");
        return false;
    }

    // Unpack bytes (assuming network byte order - big-endian) into a 32-bit unsigned integer.
    *interval = ((uint32_t)byte_buffer[0] << 24) |
                ((uint32_t)byte_buffer[1] << 16) |
                ((uint32_t)byte_buffer[2] << 8)  |
                ((uint32_t)byte_buffer[3]);
                
    return true;
}