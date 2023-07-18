/*
  monitor_printf.h - Defines class monitor_printf containing a function to do
  printfs to a serial port, with one global instance that prints to the Arduino
  IDE serial monitor port.
  Created by Ted Toal, July 5, 2023.
  Released into the public domain.


  Software License Agreement (BSD License)

  Copyright (c) 2023 Ted Toal
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  3. Neither the name of the copyright holders nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef monitor_printf_h
#define monitor_printf_h

#include <Arduino.h>

#define INITIAL_MONITOR_PRINTF_BUF_SIZE 150

class monitor_printf {

protected:
  HardwareSerial* _serial;
  bool _enabled;
  uint16_t _buf_size;
  char *_buf;

public:
  /**************************************************************************/
  /*!
    @brief    Constructor.
    @param    serial  Pointer to HardwareSerial object for the serial port to be
                      printf-ed to, or NULL to defer setting the port until
                      begin() is called.
    @note     Example:
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
                  monitor.begin(MONITOR_PORT, 9600);
                  monitor.printf("Printing to monitor, can you see it?\n");
                }
                void loop() {
                  monitor.printf("Numbers from 1 to 10:\n");
                  for (uint8_t i = 0; i < 10; i++)
                    monitor.printf(" %d", i);
                  monitor.printf("\n");
                  delay(10000);
                }
  */
  /**************************************************************************/
  monitor_printf(HardwareSerial* serial = NULL) : _serial(serial),
    _enabled(false), _buf_size(0), _buf(NULL) {}

  /**************************************************************************/
  /*!
    @brief      Destructor. Release buffer memory.
  */
  /**************************************************************************/
  ~monitor_printf() { setBufSize(0); }

  /**************************************************************************/
  /*!
    @brief    Initialize monitor_printf object.
    @param    serial      Pointer to HardwareSerial object for the serial port
                          to be printf-ed to, or NULL to keep the port that was
                          set with the constructor 'serial' argument.
    @param    baud        serial port speed in bits/second.
    @param    config      SERIAL_ constant defining data, parity, and stop bits.
    @note     When 'serial' is not NULL, this initializes the HardwareSerial
              object with baud and config, empties the Serial input buffer, does
              short delays to ensure smooth operation, and enables printfs. If
              'serial' is NULL, that initialization is not done and printfs
              remain disabled.
    @note     Use NULL for your standard monitor serial port when running
              WITHOUT A USB connection (in your final system with no USB), else
              your system will hang because the "Serial" object doesn't exist in
              that case.
  */
  /**************************************************************************/
  void begin(HardwareSerial* serial = NULL, unsigned long baud = 115200,
    byte config = SERIAL_8N1);

  /**************************************************************************/
  /*!
    @brief    Get current enable state.
    @returns  true if enabled, false if disabled.
  */
  /**************************************************************************/
  const bool isEnabled(void) { return (_enabled); }

  /**************************************************************************/
  /*!
    @brief    Set enable state.
    @param    enable    true to enable printing, false makes printf do nothing.
  */
  /**************************************************************************/
  void setEnabled(bool enable) { _enabled = (_serial != NULL) && enable; }

  /**************************************************************************/
  /*!
    @brief    Get current buffer size.
    @returns  Buffer size in chars including 1 for final nul, 0 if no buffer.
  */
  /**************************************************************************/
  const uint16_t getBufSize(void) { return (_buf_size); }

  /**************************************************************************/
  /*!
    @brief    Set buffer size.
    @param    buf_size  Buffer size in characters including 1 for final nul, or
                        0 to only deallocate current buffer, if any.
    @note     It is not necessary for the user to call this function, as the
              buffer size is managed automatically, see printf() note.
  */
  /**************************************************************************/
  void setBufSize(uint16_t buf_size);

  /**************************************************************************/
  /*!
    @brief    print string to serial monitor using printf-style format string.
    @param    format    Format string just like for printf(). Note that some
                        specifiers may not be supported under some versions and
                        settings of some hardware platforms.
    @param    ...       The values to be printed, each corresponding to format
                        string % specifiers or their arguments.
    @note     If the buffer size (buf_size arguments above) is not large enough
              to hold the string resulting from the printf formatting, it is
              automatically reallocated here to a large enough size.
  */
  /**************************************************************************/
  void printf(const char* format, ...);
};

/**************************************************************************/
/*!
  @brief    An instance of a monitor_printf class object is defined with a
            "serial" serial port pointer equal to NULL.
  @note     #define DONT_DEFINE_the_serial_monitor before #including file to not
            create the 'the_monitor_printf' instance.
  @note     You would normally set the serial port to "Serial" when calling the
            begin() function, unless you are releasing a version without a USB
            serial port, in which case you would set it to NULL.
  @note     You can also refer to the_monitor_printf using the #define 'monitor'
            below (unless that is disabled with DONT_DEFINE_monitor).
*/
/**************************************************************************/
#ifndef DONT_DEFINE_the_monitor_printf
extern monitor_printf the_monitor_printf;
#endif

// #define DONT_DEFINE_monitor before #including file to not define 'monitor'
#ifndef DONT_DEFINE_monitor
#define monitor the_monitor_printf
#endif

#endif // monitor_printf_h
