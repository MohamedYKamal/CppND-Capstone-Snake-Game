#pragma once
// Mesage queue class, that handles 1:1 communication, you can pass it a lock time to wait for, if you passed 0, it will wait forever till message is recived
template <class T> class MessageQueue
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
}