#pragma once
// Mesage queue class, htat handles 1:1 communication.
template <class T> class MessageQueue
{
public:
    void send(T &&msg);
    T recieve();

private:
    std::mutex _mtx;
    std::condition_variable _cnd;
    std::deque<T> _queue;
}