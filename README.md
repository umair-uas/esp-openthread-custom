# ESP OpenThread RCP with RGB Status

Standalone ESP‑IDF project based on the OpenThread `ot_rcp` example, extended with an addressable RGB (WS2812/NeoPixel) status LED to show RCP lifecycle states.

- Booting: quick cyan blink
- Ready (stack initialized): solid green
- Active (NCP launched): blue heartbeat
- Error (mainloop exit): fast red blink

Disable LED entirely by setting `RGB_STATUS_LED_ENABLED=n` in `menuconfig`.

## Requirements
- ESP‑IDF 6.0+ (ESP32‑H2 or other IEEE 802.15.4 SoCs)
- IDF Component Manager (bundled with ESP‑IDF)

## Quick Start (ESP32‑H2)
```
source $IDF_PATH/export.sh
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.rcp_usb" set-target esp32h2
idf.py -p /dev/ttyACM0 build flash
```

Configure the LED GPIO via `menuconfig` → `RGB Status LED` (default: GPIO 8).

## Project Layout
- `main/` RCP app with RGB status hooks
- `components/rgb_status/` small helper that uses `espressif/led_strip`
- `sdkconfig.defaults` and `sdkconfig.ci.*` presets for RCP modes (USB/UART/SPI)
- `partitions.csv` minimal NVS + app layout

## Notes
- This is RCP firmware (NCP) only; it does not run a full Thread node.
- `managed_components/` is ignored. The Component Manager will fetch dependencies using `components/rgb_status/idf_component.yml` and `dependencies.lock`.

## Border Router (host) example
Run `otbr-agent` on the host (e.g., Raspberry Pi) and point it at the device port:

```
sudo otbr-agent -I wpan0 -B wlan0 \
  spinel+hdlc+uart:///dev/ttyACM0?uart-baudrate=460800 -v
```

## License
This project includes files from Espressif examples that are CC0/Public Domain as marked. Please choose and add a license file for this repository before publishing.
