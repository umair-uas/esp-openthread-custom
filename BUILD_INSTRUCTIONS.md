# ESP OpenThread RCP – Build & Flash

This project builds an OpenThread RCP firmware for ESP32‑H2 (and other IEEE 802.15.4 SoCs) with an RGB status LED.

## Prerequisites
- ESP‑IDF 6.0+ installed and exported (`source $IDF_PATH/export.sh`)
- Python 3.10+ and IDF tools installed

## Configure, Build, Flash (USB Serial/JTAG)
```
source $IDF_PATH/export.sh
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.rcp_usb" set-target esp32h2
idf.py build
idf.py -p /dev/ttyACM0 flash
```

Notes:
- The port may vary (`/dev/ttyACM0`, `/dev/ttyACM1`, etc.).
- To clean: `rm -rf build sdkconfig`.
- The LED GPIO is configurable via `menuconfig` → `RGB Status LED`.

## Alternative RCP Interfaces
- UART: use `sdkconfig.ci.rcp_uart`
- SPI: use `sdkconfig.ci.rcp_spi`

Example:
```
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.rcp_uart" set-target esp32h2
```

## Border Router (host)
Run `otbr-agent` on the host and point it to the device:
```
sudo otbr-agent -I wpan0 -B wlan0 \
  spinel+hdlc+uart:///dev/ttyACM0?uart-baudrate=460800 -v
```

## Troubleshooting
- Wait for response timeout: ensure correct RCP mode and baud (`460800`).
- Operation not permitted: run OTBR commands with `sudo`.
- Device not found: check `/dev/ttyACM*`, `lsusb`, and cable; ensure user is in `dialout`.

## What’s included
- `sdkconfig.defaults` enables INFO logs for easier bring‑up and disables dynamic OT log level.
- `sdkconfig.ci.rcp_usb` disables console to avoid conflicts with RCP over USB.

## Publishing Tips
- Do not commit `build/` or `managed_components/` (already in `.gitignore`).
- Keep `dependencies.lock` to ensure reproducible component versions.
- Add a repository license before making the project public.
sudo otbr-agent -I wpan0 -B wlan0 spinel+hdlc+uart:///dev/ttyACM0?uart-baudrate=460800 -v

# Check Thread status
sudo ot-ctl state
sudo ot-ctl dataset active
sudo ot-ctl ifconfig
sudo ot-ctl ipaddr

# Form new network
sudo ot-ctl dataset init new
sudo ot-ctl dataset commit active
sudo ot-ctl ifconfig up
sudo ot-ctl thread start

# Get network credentials
sudo ot-ctl dataset active -x
```

## Firmware Details

- **Binary Location**: `build/esp_ot_rcp.bin`
- **Size**: ~220KB (with optimizations)
- **Configuration**:
  - USB Serial/JTAG interface
  - OpenThread RCP mode
  - Spinel HDLC protocol
  - INFO level logging
  - Console disabled

## Alternative Configurations

### Hardware UART Mode (Not Recommended for USB Connection)

If you need to use hardware UART pins instead of USB:

```bash
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.rcp_uart" set-target esp32h2
idf.py build
```

This configures:
- UART0 on GPIO pins (TX: GPIO24, RX: GPIO23)
- Requires USB-to-UART adapter or direct GPIO connection
- Use `/dev/ttyUSB0` or `/dev/serial0` instead of `/dev/ttyACM0`

### SPI Mode

```bash
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.rcp_spi" set-target esp32h2
idf.py build
```

Requires SPI wiring between host and ESP32-H2.

## References

- [ESP-IDF OpenThread Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32h2/api-guides/openthread.html)
- [OpenThread Border Router Guide](https://openthread.io/guides/border-router)
- [ESP32-H2 Datasheet](https://www.espressif.com/en/products/socs/esp32-h2)
- [Spinel Protocol Specification](https://github.com/openthread/openthread/blob/main/doc/spinel-protocol-src/spinel-intro.md)

## Version Information

- **ESP-IDF**: v6.0-dev (tested with commit 346870a304)
- **Target**: ESP32-H2
- **OpenThread**: Included in ESP-IDF
- **Last Updated**: 2025-11-27

## Notes

1. **USB vs UART**: This build uses USB Serial/JTAG, which appears as a CDC-ACM device (`/dev/ttyACM*`). This is different from traditional UART which appears as `/dev/ttyUSB*`.

2. **Baud Rate Parameter**: Even though USB doesn't technically use baud rates like UART, the `uart-baudrate=460800` parameter is still required for the otbr-agent to configure the communication properly.

3. **Console Output**: Console output is explicitly disabled in this build to prevent debug messages from interfering with the Spinel protocol communication over USB.

4. **Logging**: Logging is enabled at INFO level in the firmware but doesn't interfere with RCP communication. You can see logs via `idf.py monitor` during development.

5. **Firmware Compatibility**: This firmware is specifically built for ESP32-H2. Do not use it on other ESP32 variants (ESP32-C6, ESP32-H4, etc.) without rebuilding for the correct target.

## Support

For issues or questions:
- ESP-IDF GitHub Issues: https://github.com/espressif/esp-idf/issues
- OpenThread Forum: https://groups.google.com/g/openthread-users
- Espressif Forum: https://www.esp32.com/
