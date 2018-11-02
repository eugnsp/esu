#pragma once
#include <es_util/algorithm/for_each_pair.hpp>
#include <es_util/type_traits.hpp>
#include <es_util/numeric.hpp>
#include <es_util/tuple.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>

namespace es::util
{
// Checks if the predicate returns true for all adjacent pairs of elements in the range,
// returns true if the predicate returns true for all pairs, false otherwise,
// returns true if the range contains less than two elements.
template<class Forward_iterator, class Predicate>
bool all_of_pairs(Forward_iterator begin, Forward_iterator end, Predicate pred)
{
	if (begin == end)
		return true;

	for (auto next = std::next(begin); next != end; ++begin, ++next)
		if (!pred(*begin, *next))
			return false;

	return true;
}

// Checks if the predicate returns true for at least one adjacent pair of elements
// in the range, returns true if the predicate returns true for at least one pair,
// false otherwise, returns false if the range contains less than two elements.
template<class Forward_iterator, class Predicate>
bool any_of_pairs(Forward_iterator begin, Forward_iterator end, Predicate pred)
{
	if (begin == end)
		return false;

	for (auto next = std::next(begin); next != end; ++begin, ++next)
		if (pred(*begin, *next))
			return true;

	return false;
}

// Checks if the predicate returns true for no adjacent pairs of elements
// in the range, returns true if the predicate returns true for no pairs,
// false otherwise, returns true if the range contains less than two elements.
template<class Forward_iterator, class Predicate>
bool none_of_pairs(Forward_iterator begin, Forward_iterator end, Predicate pred)
{
	if (begin == end)
		return true;

	for (auto next = std::next(begin); next != end; ++begin, ++next)
		if (pred(*begin, *next))
			return false;

	return true;
}

template<class Input_it_begin, class Input_it_end, typename T, class Predicate>
Input_it_begin find(Input_it_begin first, Input_it_end last, const T& value, Predicate pred)
{
	for (; first != last; ++first)
		if (pred(*first, value))
			break;

	return first;
}

template<class Input_it_begin, class Input_it_end, typename T>
Input_it_begin find(Input_it_begin first, Input_it_end last, const T& value)
{
	return es::util::find(first, last, value, std::equal_to<>{});
}

// Returns true if there exists a circular permutation of the elements in the range
// [first1, last1) that makes that range equal to the range [first2, last2), where
// (last2) denotes (first2 + (last1 - first1)), returns true if the first range is empty
template<class Fw_it_begin1, class Fw_it_end1, class Fw_it_begin2, class Predicate>
bool is_circ_permutation(Fw_it_begin1 first1, Fw_it_end1 last1, 
	Fw_it_begin2 first2, Predicate pred)
{
	const auto equal = es::util::find(first1, last1, *first2, pred);
	if (equal == last1)
		return (first1 == last1);

	auto first = equal;
	++first;
	++first2;

	// Check [equal, last1)
	for (; first != last1; ++first, ++first2)
		if (!pred(*first, *first2))
			return false;

	// Check [first1, equal)
	for (; first1 != equal; ++first1, ++first2)
		if (!pred(*first1, *first2))
			return false;

	return true;
}

template<class Fw_it_begin1, class Fw_it_end1, class Fw_it_begin2>
bool is_circ_permutation(Fw_it_begin1 first1, Fw_it_end1 last1,	Fw_it_begin2 first2)
{
	return es::util::is_circ_permutation(first1, last1, first2, std::equal_to<>{});
}

template<typename T>
void sort2(T& x, T&y)
{
	using std::swap;

	if (y < x)
		swap(x, y);
}

// Computes the sum of the elements in the range <[first, last)>
// using the given binary function <op>.
template<typename T = void, typename Input_it, class Bin_op>
auto accumulate_non_empty(Input_it first, Input_it last, Bin_op op)
{
	using Value = es::util::Non_void_t_or<T, typename std::iterator_traits<Input_it>::value_type>;

	assert(first != last);
	Value init = std::move(*first);
	return std::accumulate(++first, last, std::move(init), op);
}

// Computes the sum of the elements in the range <[first, last)>.
template<typename T = void, typename Input_it>
auto accumulate_non_empty(Input_it first, Input_it last)
{
	return es::util::accumulate_non_empty<T>(first, last, std::plus<>{});
}
}