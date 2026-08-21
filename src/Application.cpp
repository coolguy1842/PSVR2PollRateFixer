#include <Application.hpp>
#include <CRC32.hpp>
#include <chrono>
#include <cstring>
#include <set>
#include <thread>

Application::Application() {
    if(hid_init() == -1) {
        setShouldExit();
        fprintf(stderr, "Error initializing hidapi: %ls\n", hid_error(nullptr));

        return;
    }

    m_devicePollThread = std::thread(&Application::devicePollThreadFunc, this);
}

Application::~Application() {
    m_shouldExit = true;
    m_devicePollThread.join();

    hid_exit();
}

void updateReportRate(unsigned char report[0x30], int rate) {
    memcpy(&report[0x2], &rate, sizeof(rate));

    unsigned long crc = calculateCRC32(0x53, report, 0x30);
    memcpy(&report[0x2C], &crc, sizeof(crc));
}

void Application::sendPollratePacket(hid_device_info* info) {
    hid_device* device = hid_open(info->vendor_id, info->product_id, info->serial_number);
    if(device == nullptr) {
        fprintf(stderr, "Failed to open HID device %04X:%04X: %ls\n", info->vendor_id, info->product_id, hid_error(nullptr));
        return;
    }

    printf("Sending poll-rate reports to %04X:%04X\n", info->vendor_id, info->product_id);

    unsigned char report[0x30];
    memset(report, 0, sizeof(report));

    report[0] = 0x08; // Report ID
    report[1] = 0x0E;

    updateReportRate(report, 0);
    hid_send_feature_report(device, report, sizeof(report));

    // poll rate in hz
    int rate = 133;
    updateReportRate(report, (int)rate + (((int)rate / 6) << 16));
    hid_send_feature_report(device, report, sizeof(report));

    printf("Sent poll-rate reports.\n");
    hid_close(device);
}

void Application::devicePollThreadFunc() {
    while(!m_shouldExit) {
        hid_device_info* info = hid_enumerate(0x054C, 0);

        std::set<std::string> newConnected;

        hid_device_info* device = info;
        while(device != nullptr) {
            if(device->bus_type != HID_API_BUS_BLUETOOTH) {
                goto skip;
            }

            switch(device->product_id) {
            case 0x0E45: // left
            case 0x0E46: // right
                break;
            default: goto skip;
            }

            if(m_connectedDevices.contains(device->path)) {
                newConnected.emplace(device->path);
                goto skip;
            }

            printf("%ls, 0x%04X, 0x%04X\n", device->product_string, device->vendor_id, device->product_id);
            sendPollratePacket(device);

            newConnected.emplace(device->path);
            printf("path: %s\n", device->path);
        skip:
            device = device->next;
        }

        m_connectedDevices.swap(newConnected);

        hid_free_enumeration(info);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Application::waitForExit() {
    m_devicePollThread.join();
}

void Application::setShouldExit(bool shouldExit) { m_shouldExit = shouldExit; }
bool Application::getShouldExit() const { return m_shouldExit; }
