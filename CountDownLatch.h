#pragma once

#include <mutex>
#include <condition_variable>

class CountDownLatch {
public:
    CountDownLatch(uint32_t count) : m_count(count) {}

    void countDown() noexcept {
        std::lock_guard<std::mutex> guard(m_mutex);
        if (0 == m_count) {
            return;
        }
        --m_count;
        if (0 == m_count) {
            m_cv.notify_all();
        }
    }

    void await() noexcept {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return 0 == m_count; });
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    uint32_t m_count;
};
