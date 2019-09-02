//#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
//#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
//
//#include "../../include/muduo/base/Logging.h"
//#include "../../include/muduo/net/Buffer.h"
//#include "../../include/muduo/net/Endian.h"
//#include "../../include/muduo/net/TcpConnection.h"
//
//#include <memory>
//
//class LengthHeaderCodec : muduo::noncopyable {
//public:
//	typedef std::function<void(const muduo::net::TcpConnectionPtr&,
//		const muduo::string& message,
//		muduo::Timestamp)> StringMessageCallback;
//
//	explicit LengthHeaderCodec(const StringMessageCallback& cb) : MessageCallback_(cb) {}
//
//	//接收到数据的回调函数
//	void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
//		while (buf->readableBytes() >= kHeaderLen) {
//			//解析：
//			//peek()返回char*指针，然后void*保存
//			//为什么要把void*转换为int32_t*指针？
//			//因为要取4个字节的头部长度，要跟kHeaderLen对应，当转换为int32_t指针后，*取值的话就取4个字节的内容了
//			const void* data = buf->peek();
//			int32_t be32 = *static_cast<const int32_t*>(data);
//			const int32_t len = muduo::net::sockets::networkToHost32(be32);
//			if (len > 65535 || len < 0) {
//				LOG_ERROR << "Invalid length " << len;
//				conn->shutdown();
//				break;
//			} else if (buf->readableBytes() >= len + kHeaderLen) {
//				buf->retrieve(kHeaderLen);
//				muduo::string message(buf->peek(), len);
//				MessageCallback_(conn, message, receiveTime);
//				buf->retrieve(len);
//			} else {
//				break;
//			}
//		}
//	}
//
//	//发送数据
//	//void send(muduo::net::TcpConnection* conn, const muduo::StringPiece& message) {
//	//void send(std::shared_ptr<muduo::net::TcpConnection> conn, const muduo::StringPiece& message) {
//	void send(muduo::net::TcpConnectionPtr conn, const muduo::StringPiece& message) {
//		muduo::net::Buffer buf;
//		buf.append(message.data(), message.size());
//		int32_t len = static_cast<int32_t>(message.size());
//		int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
//		buf.prepend(&be32, sizeof be32);
//		conn->send(&buf);
//	}
//
//private:
//	StringMessageCallback MessageCallback_;
//	const static size_t kHeaderLen = sizeof(int32_t);   //消息=长度+内容，这里kHeaderLen为头部长度4
//};
//
//#endif