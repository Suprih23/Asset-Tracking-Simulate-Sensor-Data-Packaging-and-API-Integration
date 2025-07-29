#ifndef ASSET_TRACKER_H
#define ASSET_TRACKER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Structure to hold GPS coordinates.
 */
typedef struct {
    float latitude;  // Between -90.0 and 90.0 degrees 
    float longitude; // Between -180.0 and 180.0 degrees 
} GpsData;

/**
 * @brief Structure to hold all sensor data.
 */
typedef struct {
    float temperature; // Between -20.0°C and 50.0°C 
    GpsData location;
} SensorData;

/**
 * @brief Generates random sensor data within specified valid ranges.
 *
 * @param data Pointer to a SensorData struct to be filled.
 */
void generate_random_sensor_data(SensorData *data);

/**
 * @brief (API Function) Packages sensor data into a compact hex-encoded payload.
 *
 * Takes sensor data, packs it into a binary format, and encodes it as a hex string.
 * This function simulates the core requirement of preparing data for transmission.
 *
 * @param data The sensor data to package.
 * @param payload_buffer A buffer to store the resulting hex-encoded string.
 * @param buffer_size The size of the payload_buffer.
 * @return True on success, false on failure (e.g., buffer too small).
 */
bool package_data_to_hex(const SensorData *data, char *payload_buffer, size_t buffer_size);

/**
 * @brief (Bonus API Function) Parses a hex-encoded downlink message to get a new reporting interval.
 *
 * This function demonstrates processing server responses to adjust device operation.
 *
 * @param hex_message The hex-encoded string received from the server.
 * @param interval Pointer to an integer where the new reporting interval (in seconds) will be stored.
 * @return True on success, false on parsing failure (e.g., invalid hex string).
 */
bool parse_downlink_to_interval(const char *hex_message, uint32_t *interval);

#endif // ASSET_TRACKER_H