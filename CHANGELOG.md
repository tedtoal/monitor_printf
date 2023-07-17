# monitor_printf change log
---

## Table of Contents

* [Release v1.3.1](#release-v131)
* [Release v1.2.1](#release-v121)
* [Release v1.1.1](#release-v111)
---

### Release v1.3.1

1. Bug fixed in automatic buffer reallocation.

2. The 'enable' argument in the begin() function is removed and replaced with
    new argument 'serial'. See README.md for recommended way of supplying this
    argument value to best cope with switching from use of IDE/USB to not.

3. Not documented in release V1.2.1 was that the begin() function buf_size
    argument was removed.
    **** BE SURE TO CHANGE YOUR CALLS TO begin() TO REMOVE THE buf_size ARGUMENT
         AND USE THE NEW FIRST ARG NAMED 'serial' INSTEAD OF 'enable' ****

4. Add support for DONT_DEFINE_the_monitor_printf, see README.md.

5. README.md has been extensively updated and improved.

### Release v1.2.1

1. Add README, CHANGELOG, and LICENSE files.
2. Add HardwareSerial* argument to constructor to allow support for any serial port.
3. Change buffer allocation to automatically resize buffer and start with a default size.

### Release v1.1.1

1. Initial release.

