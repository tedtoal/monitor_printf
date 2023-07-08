/*
  monitor_printf.h - Defines class monitor_printf containing a function to do
  printfs to the serial monitor, with one global instance of it.
  Created by Ted Toal, July 5, 2023.
  Released into the public domain.
*/
#ifndef monitor_printf_h
#define monitor_printf_h

#include <Arduino.h>

class monitor_printf {

protected:
  bool _enabled;
  uint16_t _buf_size;
  char *_buf;

public:
  /**************************************************************************/
  /*!
    @brief    Constructor.
    @note     Example:
                #include <monitor_printf.h>
                void setup() {
                  monitor.begin(true);
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
  monitor_printf() : _enabled(false), _buf_size(0), _buf(NULL) {}

  /**************************************************************************/
  /*!
    @brief      Destructor. Release buffer memory.
  */
  /**************************************************************************/
  ~monitor_printf() { setBufSize(0); }

  /**************************************************************************/
  /*!
    @brief    Initialize monitor_printf object.
    @param    enable      true to enable printing, false skips initialization
                          of the Serial object using baud and config and it
                          makes printf do nothing
    @param    buf_size    initial size limit for a string printed by printf
    @param    baud        serial port speed in bits/second
    @param    config      SERIAL_ constant defining data, parity, and stop bits
    @note     When enable is true, this initializes the Serial object with baud
              and config, empties the Serial input buffer, and does short
              delays to ensure smooth operation. If false, that initialization
              is not done and must be done externally to this. Be sure to use
              false for enable when running WITHOUT A USB connection (in your
              final system with no USB), else your system will hang.
  */
  /**************************************************************************/
  void begin(bool enable = false, uint16_t buf_size = 150,
             unsigned long baud = 115200, byte config = SERIAL_8N1);

  /**************************************************************************/
  /*!
    @brief    Get current enable state.
    @returns  true if enabled, false if disabled.
  */
  /**************************************************************************/
  bool isEnabled(void) { return (_enabled); }

  /**************************************************************************/
  /*!
    @brief    Set enable state.
    @param    enable    true to enable printing, false makes printf do nothing
  */
  /**************************************************************************/
  void setEnabled(bool enable) { _enabled = enable; }

  /**************************************************************************/
  /*!
    @brief    Get current buffer size.
    @returns  Buffer size in characters including 1 for final nul, 0 if no
    buffer
  */
  /**************************************************************************/
  uint16_t getBufSize(void) { return (_buf_size); }

  /**************************************************************************/
  /*!
    @brief    Set buffer size.
    @param    buf_size  Buffer size in characters including 1 for final nul, or
                        0 to only deallocate current buffer, if any
    @note     It is not necessary for the user to call this function, as the
              buffer size is managed automatically, see printf() note
  */
  /**************************************************************************/
  void setBufSize(uint16_t buf_size);

  /**************************************************************************/
  /*!
    @brief    print string to serial monitor using printf-style format string
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
  void printf(const char *format, ...);
};

/**************************************************************************/
/*!
  @brief    'the_monitor_printf' is the single global instance of a
            monitor_printf object, and it can be referred to using the #define
            'monitor' (unless that is disabled with DONT_DEFINE_monitor).
*/
/**************************************************************************/

extern monitor_printf the_monitor_printf;

#ifndef DONT_DEFINE_monitor
#define monitor the_monitor_printf
#endif

#endif // monitor_printf_h
