# PSVR2 Poll-Rate Fixer
Simple C++ background application, waits for a new Bluetooth controller connection, and sends a feature report to set the frequency to 133hz.

This repository is temporary until PSVR2toolkit implements poll-rate switching.

### Building
```sh
git clone https://github.com/coolguy1842/PSVR2PollRateFixer
cd PSVR2PollRateFixer

meson setup build
meson compile -C build

./build/PSVR2PollRateFixer
```

### Thanks To
- [PSHidInfo](https://github.com/RealSupremium/PSHidInfo) used as reference for the feature report, and CRC32 implementation