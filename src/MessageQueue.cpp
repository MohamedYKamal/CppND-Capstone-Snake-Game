#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include "MessageQueue.h"

void MessageQueue<T>::send(T&& x) {
  std::lock_guard<std::mutex> lock(_mtx);
  _queue.push_back(std::move(x));
  lock.notify_one();
}

T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> ulock(_mtx);
  if (lockingTime != 0) {
    if (_cnd.wait_for(ulock, std::chrono::milliseconds(lockingTime),
                      [this] { return !this->_queue.isempty(); })) {
      T msg = std::move(_queue.back());
      _queue.pop_back();
      return msg
    } else {
      return (T)cv_status::timeout;
    }
  } else {
      _cnd.wait(ulock, [this] { return !this->_queue.isempty(); });
      T msg = std::move(_queue.back());
      _queue.pop_back();
      return msg
  }
}