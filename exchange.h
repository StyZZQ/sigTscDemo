#pragma once
#include <cstdint>
#include <type_traits>

namespace common {

static inline void exchange(const uint8_t *src, uint8_t *dst, std::size_t len)
{
	for (std::size_t i = 0; i < len; i++) {
		dst[i] = src[len - 1 - i];
	}
}

template<typename T>
using Decay = typename std::decay<T>::type;

template<typename T>
static inline typename std::enable_if<std::is_integral<Decay<T>>::value, Decay<T>>::type ntoh(T &&v)
{
	Decay<T> ret = 0;
	exchange((const uint8_t *)&v, (uint8_t *)&ret, sizeof(T));
	return ret;
}

template<std::size_t N>
static inline void ntoh(const uint8_t (&net)[N], uint8_t (&host)[N])
{
	exchange((const uint8_t *)net, (uint8_t *)host, sizeof(host));
}

template<typename T>
static inline typename std::enable_if<std::is_integral<Decay<T>>::value, Decay<T>>::type hton(T &&v)
{
	Decay<T> ret = 0;
	exchange((const uint8_t *)&v, (uint8_t *)&ret, sizeof(T));
	return ret;
}

template<std::size_t N>
static inline void hton(const uint8_t (&host)[N], uint8_t (&net)[N])
{
	exchange((const uint8_t *)host, (uint8_t *)net, sizeof(net));
}

} //end of namespace common