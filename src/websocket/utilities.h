/*
 * Copyright (c) 2014, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>
#include <algorithm>
#include <locale>

//#include <netinet/in.h>
#include <stdint.h>

namespace net {

	inline bool is_little_endian() {
		short int val = 0x1;
		char *ptr = reinterpret_cast<char *>(&val);
		return (ptr[0] == 1);
	}

#define TYP_INIT 0
#define TYP_SMLE 1
#define TYP_BIGE 2

	/// Convert 64 bit value to network byte order
	/**
	 * This method is prefixed to avoid conflicts with operating system level
	 * macros for this functionality.
	 *
	 * TODO: figure out if it would be beneficial to use operating system level
	 * macros for this.
	 *
	 * @param src The integer in host byte order
	 * @return src converted to network byte order
	 */
	inline uint64_t _htonll(uint64_t src) {
		static int typ = TYP_INIT;
		unsigned char c;
		union {
			uint64_t ull;
			unsigned char c[8];
		} x;
		if (typ == TYP_INIT) {
			x.ull = 0x01;
			typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE;
		}
		if (typ == TYP_BIGE)
			return src;
		x.ull = src;
		c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
		c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
		c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
		c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;
		return x.ull;
	}

	/// Convert 64 bit value to host byte order
	/**
	 * This method is prefixed to avoid conflicts with operating system level
	 * macros for this functionality.
	 *
	 * TODO: figure out if it would be beneficial to use operating system level
	 * macros for this.
	 *
	 * @param src The integer in network byte order
	 * @return src converted to host byte order
	 */
	inline uint64_t _ntohll(uint64_t src) {
		return _htonll(src);
	}

} // net

namespace utility {

	/// Helper functor for case insensitive find
	/**
	 * Based on code from
	 * http://stackoverflow.com/questions/3152241/case-insensitive-stdstring-find
	 *
	 * templated version of my_equal so it could work with both char and wchar_t
	 */
	template<typename charT>
	struct my_equal {
		/// Construct the functor with the given locale
		/**
		 * @param [in] loc The locale to use for determining the case of values
		 */
		my_equal(std::locale const &loc) : m_loc(loc) {}

		/// Perform a case insensitive comparison
		/**
		 * @param ch1 The first value to compare
		 * @param ch2 The second value to compare
		 * @return Whether or not the two values are equal when both are converted
		 *         to uppercase using the given locale.
		 */
		bool operator()(charT ch1, charT ch2) {
			return std::toupper(ch1, m_loc) == std::toupper(ch2, m_loc);
		}
	private:
		std::locale const &m_loc;
	};

	/// Helper less than functor for case insensitive find
	/**
	 * Based on code from
	 * http://stackoverflow.com/questions/3152241/case-insensitive-stdstring-find
	 */
	struct ci_less {
		// case-independent (ci) compare_less binary function
		struct nocase_compare {
			bool operator() (unsigned char const &c1, unsigned char const &c2) const {
				return tolower(c1) < tolower(c2);
			}
		};
		bool operator() (std::string const &s1, std::string const &s2) const {
			return std::lexicographical_compare
			(s1.begin(), s1.end(),   // source range
			 s2.begin(), s2.end(),   // dest range
			 nocase_compare());  // comparison
		}
	};

	/// Find substring (case insensitive)
	/**
	 * @param [in] haystack The string to search in
	 * @param [in] needle The string to search for
	 * @param [in] loc The locale to use for determining the case of values.
	 *             Defaults to the current locale.
	 * @return An iterator to the first element of the first occurrance of needle in
	 *         haystack. If the sequence is not found, the function returns
	 *         haystack.end()
	 */
	template<typename T>
	typename T::const_iterator ci_find_substr(T const &haystack, T const &needle,
											  std::locale const &loc = std::locale()) {
		return std::search(haystack.begin(), haystack.end(),
						   needle.begin(), needle.end(), my_equal<typename T::value_type>(loc));
	}

	/// Find substring (case insensitive)
	/**
	 * @todo Is this still used? This method may not make sense.. should use
	 * iterators or be less generic. As is it is too tightly coupled to std::string
	 *
	 * @param [in] haystack The string to search in
	 * @param [in] needle The string to search for as a char array of values
	 * @param [in] size Length of needle
	 * @param [in] loc The locale to use for determining the case of values.
	 *             Defaults to the current locale.
	 * @return An iterator to the first element of the first occurrance of needle in
	 *         haystack. If the sequence is not found, the function returns
	 *         haystack.end()
	 */
	template<typename T>
	typename T::const_iterator ci_find_substr(T const &haystack,
											  typename T::value_type const *needle, typename T::size_type size,
											  std::locale const &loc = std::locale()) {
		return std::search(haystack.begin(), haystack.end(),
						   needle, needle + size, my_equal<typename T::value_type>(loc));
	}

	/// Convert a string to lowercase
	/**
	 * @param [in] in The string to convert
	 * @return The converted string
	 */
	std::string to_lower(std::string const &in);

	/// Replace all occurrances of a substring with another
	/**
	 * @param [in] subject The string to search in
	 * @param [in] search The string to search for
	 * @param [in] replace The string to replace with
	 * @return A copy of `subject` with all occurances of `search` replaced with
	 *         `replace`
	 */
	std::string string_replace_all(std::string subject, std::string const &search,
								   std::string const &replace);

	/// Convert std::string to ascii printed string of hex digits
	/**
	 * @param [in] input The string to print
	 * @return A copy of `input` converted to the printable representation of the
	 *         hex values of its data.
	 */
	std::string to_hex(std::string const &input);

	/// Convert byte array (uint8_t) to ascii printed string of hex digits
	/**
	 * @param [in] input The byte array to print
	 * @param [in] length The length of input
	 * @return A copy of `input` converted to the printable representation of the
	 *         hex values of its data.
	 */
	std::string to_hex(uint8_t const *input, size_t length);

	/// Convert char array to ascii printed string of hex digits
	/**
	 * @param [in] input The char array to print
	 * @param [in] length The length of input
	 * @return A copy of `input` converted to the printable representation of the
	 *         hex values of its data.
	 */
	std::string to_hex(char const *input, size_t length);

	class noncopyable {
	public:
		noncopyable(const noncopyable &) = delete;
		void operator=(const noncopyable &) = delete;

	protected:
		noncopyable() = default;
		~noncopyable() = default;
	};

	class copyable {
	protected:
		copyable() = default;
		~copyable() = default;
	};

} // namespace utility

#endif