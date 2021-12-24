#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class Watchdog
{
public:
    Watchdog(std::chrono::seconds timeout, std::function<void()> callback_on_timeout);
    ~Watchdog();

private:
    void WatcherThread(std::chrono::seconds timeout);

    const std::function<void()> m_callback_on_timeout;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::thread m_watcher_thread;
    bool m_stopped = false;
};