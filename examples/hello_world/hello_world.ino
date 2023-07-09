#include <monitor_printf.h>

void setup() {
  monitor.begin(true, 115200);
  monitor.printf("Hello world!\n");
}

int count = 0;
void loop() {
  delay(1500);
  count++;
  monitor.printf("count = %d\n", count);
}
