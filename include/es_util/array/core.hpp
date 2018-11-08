#pragma once
#include <es_util/type_traits.hpp>
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace es_util
{
namespace internal
{
template<class Array>
struct Array_size;

template<typename T, std::size_t size>
struct Array_size<std::array<T, size>>
{
	static constexpr auto value = size;
};
} // namespace internal

template<class Array>
inline constexpr auto array_size = internal::Array_size<es_util::Remove_cvref<Array>>::value;
} // namespace es_util