
/**
    对HTTP响应进行封装
 */

#pragma once

#include<string>
#include<utility>
#include <map>



class HttpResponse{
    public:
        enum HttpStatusCode
        {
            kUnkonwn = 1,
            k100Continue = 100,
            k200K = 200,
            k400BadRequest = 400,
            k403Forbidden = 403,
            k404NotFound = 404,
            k500internalServerError = 500
        };
        HttpResponse(bool close_connection);
        ~HttpResponse();

        void SetStatusCode(HttpStatusCode status_code); // 设置回应码
        void SetStatusMessage(const std::string &status_message);//回应马对应的状态
        void SetCloseConnection(bool close_connection);//是否关闭连接

        void SetContentType(const std::string &content_type); //设置响应体的格式html/json/imag/text等等
        void AddHeader(const std::string &key, const std::string &value); // 设置回应头

        void SetBody(const std::string &body);//设置响应体

        std::string message(); // 将信息加入到buffer中。

        bool IsCloseConnection();//是否关闭连接

    private:
        // static const std::string server_name_;

        //响应头对应的键值对
        std::map<std::string, std::string> headers_;

        HttpStatusCode status_code_; //相应回的状态码
        std::string status_message_; //状态码对应的状态
        std::string body_;  //相应体
        bool close_connection_;  //是否在响应完成就关闭连接
};