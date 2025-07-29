#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "asset_tracker.h"

int main() {
    // Seed the random number generator once.
    srand(time(NULL));
    
    printf("--- Asset Tracker Simulation ---\n\n");

    // --- Test Case 1: Normal Run 
    printf("--- Test Case 1: Normal Run ---\n");
    SensorData sensor_data;
    generate_random_sensor_data(&sensor_data);
    printf("Generated Data: Temp=%.2f C, Lat=%.4f, Lon=%.4f\n",
           sensor_data.temperature, sensor_data.location.latitude, sensor_data.location.longitude);

    char payload_hex_buffer[30]; // Buffer for 10 bytes -> 20 hex chars + null
    if (package_data_to_hex(&sensor_data, payload_hex_buffer, sizeof(payload_hex_buffer))) {
        // Print the payload as a hex-encoded string 
        printf("Hex-Encoded Payload: %s\n", payload_hex_buffer);
    } else {
        fprintf(stderr, "Error: Failed to package data.\n");
    }

    // --- Bonus: Process a valid downlink message ---
    const char* downlink_msg = "0000003C"; // 60 seconds
    uint32_t new_interval;
    if (parse_downlink_to_interval(downlink_msg, &new_interval)) {
        // Print the updated interval [cite: 16]
        printf("Parsed downlink '%s'. Updated reporting interval: %u seconds\n", downlink_msg, new_interval);
    }
    printf("\n");

    // --- Test Case 2: Data Limits 
    printf("--- Test Case 2: Data Limits ---\n");
    SensorData min_data = {-20.0f, {-90.0f, -180.0f}};
    SensorData max_data = {50.0f, {90.0f, 180.0f}};
    
    package_data_to_hex(&min_data, payload_hex_buffer, sizeof(payload_hex_buffer));
    printf("Min Data Payload: %s\n", payload_hex_buffer);

    package_data_to_hex(&max_data, payload_hex_buffer, sizeof(payload_hex_buffer));
    printf("Max Data Payload: %s\n", payload_hex_buffer);
    printf("\n");


    // --- Test Case 3: API Fail (Invalid Downlink) 
    printf("--- Test Case 3: API Fail ---\n");
    const char* invalid_downlink = "INVALID_HEX";
    printf("Attempting to parse invalid downlink message: '%s'\n", invalid_downlink);
    if (!parse_downlink_to_interval(invalid_downlink, &new_interval)) {
        // Expected to fail and print an error message from the API function.
        printf("Graceful handling of API failure successful.\n");
    }
    printf("\n");

    return 0;
}