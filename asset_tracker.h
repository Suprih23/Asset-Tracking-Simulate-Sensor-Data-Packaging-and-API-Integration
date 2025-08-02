#ifndef ASSET_TRACKER_H
#define ASSET_TRACKER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float latitude;  // Between -90.0 and 90.0 degrees 
    float longitude; // Between -180.0 and 180.0 degrees 
} GpsData;

typedef struct {
    float temperature; // Between -20.0°C and 50.0°C 
    GpsData location;
} SensorData;

void generate_random_sensor_data(SensorData *data);


bool package_data_to_hex(const SensorData *data, char *payload_buffer, size_t buffer_size);


bool parse_downlink_to_interval(const char *hex_message, uint32_t *interval);

#endif // ASSET_TRACKER_H
