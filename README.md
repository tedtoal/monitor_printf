# monitor_printf
## library for printf to the Arduino IDE serial monitor window or to other ports

This library makes it easy to use printf()-style function calls in your Arduino C++ code to print messages to
the Arduino IDE serial monitor window or to other serial ports.

## Quick start

Here is sample code if you want to just get going:

```
#include <monitor_printf.h>

// Set this to 1 to enable debug output to the serial monitor.
// Set this to 0 when releasing code to use without the IDE, USB port, and serial monitor.
#define MY_PROGRAM_DBG 1

void setup() {
  monitor.begin(MY_PROGRAM_DBG, 115200);
  monitor.printf("MyProgram is running\n");
}
```

## Basics

The library consists of a .cpp/.h file pair that define a C++ class named `monitor_printf` containing several
member functions.

### Include file

To use the library, include the library header file in your code file:

```
  #include <monitor_printf.h>
```

### Member functions `begin()` and `printf()`

**`begin()`** initializes the class instance for printing, including initialization of the Serial object that
communicates with the serial monitor window. Alternatively, `begin()` can disable the `printf()` function,
used when running code without the USB port and IDE monitor window.

**`printf()`** prints a character string to the serial monitor window, using a standard printf-style format string
to control the formatting of the text, and with optional arguments that contain values to be printed or are
arguments used by format string "% specifiers".

### Class instance

**`the_monitor_printf`** is a global instance of the monitor_printf class that uses the standard Arduino `Serial`
object, which communicates over the USB port to the IDE serial monitor window.

**`monitor`** is a symbol that is #defined to equal `the_monitor_printf`, and is the normal variable you would use
to call `printf()`. Either of these two methods will work to call `printf()` to output to the serial monitor:

```
  monitor.printf("Hello world!\n");  // ** preferred **

  the_monitor_printf.printf("Hello world!\n"); // ** optional **
```

Since `monitor` might possibly already be used elsewhere in your code, a method is provided to not define it. Simply
define the symbol `DONT_DEFINE_monitor` before including the library header file:

```
#define DONT_DEFINE_monitor
#include <monitor_printf.h>

// Define something shorter than 'the_monitor_printf' to use, so we can do mon.printf(...);
#define mon the_monitor_printf
```

### Call `begin()` to initialize

The `begin()` member function must be called to initialize before printing. It has several arguments, all optional:

```
void begin(bool enable = false, unsigned long baud = 115200, byte config = SERIAL_8N1);
```

**`enable`** is normally set to **`true`**, which causes `Serial.begin()` to be called to initialize serial communications,
using the arguments **`baud`** and **`config`** to set serial speed and data/parity/stop bits. Typically **`config`** is left
at its default of `SERIAL_8N1`.

When you are running your code outside the Arduino IDE, with no USB port and no serial monitor, it is **crucial that you
set `enable = false`** Otherwise, your code will hang when it tries to initialize a non-existent serial port. You might
define a variable for enabling monitor_printf at the start of your file and use it in the call to `begin()`:

```
#include <monitor_printf.h>

// Set this to 1 to enable debug output to the serial monitor.
// Set this to 0 when releasing code to use without the IDE, USB port, and serial monitor.
#define MY_PROGRAM_DBG 1

void setup() {
  monitor.begin(MY_PROGRAM_DBG, 115200);
  monitor.printf("MyProgram is running\n");
}
```

When monitor_printf is disabled, printf() does nothing if called.

You might also choose to keep monitor_printf disabled until some condition occurs that triggers the beginning of
debug output.

### Call `printf()` to print

The ``printf()`` member function uses an internal C++ library function (``vsnprintf()``) to format your text string
for output. It uses the ``format`` argument you specify, plus any additional arguments that follow it as needed to
process the printf() "%-specifiers".

```
int x = 123;
long Z = 123456789;
char c = 'A';
char* p = "Goodbye";

monitor.printf("Value of x: %d   Z: %ld   c: %c   p: %s\n", x, Z, c, p);
```

