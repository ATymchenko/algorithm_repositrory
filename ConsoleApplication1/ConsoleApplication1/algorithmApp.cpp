#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

template <class T>
ostream& operator<<(ostream& o, const vector<T>& v) {
	o << "[";
	for (auto& x : v) {
		o << x << ", ";
	}
	return o << "]";
}

template <class TFunc, class TResult, class TParam1>
void test(TResult expect, TFunc f, TParam1 p1) {
	auto got = f(p1);
	if (got != expect) {
		cerr << "failed: " << expect << " != " << got << endl;
	}
}

template <class TFunc, class TResult, class TParam1, class TParam2>
void test(TResult expect, TFunc f, TParam1 p1, TParam2 p2) {
	auto got = f(p1, p2);
	if (got != expect) {
		cerr << "failed: f(" << p1 << ", " << p2 << ") = "
			<< got << " != " << expect << endl;
	}
}

template <class TFunc, class TResult, class TParam1, class TParam2, class TParam3, class TParam4>
void test(TResult expect, TFunc f, TParam1 p1, TParam2 p2, TParam3 p3, TParam4 p4) {
	auto got = f(p1, p2, p3, p4);
	if (got != expect) {
		cerr << "failed: f(" << p1 << ", " << p2 << ") = "
			<< got << " != " << expect << endl;
	}
}


int search_0(int v[], size_t size, int key) {
	for (int i = 0; i < size; ++i) {
		if (v[i] == key) {
			return i;
		}
	}

	return -1;
}

int search_1(int v[], size_t size, int key) {
	v[size] = key;

	int i = 0;
	while (v[i] != key) { ++i; }

	if (i != size) {
		return i;
	}

	return -1;
}

int search_2(const std::vector<int>& v, int key) {
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] == key) {
			return i;
		}
	}

	return -1;
}

int search_3(std::vector<int>& v, int key) {
	v.push_back(key);
	int i = 0;
	while (v[i] != key) {
		++i;
	}
	v.pop_back();

	if (i != v.size()) {
		return i;
	}

	return -1;
}

int binary_search_helper
(
const  vector<int>& v,
size_t begin,
size_t end,
int key
)
{
	assert(std::is_sorted(v.begin(), v.end()));

	if (begin == end) return -1;
	if (end - begin == 1) {
		if (v[begin] == key)
			return begin;
		else
			return -1;
	}

	// [b, e) = [b, m) U [m, e)
	size_t m = (begin + end) / 2;
	assert((m - begin) + (end - m) == (end - begin));
	if (key < v[m]) {
		return binary_search_helper(v, begin, m, key);
	}
	else if (v[m] < key) {
		return binary_search_helper(v, m, end, key);
	}
	else {
		return m;
	}
}

void test_search() {

	typedef vector<int> Array;

	auto search = search_2;

	auto key = 8;
	// key not exists in array
	test(-1, search, Array(), key); // degerate
	test(-1, search, Array({ key - 1 }), key); // trivial
	test(-1, search, Array({ key - 1, key + 1 }), key); // trivial2
	test(-1, search, Array({ 1, 2, 3, 4, 5, 7 }), key); // general
	test(-1, search, Array({ 9, 10, 11, 12 }), key); // general
	test(-1, search, Array({ 4, 1, 2, 7, 10 }), key); // general
	// key exists in array
	// non appliable // degerate
	test(0, search, Array({ key }), key); // trivial
	test(0, search, Array({ key, key + 1 }), key); // trivial2
	test(1, search, Array({ key - 1, key }), key); // trivial2
	test(8, search, Array({ 0, 1, 2, 3, 4, 5, 6, 7, key }), key); // general
	test(0, search, Array({ key, 9, 10, 11, 12 }), key); // general
	test(2, search, Array({ 4, 1, key, 7, 10 }), key); // general                

	test(0, search, Array({ key, 1, key, 7, 10 }), key); // general                
	test(2, search, Array({ 2, 1, key, 7, key }), key); // general                
}


void test_binary_search()
{
	auto search = binary_search_helper;
	typedef vector<int> Array;
	auto key = 8;
	// degenarated
	test(-1, search, Array(), 0, 0, key);
	// trivial 
	test(-1, search, Array({ 1 }), 0, 1, key);
	test(-1, search, Array({ 100 }), 0, 1, key);
	test(0, search, Array({ 8 }), 0, 1, key);
	// and 2nd trivial
	test(-1, search, Array({ 1, 2 }), 0, 2, key);
	test(1, search, Array({ 1, 8 }), 0, 2, key);
	test(0, search, Array({ 8, 100 }), 0, 2, key);
	// key not in array
	test(-1, search, Array({ 1, 2, 3, 5, 41 }), 0, 5, key);
	test(-1, search, Array({ 43, 45, 67, 100 }), 0, 4, key);
	test(-1, search, Array({ 3, 5, 41, 43, 45, 67 }), 0, 6, key);
	// key in array
	test(3, search, Array({ 1, 2, 5, 8 }), 0, 4, key);
	test(0, search, Array({ 8, 45, 67, 100 }), 0, 4, key);
	test(3, search, Array({ 3, 5, 7, 8, 45, 67 }), 0, 6, key);
	// binary search specific:
	test(3, search, Array({ 3, 5, 7, 8, 45, 67 }), 0, 6, key);
	test(2, search, Array({ 3, 5, 8, 45, 67 }), 0, 5, key);
	//more tham one key
	test(1, search, Array({ key, key }), 0, 2, key);
	test(2, search, Array({ 1, 2, key, key }), 0, 4, key);
	test(3, search, Array({ 1, 2, 3, key, key }), 0, 5, key);
	test(1, search, Array({ key, key, key + 1 }), 0, 3, key);
	test(1, search, Array({ key, key, key + 1, key + 10 }), 0, 4, key);
}

int main(int argc, char const *argv[])
{
	test_search();
	typedef std::vector<int> Array;
	test_binary_search();
	return 0;
}