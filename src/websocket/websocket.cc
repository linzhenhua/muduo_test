#include <stdio.h>

#include <functional>

#include "websocket.h"
#include "http/constants.h"
#include "../../include/muduo/base/Logging.h"
#include "../../include/muduo/net/TcpConnection.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using namespace std::literals;
using namespace muduo;
using namespace muduo::net;

namespace websocket {

	WebSocketClient::WebSocketClient(EventLoop *loop,
									 const InetAddress &addr) :
		//m_request(),
		//m_response(),
		m_loop(loop),
		m_client(loop, addr, "WebSocketClient"),
		m_is_server_support_websocket(false),
		m_state(websocketState::disconnected) {
		//注册回调函数
		m_client.setConnectionCallback(std::bind(&WebSocketClient::onConnection, this, _1));
		m_client.setMessageCallback(std::bind(&WebSocketClient::onMessage, this, _1, _2, _3));
	}

	void WebSocketClient::onConnection(const TcpConnectionPtr &conn) {
		LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");

		if (!conn->connected()) {
			m_loop->quit();
		}

		conn->setTcpNoDelay(true);

		//构造websocket握手包
		std::string_view request_msg =
			"GET /lzh HTTP/1.1\r\n \
			Host: 127.0.0.1 : 9989\r\n \
			Connection : Upgrade\r\n \
			Upgrade : websocket\r\n \
			Origin : http://xyz.com\r\n \
		    Sec - WebSocket - Version : 13\r\n \
			Sec - WebSocket - Key: IqcAWodjyPDJuhGgZwkpKg == \r\n \
			\r\n"sv;
		constructRequestPacket(request_msg);

		//发送websocket握手包
		sendRequestHandshake();

		m_state = websocketState::connecting;
	}

	void WebSocketClient::onMessage(const TcpConnectionPtr &conn,
									Buffer *buf, Timestamp time) {
		if (m_state == websocketState::connecting) {   //这个状态表明HTTP握手阶段
			parseReponseHandshake(buf->retrieveAllAsString());
			return;
		} else if (m_state == websocketState::connected) {  //这个状态表明websocket数据交互
			if (buf->readableBytes() >= frame::BASIC_HEADER_LENGTH) {
				frame::uint16_converter temp16;
				temp16.i = buf->readInt16();
				m_base_header.b0 = temp16.c[0];
				m_base_header.b1 = temp16.c[1];

				//解析出payload_len
				uint8_t payload_len = frame::get_basic_size(m_base_header);
			} else {
				return;
			}
			parseWebSocketPacket(buf->retrieveAllAsString());
		} else {
			LOG_TRACE << "what happened?";
		}
	}

	void WebSocketClient::constructRequestPacket(const std::string_view &packet) {
		m_request.consume(packet.data(), packet.size());
	}

	void WebSocketClient::sendRequestHandshake() {
		std::string reqStr(m_request.raw());
		m_client.connection()->send(reqStr.c_str(), reqStr.length());

	}

	void WebSocketClient::parseReponseHandshake(const std::string &handshake_data) {
		m_response.consume(handshake_data.c_str(), handshake_data.length());

		if (m_response.headers_ready()) {  //解析HTTP头部完成
			if (m_response.get_version() == std::string("HTTP/1.1") &&
				m_response.get_status_code() == http::status_code::switching_protocols &&
				m_response.get_status_msg() == http::status_code::get_string(http::status_code::switching_protocols) &&
				!m_response.get_header(std::string("Upgrade")).empty() &&
				!m_response.get_header(std::string("Connection")).empty() &&
				!m_response.get_header(std::string("Sec-WebSocket-Accept")).empty() &&
				m_response.get_header(std::string("Upgrade")) == std::string("websocket") &&
				m_response.get_header(std::string("Connection")) == std::string("Upgrade")) {
				m_is_server_support_websocket = true;
				m_state = websocketState::connected;

				LOG_TRACE << "websocket connected";

				//这里发送websocket数据或者开个线程接收用户输入
			} else {
				LOG_TRACE << "server not support websocket";

				m_client.disconnect();  //主动断开连接
				m_is_server_support_websocket = false;
				m_state = websocketState::disconnected;
				m_loop->quit();

				::exit(0);
			}
		}
	}

	bool WebSocketClient::constructWebSocketPacket() {

	}

	void WebSocketClient::parseWebSocketPacket(const std::string &websocket_packet) {

	}

	void WebSocketClient::start() {
		m_client.connect();
	}

	WebSocketServer::WebSocketServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
		: m_isRequestHandshake(false)
		//, m_response()  //这一行加也行，不加也行
		, m_loop(loop)
		, m_server(loop, addr, "WebSocketServer") {
		using namespace std::placeholders;
		//设置回调函数
		m_server.setConnectionCallback(std::bind(&WebSocketServer::onConnection, this, _1));
		m_server.setMessageCallback(std::bind(&WebSocketServer::onMessage, this, _1, _2, _3));
	}

	void WebSocketServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
		//建立连接时，只是简单的打印端口信息
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

		//握手包分包策略：\r\n
		//需要判断是握手包还是websokcet包
		if (1) {
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


