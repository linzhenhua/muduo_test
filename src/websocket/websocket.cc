#include <functional>
#include <string>

#include "websocket.h"
#include "../../include/muduo/base/Logging.h"


namespace websocket {

	void WebSocketClient::onConnection(const muduo::net::TcpConnectionPtr &conn) {
		LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");
		conn->setTcpNoDelay(true);
	}

	void WebSocketClient::onMessage(const muduo::net::TcpConnectionPtr &conn, 
									muduo::net::Buffer *buf, muduo::Timestamp time) {

	}

	size_t WebSocketClient::constructRequestPacket(const std::string &packet) {
		m_request.consume(packet.c_str(), packet.length());
	}

	bool WebSocketClient::sendRequestHandshake() {

	}

	bool WebSocketClient::parseReponseHandshake(std::string responseData) {

	}

	bool WebSocketClient::constructWebSocketPacket() {

	}

	bool WebSocketClient::parseWebSocketPacket() {

	}

	void WebSocketClient::start() {

	}

	void WebSocketServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
		LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");
		conn->setTcpNoDelay(true);
	}

	void WebSocketServer::onMessage(const muduo::net::TcpConnectionPtr &conn, 
									muduo::net::Buffer *buf, muduo::Timestamp time) {
		//需要判断是握手包还是websokcet包
		if (true) {
			m_isRequestHandshake = true;


		} else {

		}
	}

	bool WebSocketServer::parseRequestHandshake(std::string requestData) {

	}

	bool WebSocketServer::parseBasicHeader() {

	}

	void WebSocketServer::start() {
		LOG_INFO << "WebSocketServer start.";
		m_server.start();
	}
}


