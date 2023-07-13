# monitor_printf change log
---

## Table of Contents

* [Release v1.3.1](#release-v131)
* [Release v1.2.1](#release-v121)
* [Release v1.1.1](#release-v111)
---

### Release v1.3.1

1. No changes, just note: to upgrade from V1.1.1 to V1.2.1 or V1.3.1, REMOVE THE
    buf_size argument value from the monitor.begin() call, so typically you will
    have 'monitor.begin(true, 115200)' or 'monitor.begin(true, 9600, DATA_8N1)'
    for example.
2. The Arduino library manager did not pick up release V1.2.1, I think because
    it was done before it had finished picking up V1.1.1. I hope it picks up
    this V1.3.1 release.

### Release v1.2.1

1. Add README, CHANGELOG, and LICENSE files.
2. Add HardwareSerial* argument to constructor to allow support for any serial port.
3. Change buffer allocation to automatically resize buffer and start with a default size.

### Release v1.1.1

1. Initial release.

