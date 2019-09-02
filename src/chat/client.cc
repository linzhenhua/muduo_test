//#include "codec.h"
//
//#include "../../include/muduo/base/Logging.h"
//#include "../../include/muduo/base/Mutex.h"
//#include "../../include/muduo/net/EventLoopThread.h"
//#include "../../include/muduo/net/TcpClient.h"
//
//#include <iostream>
//#include <functional>
//
//#include <stdio.h>
//#include <unistd.h>
//
//using namespace muduo;
//using namespace muduo::net;
//
//class ChatClient : noncopyable {
//public:
//	ChatClient(EventLoop* loop, const InetAddress& serverAddr)
//		: client_(loop, serverAddr, "ChatClient"),
//		codec_(std::bind(&ChatClient::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)) {
//		client_.setConnectionCallback(
//			std::bind(&ChatClient::onConnection, this, std::placeholders::_1));
//		client_.setMessageCallback(
//			std::bind(&LengthHeaderCodec::onMessage, &codec_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//		client_.enableRetry();
//	}
//
//	void connect() {
//		client_.connect();
//	}
//
//	void disconnect() {
//		client_.disconnect();
//	}
//
//	void write(const StringPiece& message) {
//		MutexLockGuard lock(mutex_);
//		if (connection_) {
//			codec_.send(connection_, message);
//		}
//	}
//
//private:
//	void onConnection(const TcpConnectionPtr& conn) {
//		LOG_INFO << conn->localAddress().toIpPort() << " -> "
//			<< conn->peerAddress().toIpPort() << " is "
//			<< (conn->connected() ? "UP" : "DOWN");
//
//		MutexLockGuard lock(mutex_);
//		if (conn->connected()) {
//			connection_ = conn;
//		} else {
//			connection_.reset();
//		}
//	}
//
//	void onStringMessage(const TcpConnectionPtr&,
//		const string& message,
//		Timestamp) {
//		printf("<<< %s\n", message.c_str());
//	}
//
//	TcpClient client_;
//	LengthHeaderCodec codec_;
//	MutexLock mutex_;
//	TcpConnectionPtr connection_ GUARDED_BY(mutex_);
//};
//
//int main(int argc, char* argv[]) {
//	LOG_INFO << "pid = " << getpid();
//	if (argc > 2) {
//		EventLoopThread loopThread;
//		uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
//		InetAddress serverAddr(argv[1], port);
//
//		ChatClient client(loopThread.startLoop(), serverAddr);
//		client.connect();
//		std::string line;
//		while (std::getline(std::cin, line)) {
//			client.write(line);
//		}
//		client.disconnect();
//		CurrentThread::sleepUsec(1000 * 1000);  // wait for disconnect, see ace/logging/client.cc
//	} else {
//		printf("Usage: %s host_ip port\n", argv[0]);
//	}
//}
//
