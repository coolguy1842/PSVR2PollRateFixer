#include <CRC32.hpp>

uint32_t calculateCRC32(uint8_t seedByte, const uint8_t* data, size_t dataSize) {
    uint32_t crc = Crc32Table[(uint8_t)~seedByte] ^ 0xFFFFFF;

    for(size_t i = 0; i < dataSize - 4; i++) {
        crc = (crc >> 8) ^ Crc32Table[(crc ^ data[i]) & 0xFF];
    }

    return ~crc;
}