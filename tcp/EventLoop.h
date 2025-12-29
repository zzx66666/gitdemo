/*
    事件循环的类，对epoll所监听的各种事件进行处理
*/

#pragma once
#include "../base/common.h"
#include <memory>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>

class Epoller;

class EventLoop {

private:
    //一个Epoller的实例对象，负责将channel从epoll上面挂载或删除
    std::unique_ptr<Epoller> poller_;

    //一个消息任务队列，负责处理每一个事件循环线程所负责的channel事件处理
    std::vector<std::function<void()>> to_do_list_;

    //一个任务锁，以便在添加任务时避免资源竞争
    std::mutex mutex_;

    //一个唤醒标识符，在任务队列中有事件时来唤醒处理的程序
    int wakeup_fd_;

    //一个由事件标识符所构建的Channel类
    std::unique_ptr<Channel> wakeup_channel_;

    //一个判断符号，如果任务正在添加并执行的过程中又来了新任务，那么就会再次添加
    bool calling_functors_;

    //该事件循环的线程标识符id
    pid_t tid_;


public:

    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void Loop();//启动事件循环

    void UpdateChannel(Channel* ch);//添加channel事件
    void DeleteChannel(Channel* ch);//删除channel事件

    void DoToDoList();//执行任务的的队列中的任务

    void QueueOneFunc(std::function<void()> fn);//将任务添加进任务队列中

    //将任务分流执行，判断需要在哪个线程中执行，并把任务放到对应的事件循环线程内
    void RunOneFunc(std::function<void()> fn);

    bool IsInLoopThread();//判断要执行该任务的线程是不是当前线程

    //构建属于唤醒Channel的读方法，将该事件中的计数器重新设置为0，避免一直造成epoll读响应
    void HandleRead();
};