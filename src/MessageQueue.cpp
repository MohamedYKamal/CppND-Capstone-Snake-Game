#include <mutex>
#include <condition_variable>
#include <deque>

#include "MessageQueue.h"

void MessageQueue<T>::send(T&& x) {
  std::lock_guard<std::mutex> lock(_mtx);
  _queue.push_back(std::move(x));
  lock.notify_one();
}

T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> ulock(_mtx);
   _cnd.wait(_mtx, [this]{
    return !this->_queue.isempty();}
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;
}