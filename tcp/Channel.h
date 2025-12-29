/*
    封装所关心的事件，以及事件的各种对应的处理方法，设置对应的回调函数
*/

#pragma once

#include "../base/common.h"

#include <functional>
#include <memory>

class EventLoop;

class Channel {

private:
    int fd_;//事件id

    EventLoop* loop_;//事件循环对象，每一个loop对象都管理在着多个channel

    uint16_t listen_events_;//监听的事件
    uint16_t ready_events_;//准备就绪的事件

    bool in_epoll_;//判断是否当前channel在epoll上面

    std::function<void()> read_callback_;//读的回调函数，用来处理读事件
    std::function<void()> write_callback_;//写事件的回调函数

    bool tied_;//判断是否跟connection连接类对象相关联

    //通过弱引用的方式来判断当前连接是否被销毁了，同时还可以保证当前连接在channel进行处理的时候不会被突然销毁
    std::weak_ptr<void> tie_;

public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(int fd, EventLoop * loop);
        
    ~Channel();

    void HandleEvent() ; // 处理事件
    void HandleEventWithGuard() ;
    void EnableRead();  // 允许读
    void EnableWrite(); // 允许写
    void EnableET(); // 以ET形式触发
    void DisableWrite();

    int fd() const;  // 获取fd
    short listen_events() const; // 监听的事件
    short ready_events() const; // 准备好的事件

    bool IsInEpoll() const; // 判断当前channel是否在poller中
    void SetInEpoll(bool in = true); // 设置当前状态为poller中
        

    void SetReadyEvents(int ev);
    void set_read_callback(std::function<void()> const &callback);// 设置回调函数
    void set_write_callback(std::function<void()> const &callback);

    void Tie(const std::shared_ptr<void> &ptr); // 设定tie
};