#include <functional>
#include <string>

#include "websocket.h"

namespace websocket {

	void WebSocketClient::onConnection(const muduo::net::TcpConnectionPtr &conn) {

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

	void WebSocketServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {

	}

	void WebSocketServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time) {

	}

	bool WebSocketServer::parseRequestHandshake(std::string requestData) {

	}

	bool WebSocketServer::parseBasicHeader() {

	}
}


