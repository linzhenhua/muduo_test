#ifndef MUDUO_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H
#define MUDUO_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H

#include "../include/muduo/net/TcpServer.h"

class ChargenServer {
public:
    ChargenServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, bool print = false);

    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);
    
	void onWriteComplete(const muduo::net::TcpConnectionPtr& conn);
	void printThroughput();

    muduo::net::TcpServer server_;
    muduo::string message_;
    int64_t transferred_;
    muduo::Timestamp startTime_;
};

#endif
