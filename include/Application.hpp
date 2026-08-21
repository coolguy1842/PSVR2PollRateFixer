#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <hidapi.h>
#include <mutex>
#include <set>
#include <thread>

class Application {
public:
    Application();
    ~Application();

    bool getShouldExit() const;
    void setShouldExit(bool shouldExit = true);

    void waitForExit();

private:
    void sendPollratePacket(hid_device_info* device);
    void devicePollThreadFunc();

private:
    bool m_shouldExit;

    std::set<std::string> m_connectedDevices;

    std::thread m_devicePollThread;
    std::mutex m_deviceMutex;
};

#endif