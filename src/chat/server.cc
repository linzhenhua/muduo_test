//#include "codec.h"
//
//#include "../../include/muduo/base/Logging.h"
//#include "../../include/muduo/base/Mutex.h"
//#include "../../include/muduo/net/EventLoop.h"
//#include "../../include/muduo/net/TcpServer.h"
//
//#include <set>
//#include <stdio.h>
//#include <unistd.h>
//
//using namespace muduo;
//using namespace muduo::net;
//
//class ChatServer : noncopyable {
//public:
//	ChatServer(EventLoop* loop, const InetAddress& listenAddr) :
//		server_(loop, listenAddr, "ChatServer"),
//		codec_(std::bind(&ChatServer::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)) {
//		server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
//		server_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, this, &codec_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//	}
//
//	void start() {
//		server_.start();
//	}
//
//private:
//	void onConnection(const TcpConnectionPtr& conn) {
//		LOG_INFO << conn->localAddress().toIpPort() << " -> "
//			<< conn->peerAddress().toIpPort() << " is "
//			<< (conn->connected() ? "UP" : "DOWN");
//
//		if (conn->connected()) {
//			connections_.insert(conn);
//		} else {
//			connections_.erase(conn);
//		}
//	}
//
//	void onStringMessage(const TcpConnectionPtr&, const string& message, Timestamp) {
//		for (auto i : connections_) {
//			codec_.send(get_pointer(*i), message);
//		}
//	}
//
//	typedef std::set<TcpConnectionPtr> ConnectionList;
//	TcpServer server_;
//	LengthHeaderCodec codec_;
//	ConnectionList connections_;
//};
//
//int main(int argc, char* argv[]) {
//	LOG_INFO << "pid = " << getpid();
//	if (argc > 1) {
//		EventLoop loop;
//		uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
//		InetAddress serverAddr(port);
//		ChatServer server(&loop, serverAddr);
//		server.start();
//		loop.loop();
//	} else {
//		printf("Usage: %s port\n", argv[0]);
//	}
//}