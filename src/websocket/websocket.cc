#include <functional>
#include <functional>

#include "websocket.h"
#include "../../include/muduo/base/Logging.h"


namespace websocket {

	WebSocketClient::WebSocketClient(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
		: m_response()
		, m_loop(loop)
		, m_client(loop, addr, "WebSocketClient") {
		using namespace std::placeholders;
		//���ûص�����
		m_client.setConnectionCallback(std::bind(&WebSocketClient::onConnection, this, _1));
		m_client.setMessageCallback(std::bind(&WebSocketClient::onMessage, this, _1, _2, _3));
	}

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

	WebSocketServer::WebSocketServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
		: m_isRequestHandshake(false)
		//, m_response()  //��һ�м�Ҳ�У�����Ҳ��
		, m_loop(loop) 
		, m_server(loop, addr, "WebSocketServer") {
		using namespace std::placeholders;
		//���ûص�����
		m_server.setConnectionCallback(std::bind(&WebSocketServer::onConnection, this, _1));
		m_server.setMessageCallback(std::bind(&WebSocketServer::onMessage, this, _1, _2, _3));
	}

	void WebSocketServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
		//��������ʱ��ֻ�Ǽ򵥵Ĵ�ӡ�˿���Ϣ
		LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");
		conn->setTcpNoDelay(true);
	}

	void WebSocketServer::onMessage(const muduo::net::TcpConnectionPtr &conn, 
									muduo::net::Buffer *buf, muduo::Timestamp time) {
		if (buf->findCRLF() != nullptr) {
			buf->retrieveAll();
		}

		//���ְ��ְ����ԣ�\r\n
		//��Ҫ�ж������ְ�����websokcet��
		if () {
			m_isRequestHandshake = true;
		} else {

		}
	}

	void WebSocketServer::start() {
		LOG_INFO << "WebSocketServer start.";
		m_server.start();
	}

	bool WebSocketServer::parseRequestHandshake(std::string requestData) {

	}

	size_t WebSocketServer::constructResponsePacket(const std::string &packet) {

	}

	bool WebSocketServer::sendResponseHandshake() {

	}

	bool WebSocketServer::constructWebSocketPacket() {

	}

	bool WebSocketServer::parseWebSocketPacket() {

	}
}


