#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
// Mesage queue class, that handles 1:1 communication, you can pass it a lock time to wait for, if you passed 0, it will wait forever till message is recived
template <typename  T> 
class MessageQueue
{
public:
    void send(T &&msg);
    T recieve();
    MessageQueue(int x) : lockingTime(x) {}

private:
    int lockingTime;
    std::mutex _mtx;
    std::condition_variable _cnd;
    std::deque<T> _queue;
};
template<typename T>
void MessageQueue<T>::send(T&& x) {
  std::lock_guard<std::mutex> lock(_mtx);
  _queue.push_back(std::move(x));
  _cnd.notify_one();
}
template<typename T>
T MessageQueue<T>::recieve() {
  std::unique_lock<std::mutex> ulock(_mtx);
  if (lockingTime != 0) {
    if (_cnd.wait_for(ulock, std::chrono::milliseconds(lockingTime),
                      [this] { return !this->_queue.empty(); })) {
      T msg = std::move(_queue.back());
      _queue.pop_back();
      return msg;
    } else {
      return (T)std::cv_status::timeout;
    }
  } else {
      _cnd.wait(ulock, [this] { return !this->_queue.empty(); });
      T msg = std::move(_queue.back());
      _queue.pop_back();
      return msg;
  }
}