

/*
对http server的处理,封装了底层的TcpServer
*/


#pragma once
#include <functional>
#include <memory>
#include <stdio.h>
#include "../base/common.h"
class TcpServer;
class TcpConnection;
class HttpRequest;
class HttpResponse;
class EventLoop;
class HttpServer
{
public:

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    typedef std::function<void(const HttpRequest&, HttpResponse*)> HttpResponseCallback;
    
    DISALLOW_COPY_AND_MOVE(HttpServer);
    HttpServer(EventLoop* loop, const char* ip, const int port);
    ~HttpServer();

    //默认的策略，当用户没有制定回传策略时，调用这个
    void HttpDefaultCallBack(const HttpRequest& request, HttpResponse* resp);

    void SetHttpCallback(const HttpResponseCallback &cb);

    void start();//开始http服务

    //设置connection中的连接上时的回传函数
    void onConnection(const TcpConnectionPtr& conn);

    /*设置主要的逻辑。
    用来处理http请求，根据connection中的独有的的httpConext对象来得到完整的request对象，
    然后交给onRequest函数来进行具体的操作
    对应的connection中的代码如下
            void TcpConnection::HandleMessage(){
                Read();
                if (on_message_)
                {
                    on_message_(shared_from_this());
                }
            }
    当有数据到来时，就会执行这个函数来对数据进行解析（使用httpcontext解析机）
    */
    void onMessage(const TcpConnectionPtr& conn);

    //业务处理与设置响应，判断相应的类型，以及创建响应对象传送给回调函数来完成最后的自定义操作
    void onRequest(const TcpConnectionPtr& conn, const HttpRequest& request);

    void SetThreadNums(int thread_nums);

private:
    
    EventLoop* loop_;//传入主reactor
    
    std::unique_ptr<TcpServer> server_;//封装调用底层的TcpServer，通过设置回调函数，来具体实现怎么解析http协议

    HttpResponseCallback response_callback_;//回调事件，由用户自己决定传递给客户端什么样的response对象
};