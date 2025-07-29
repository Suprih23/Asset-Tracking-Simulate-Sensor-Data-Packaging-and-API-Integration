================================
Assignment 1: Asset Tracker Simulation
================================

## Description

This program simulates an asset tracking device in C[cite: 4]. It performs the following actions:
- Generates random temperature and GPS (latitude, longitude) data internally[cite: 7].
- Implements a simple "API" in `asset_tracker.c` and `asset_tracker.h` to package the sensor data into a compact payload.
- Encodes and prints the payload as a hexadecimal string to the console to simulate transmission[cite: 9].
- Includes a bonus feature to parse a hex-encoded downlink message to extract a new reporting interval, demonstrating server response handling[cite: 10].

## Assumptions

- The "appropriate API" is implemented as a set of functions within the provided source files (`asset_tracker.c`, `asset_tracker.h`) for this simulation[cite: 8].
- To create a "compact binary payload," floating-point sensor values are converted to scaled 16-bit and 32-bit integers before being packed. This is a common embedded systems technique to reduce payload size.
- The simulation runs through a series of hardcoded test cases to demonstrate functionality without requiring user input[cite: 11].

## How to Compile

The program is written in standard C and can be compiled using GCC or a similar toolchain[cite: 3]. Navigate to the directory containing the source files and run the following command:

`gcc main.c asset_tracker.c -o asset_tracker_simulation`

## How to Run

After successful compilation, execute the program from your terminal:

`./asset_tracker_simulation`

The program will run once, executing the predefined test cases for a normal run, data limits, and API failure, and then exit.

## Expected Output

The program will print the results of its test runs to the console, including:
1. A hex-encoded payload for randomly generated data[cite: 14, 15].
2. The updated reporting interval extracted from a sample downlink message[cite: 16].
3. Payloads for minimum and maximum sensor value limits.
4. An error message demonstrating graceful failure when parsing an invalid downlink message.

Example output snippet:
--- Test Case 1: Normal Run ---
Generated Data: Temp=25.12 C, Lat=45.1234, Lon=-75.5678
Hex-Encoded Payload: 09d0006e3952fc9b744e
Parsed downlink '0000003C'. Updated reporting interval: 60 seconds