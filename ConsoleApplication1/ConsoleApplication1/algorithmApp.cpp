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

int binary_search_helper(const  vector<int>& v, size_t begin, size_t end, int key)
{
	assert(std::is_sorted(v.begin(), v.end()));

	if (begin < end) {
		// [b, e) = [b, m) U [m] [m + 1, e)
		size_t m = (begin + end) / 2;
		assert((m - begin) + (end - m) == (end - begin));
		if (key < v[m]) {
			return binary_search_helper(v, begin, m, key);
		}
		else if (v[m] < key) {
			return binary_search_helper(v, m + 1, end, key);
		}
		else {
			return m;
		}
	}
	return -1;
}

int binary_search_helper_limited_rec(const  vector<int>& v, size_t begin, size_t end, int key, size_t depth = 0)
{
	assert(depth < 1000);
	assert(std::is_sorted(v.begin(), v.end()));
	if (begin < end) {
		// [b, e) = [b, m) U [m] [m + 1, e)
		size_t m = (begin + end) / 2;
		assert((m - begin) + (end - m) == (end - begin));
		if (key < v[m]) {
			return binary_search_helper_limited_rec(v, begin, m, key, depth + 1);
		}
		else if (v[m] < key) {
			return binary_search_helper_limited_rec(v, m + 1, end, key, depth + 1);
		}
		else {
			return m;
		}
	}
	return -1;
}

int binary_search_loop(const  vector<int>& v, int key)
{
	assert(std::is_sorted(v.begin(), v.end()));
	size_t begin = 0;
	size_t end = v.size();

	if (begin == end) return -1;

	while (begin < end) {
		//[b, e) = [b, m) U [m] [m + 1, e)
		//size_t m = (begin + end) / 2;
		//size_t m = begin + (end - begin) / 2;
		size_t m = begin + (end - begin) >> 1;
		if (key < v[m]) {
			end = m;
		}
		else if (v[m] < key) {
			begin = m + 1;
		}
		else {
			return m;
		}
	}
	return -1;
}

template <class TIter, class T>
TIter my_lower_bound(TIter b, TIter e, const T& k)
{
	assert(b <= e && std::is_sorted(b, e));
	while (b < e) {
		TIter m = b + (e - b) / 2;
		if (*m < k) {
			b = m + 1;
		}
		else e = m;
	}
	return b;
}

template <class TIter, class T>
TIter binary_search_lower_bound1(TIter b,TIter e,const T& key) {
	auto lb = my_lower_bound(b, e, key);
	return b != e && !(key<*lb)?lb:e;
}

template <class TInt>
TInt binary_search_lower_bound(const vector<TInt>& v, TInt k) {
	auto b = v.begin();
	auto e = v.end();
	auto m = my_lower_bound(b, e, k);
	if (!(k < *m)) return *m;
	return *e;
}

void test_search() {

	typedef vector<int> Array;
	auto search = [](const vector<int>& v, int key) {
		auto r = binary_search_lower_bound1(v.begin(), v.end(), key);
		return r != v.end() ? r - v.begin() : -1;
	};

	//auto search = search_2;

	auto key = 8;
	// key not exists in array
	test(-1, search, Array(), key); // degenarated
	//test(-1, search, Array({ key - 1 }), key); // trivial
	test(-1, search, Array({ key - 1, key + 1 }), key); // trivial2
	test(-1, search, Array({ 1, 2, 3, 4, 5, 7,10 }), key); // general
	test(-1, search, Array({ 7 ,9, 10, 11, 12 }), key); // general
	test(-1, search, Array({ 4, 1, 2, 7, 10 }), key); // general
	// key exists in array
	// non appliable // degenarated
	test(0, search, Array({ key }), key); // trivial
	test(0, search, Array({ key, key + 1 }), key); // trivial2
	test(1, search, Array({ key - 1, key }), key); // trivial2
	test(8, search, Array({ 0, 1, 2, 3, 4, 5, 6, 7, key }), key); // general
	test(0, search, Array({ key, 9, 10, 11, 12 }), key); // general
	test(2, search, Array({ 4, 1, key, 7, 10 }), key); // general                

	test(0, search, Array({ 1, 7, key, 10 }), key); // general                
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
	test(4, search, Array({ 1, 2, 3, key, key }), 0, 5, key);
	test(1, search, Array({ key, key, key + 1 }), 0, 3, key);
	test(1, search, Array({ key, key, key + 1, key + 10 }), 0, 4, key);
}

template<class TIter>
TIter min_element1(TIter b, TIter e) {
	//[) = [processed)[current][unprocessed)
	//result is min from [processed)
	auto result = b;
	while (b < e) {
		if (*b < *result) {
			result = b;
		}
		++b;
	}
	return result;
}



int main(int argc, char const *argv[])
{
	//test_search();
	//typedef std::vector<int> Array;
	//test_binary_search();

	const vector<int> v({ 1 ,2, 3, 4, 5, 6, 8, 9 });
	cout << *min_element1(v.begin(), v.end());
	//cout << binary_search_lower_bound(v, 8);



	return 0;
}