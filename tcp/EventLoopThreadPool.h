/*
    EventLoop的线程池对象，里面存放eventloop事件循环对象，方便主reactor进行reactor的分发。
*/

#pragma once

#include "../base/common.h"

#include <memory>
#include <thread>
#include <vector>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {

private:
    //设置主reactor，当从reactor的数量设置为0的时候，所有工作全都交给主reactor来进行
    EventLoop* main_reactor_;

    //设置线程池，里面存放的是EventLoop的线程
    std::vector<std::unique_ptr<EventLoopThread>> threads_;

    //设置一个存放EentLoop的数组，每一个EventLoopThread线程会返回一个EventLoop的对象，这就是后面的从reactor
    std::vector<EventLoop*> loops_;

    //设置线程池中线程的个数
    int thread_nums_;

    int next_;//从线程池中获取下一个EentLoop来使用

public:
    DISALLOW_COPY_AND_MOVE(EventLoopThreadPool);
    EventLoopThreadPool(EventLoop *loop);
    ~EventLoopThreadPool();

    //设置进程数量
    void SetThreadNums(int thread_nums);

    //开始创建线程池
    void start();

    //获取线程池中的EentLoop
    EventLoop* nextloop();

};
