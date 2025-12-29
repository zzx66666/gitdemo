/*
    一个ELentoop的线程类，用来创建一个ELentoop的线程，同时启动ELentoop的事件循环
*/

#pragma once
#include "../base/common.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class EventLoop;

class EventLoopThread {

private:

    //用于主线程和在这个线程里面创建的那个eventloop线程之间进行数据传输
    EventLoop* loop_;

    //一个事件循环的线程
    std::thread thread_;

    //加锁避免资源竞争
    std::mutex mutex_;
    std::condition_variable cv_;

    //线程运行的函数
    void ThreadRun();

public:
    DISALLOW_COPY_AND_MOVE(EventLoopThread)
    EventLoopThread();
    ~EventLoopThread();

    EventLoop *StartLoop(); // 启动线程， 使EventLoop成为IO线程
};