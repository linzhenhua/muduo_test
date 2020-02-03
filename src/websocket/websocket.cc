#include <functional>

#include "websocket.h"
#include "../../include/muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using namespace muduo;
using namespace muduo::net;

namespace websocket {

	WebSocketClient::WebSocketClient(EventLoop *loop,
									 const InetAddress &addr) :
	//m_request(),
	//m_response(),
	m_loop(loop),
	m_client(loop, addr, "WebSocket"),
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
		conn->setTcpNoDelay(true);
	}

	void WebSocketClient::onMessage(const TcpConnectionPtr &conn, 
									Buffer *buf, Timestamp time) {

	}

	//eg:
	/*
	std::string =
	GET /realtime HTTP/1.1\r\n
	Host: 127.0.0.1:9989\r\n
	Connection: Upgrade\r\n
	Pragma: no-cache\r\n
	Cache-Control: no-cache\r\n
	User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n
	Upgrade: websocket\r\n
	Origin: http://xyz.com\r\n
	Sec-WebSocket-Version: 13\r\n
	Accept-Encoding: gzip, deflate, br\r\n
	Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n
	Sec-WebSocket-Key: IqcAWodjyPDJuhGgZwkpKg==\r\n
	Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n
	\r\n
	*/
	size_t WebSocketClient::constructRequestPacket(const std::string &packet) {
		m_request.consume(packet.c_str(), packet.size());
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


