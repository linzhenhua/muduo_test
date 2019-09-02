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
//	//���յ����ݵĻص�����
//	void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
//		while (buf->readableBytes() >= kHeaderLen) {
//			//������
//			//peek()����char*ָ�룬Ȼ��void*����
//			//ΪʲôҪ��void*ת��Ϊint32_t*ָ�룿
//			//��ΪҪȡ4���ֽڵ�ͷ�����ȣ�Ҫ��kHeaderLen��Ӧ����ת��Ϊint32_tָ���*ȡֵ�Ļ���ȡ4���ֽڵ�������
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
//	//��������
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
//	const static size_t kHeaderLen = sizeof(int32_t);   //��Ϣ=����+���ݣ�����kHeaderLenΪͷ������4
//};
//
//#endif