/*
    监听socket的封装，相当于主reactor，主要负责新连接的接受操作
 */

#pragma once
#include "../base/common.h"
#include <memory>
#include <functional>

class EventLoop;
class Channel;

class Acceptor {

private:

    EventLoop* loop_;//将将监听服务所使用的事件循环

    int listenfd_;//监听socket的fd

    std::unique_ptr<Channel> channel_;//监听服务器所使用的channel

    //当新的客户端连接到来时，执行连接操作，但其主要的操作回放到TcpServe中，这里主要是负责接受连接
    std::function<void(int)> new_connection_callback_;

public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    Acceptor(EventLoop *loop, const char * ip, const int port);
    ~Acceptor();

    void set_newconnection_callback(std::function<void(int)> const &callback);

    // 创建socket
    void Create();

    // 与ip地址绑定
    void Bind(const char* ip, const int port);
    
    // 监听Socket
    void Listen();

    // 接收连接
    void AcceptConnection();

};