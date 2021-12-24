#include "watchdog.hpp"

Watchdog::Watchdog(std::chrono::seconds timeout, std::function<void()> callback_on_timeout)
    : m_callback_on_timeout(std::move(callback_on_timeout))
{
    m_watcher_thread = std::thread(&Watchdog::WatcherThread, this, timeout);
}

Watchdog::~Watchdog()
{
    {
        std::lock_guard lock(m_mutex);
        m_stopped = true;
    }
    m_cv.notify_one();
    m_watcher_thread.join();
}

void Watchdog::WatcherThread(std::chrono::seconds timeout)
{
    std::unique_lock lock(m_mutex);
    const bool stopped_before_timeout =
        m_cv.wait_for(lock, timeout, [this]() { return m_stopped; });
    if (!stopped_before_timeout)
    {
        m_callback_on_timeout();
    }
}
