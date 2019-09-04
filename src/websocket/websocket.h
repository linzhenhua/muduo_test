#ifndef _WEBSOCKET_H_
#define _WEBSOCKET_H_

#include <arpa/inet.h>

#include <string>

#include "ws_define.h"
#include "sha1.h"

namespace websocket {
	class websocketImpl {
	public:

	private:
		size_t encode_header(size_t length, opcode code) {
			size_t header_length;

			if (length < 126) {
				header_length = 2;
				msg_header_[1] = static_cast<char>(length);
			} else if (length <= UINT16_MAX) {
				header_length = 4;
				msg_header_[1] = 126;
				*((uint16_t *)& msg_header_[2]) = htons(static_cast<uint16_t>(length));
			} else {
				header_length = 10;
				msg_header_[1] = 127;
				*((uint64_t *)& msg_header_[2]) = htobe64(length);
			}

			int flags = 0;
			msg_header_[0] = (flags & SND_NO_FIN ? 0 : 128);
			if (!(flags & SND_CONTINUATION)) {
				msg_header_[0] |= code;
			}

			return header_length;
		}

		void SHA1(uint8_t *key_src, size_t size, uint8_t *sha1buf) {
			sha1_context ctx;
			init(ctx);
			update(ctx, key_src, size);
			finish(ctx, sha1buf);
		}

		std::string_view sec_ws_key_;

		size_t payload_length_ = 0;
		size_t left_payload_length_ = 0;

		size_t left_header_len_ = 0;
		unsigned int mask_ = 0;
		unsigned char msg_opcode_ = 0;
		unsigned char msg_fin_ = 0;

		char msg_header_[10];
	};
}

#endif