If you don't have a good working knowledge of the other aspects of using "%-specifiers", such as field width and
justification, look them up and study them a bit.

Although most standard printf() functionality should be present, I've noticed that **%u** is missing (use
**`%d`** instead, not ideal but it works usually). Also, something of great importance to me that was missing was
the ability to format and print ``float`` and ``double`` values. These are apparently disabled in standard versions
of Arduino libraries, but can be enabled through some mechanism that I have not yet figured out. If you need to print
floating point values, you may want to use my **``floatToString``** library.

## Additional details

### Doing Serial.begin elsewhere in the code

When ``begin()`` is called with ``enable = false``, Serial.begin() is not called, so the serial port remains uninitialized.
You might wish to initialize it yourself, and you could do that as follows:

```
void setup() {
  // Initialize Serial communications
  Serial.begin(9600);

  // Initialize monitor for printf, initially disabled, no serial initialization done
  monitor.begin(false);

  // Enable monitor printf function.
  monitor.setEnabled(true);
}
```

This uses the ``setEnabled()`` member function, which simply enables or disables printf() to do its work. It could be
called at various times to enable and disable debug output in your code, if that is needed for some reason.

### Buffer size grows automatically

The ``printf()`` member function formats the string to be output into a buffer, then writes the string to the output using
the `Serial.write()` function. The buffer is automatically allocated, and if the initial buffer size is not big enough for
the character string you are printing, it automatically reallocates the buffer to a larger size that will hold it. It
currently never reduces the buffer size, but if for some reason you wanted to do that, you can do it like this:

```
// Output a very long string.
monitor.printf("very long string....");
// Reallocate buffer to a small one. It will grow as needed.
monitor.setBufSize(20);
```

This uses the ``setBufSize()`` member function, which frees the existing buffer, if any, then allocates a new buffer
of the specified size. It is used by ``begin()`` and ``printf()`` when buffer allocation is needed.

### Using more instances of the serial_monitor class for other serial ports

The above descriptions pretended that the serial_monitor class specifically uses the `Serial` object, which is the
standard serial port object for Arduino microcomputers. However, the class actually allows any serial port to be
used, so long as the port's variable is an instance of class `HardwareSerial` (or a class derived from it). The
`HardwareSerial` class is defined in the Arduino library to encompass the functionality needed for a serial port, and
the standard `Serial` object is an instance of the `HardwareSerial` class.

If you are using a microcomputer with hardware that supports multiple serial ports, there is a good chance that the
Arduino run-time code defines a variable for each one, all derived from `HardwareSerial`. Often these
variables are named `Serial1`, `Serial2`, etc. You can try putting one of those variables in your code and checking
to see if the code compiles. Right-clicking on a such a system variable within the IDE and choosing "Go to Definition"
opens the file where the variable is defined, and this is often useful to see how it is defined and what class it is.

The C++ **constructor** function for the `monitor_printf` class has one mandatory argument: a pointer to a variable
that is a `HardwareSerial` instance. This allows you to make additional instances of `monitor_printf`, one for each
serial port. The instance defined in the monitor_printf library, named `the_monitor_printf`, uses `Serial` as the
variable, so it connects to the serial monitor port. That declaration looks like this:

```
monitor_printf the_monitor_printf(&Serial);
```

You could do something similar with another serial port. Say you have variable `Serial2` for your second serial port.
You can make a `monitor_printf` instance for it like this:

```
#include <monitor_printf.h>

monitor_printf serial2_printf(&Serial2);

void setup() {
  monitor.begin(true, 115200);
  serial2_printf.begin(true, 9600);

  monitor.printf("Program is running\n");

  serial2.printf("Testing output from serial port 2\n");
}
```

## Contact

If you find a problem, please submit an issue report [here](https://github.com/tedtoal/monitor_printf/issues/new/choose).

You can contact me at [ted@tedtoal.net](ted@tedtoal.net)
