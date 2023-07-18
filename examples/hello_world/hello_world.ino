#include <Arduino.h>
#include <monitor_printf.h>

// Set this to 1 to enable debug output to the serial monitor.
// Set this to 0 when releasing code to use without the IDE, USB port, and serial monitor.
#define USE_MONITOR_PORT 1

// Define the port to be used by the global instance named "the_serial_monitor".
#if USE_MONITOR_PORT
#define MONITOR_PORT &Serial  // Enable printf output to the serial monitor port identified by variable "Serial".
#else
#define MONITOR_PORT NULL     // Disable printf output when using WITHOUT the IDE, USB port, and serial monitor.
#endif

void setup() {
  monitor.begin(MONITOR_PORT, 115200);
  monitor.printf("Hello world!\n");
}

int count = 0;
void loop() {
  delay(1500);
  count++;
  monitor.printf("count = %d\n", count);
}
