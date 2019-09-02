#include "echo/echo.h"

#include "../include/muduo/base/Logging.h"
#include "../include/muduo/net/EventLoop.h"

#include <unistd.h>

int main(int argc, char** argv) {
	LOG_INFO << "pid = " << getpid();
	muduo::net::EventLoop loop;

	EchoServer echoServer(&loop, muduo::net::InetAddress(2007));
	echoServer.start();

	//DiscardServer discardServer(&loop, muduo::net::InetAddress(2009));
	//discardServer.start();

	//DaytimeServer daytimeServer(&loop, muduo::net::InetAddress(2013));
	//daytimeServer.start();

	loop.loop();

	return 0;
}
