/*
  monitor_printf.cpp - Defines class monitor_printf containing a function to do
  printfs to the serial monitor, with one global instance of it.
  Created by Ted Toal, July 5, 2023.
  Released into the public domain.
*/
#include <Arduino.h>
#include <monitor_printf.h>
#include <stdarg.h>
#include <stdio.h>

/**************************************************************************/

#ifndef DONT_DEFINE_the_monitor_printf
monitor_printf the_monitor_printf;
#endif

/**************************************************************************/

void monitor_printf::begin(HardwareSerial* serial, unsigned long baud,
    byte config) {
  if (serial != NULL)
    _serial = serial;
  setEnabled(_serial != NULL);
  setBufSize(INITIAL_MONITOR_PRINTF_BUF_SIZE);
  // Only initialize serial port if 'serial' argument is not NULL.
  if (serial != NULL) {
    // This delay seems to be necessary, at least for SAMD21 hardware.
    delay(1000);
    _serial->begin(baud, config);
    // Discard anything in serial receive buffer.
    while (! *_serial)
      ;
    // Final delay.
    delay(200);
  }
}

/**************************************************************************/

void monitor_printf::setBufSize(uint16_t buf_size) {
  if (_buf != NULL) {
    free(_buf);
    _buf = NULL;
    _buf_size = 0;
  }
  if (buf_size > 0) {
    _buf_size = buf_size;
    _buf = (char *)malloc(_buf_size);
  }
}

/**************************************************************************/
void monitor_printf::printf(const char *format, ...) {
  if (_enabled) {
    va_list args;
    va_start(args, format);
    int N = vsnprintf(_buf, _buf_size, format, args);
    // Reallocate a larger buffer if it was too small.
    if (N >= 0 && N < _buf_size) {
      setBufSize(N + 1);
      N = vsnprintf(_buf, _buf_size, format, args);
    }
    va_end(args);
    if (N >= 0)
      _serial->write(_buf);
    else
      _serial->write("vsnprintf() error occurred!\n");
  }
}

// -------------------------------------------------------------------------
