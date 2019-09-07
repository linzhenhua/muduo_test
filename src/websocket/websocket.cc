#include <functional>
#include <string>

#include "websocket.h"

namespace websocket {

	size_t WebSocketClient::constructRequestPacket(const std::string &packet) {
		m_request.consume(packet.c_str(), packet.length());
	}

}

