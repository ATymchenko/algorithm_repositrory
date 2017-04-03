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
TIter binary_search_lower_bound1(TIter b, TIter e, const T& key) {
	auto lb = my_lower_bound(b, e, key);
	return b != e && !(key < *lb) ? lb : e;
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
		return result;
	}
}

template<class TIter>
void naive_sort(TIter b, TIter e) {
	for (auto i = b; i < e; ++i) {
		assert(std::is_sorted(b, i));
		//[sorted) U [unsorted) = [b, i) U [i e)
		for (auto j = i + 1; j < e; ++j) {
			//[unsorted) = [i] U [i+1,j) U [j e)
			assert(std::min_element(i, j) == i);
			if (*j < *i) {
				swap(*i, *j);
			}
		}
		assert(std::is_sorted(b, i + 1));
	}
}

template<class TIter>
void selection_sort1(TIter b, TIter e) {
	for (auto i = b; i < e; ++i) {
		assert(std::is_sorted(b, i));
		//[sorted) U [unsorted) = [b, i) U [i e)
		swap(*i, *min_element1(i, e));
		assert(std::is_sorted(b, i + 1));
	}
}



template<class TIter>
void bubble_sort1(TIter b, TIter e) {
	if (b == e) {
		return;
	}
	TIter sorted_begin = e - 1;
	while (b < sorted_begin) {
		assert(sorted_begin < e);
		assert(std::is_sorted(sorted_begin, e));
		auto j = b;
		while (j < sorted_begin) {
			//assert(std::reverse_max_element(b, j + 1) == j);
			if (*(j + 1) < *j) {
				std::iter_swap(j + 1, j);
			}
			++j;
			//assert(std::reverse_max_element(b, j + 1) == j);
		}
		--sorted_begin;
		assert(std::is_sorted(sorted_begin, e));
	}
}

void test_select_sort() {
	typedef vector<int> Array;
	auto sort = [](const vector<int>& v) {
		auto u = v;
		selection_sort1(u.begin(), u.end());
		return u;
	};

	test(Array(), sort, Array());//degenerate
	test(Array({ 1 }), sort, Array({ 1 }));//trivial
	test(Array({ 1,2 }), sort, Array({ 1,2 }));
	test(Array({ 1,2 }), sort, Array({ 2,1 }));
	test(Array({ 1,1 }), sort, Array({ 1,1 }));

	test(Array({ 1,1,1 }), sort, Array({ 1,1,1 }));
	test(Array({ 1,2,3 }), sort, Array({ 1,2,3 }));
	test(Array({ 1,2,3 }), sort, Array({ 3,2,1 }));
	test(Array({ 1,2,3 }), sort, Array({ 2,3,1 }));

	test(Array({ 0,1,5,5,6,7,8 }), sort, Array({ 8,5,1,7,6,0,5 }));
}

void test_bubble_sort() {
	typedef vector<int> Array;
	auto sort = [](const vector<int>& v) {
		auto u = v;
		bubble_sort1(u.begin(), u.end());
		return u;
	};

	test(Array(), sort, Array());//degenerate
	test(Array({ 1 }), sort, Array({ 1 }));//trivial
	test(Array({ 1,2 }), sort, Array({ 1,2 }));
	test(Array({ 1,2 }), sort, Array({ 2,1 }));
	test(Array({ 1,1 }), sort, Array({ 1,1 }));

	test(Array({ 1,1,1 }), sort, Array({ 1,1,1 }));
	test(Array({ 1,2,3 }), sort, Array({ 1,2,3 }));
	test(Array({ 1,2,3 }), sort, Array({ 3,2,1 }));
	test(Array({ 1,2,3 }), sort, Array({ 2,3,1 }));

	test(Array({ 0,1,5,5,6,7,8 }), sort, Array({ 8,5,1,7,6,0,5 }));
}

template <class TIter>
void selection_sort(TIter b, TIter e) {
	for (auto i = b; i < e; ++i) {
		// [sorted) U [unsorted) = [b, i) U [i e)
		assert(is_sorted(b, i));
		swap(*i, *min_element(i, e));
	}
}

template <class TIter>
void insertion_sort(TIter b, TIter e) {

	auto pivot = b + 1;
	while (pivot < e) {
		// [b, pivot) [pivot) [pivot+1, e)
		assert(std::is_sorted(b, pivot));

		auto i = pivot;
		while (b < i && *i < *(i - 1)) {
			assert(is_sorted(b, i) && is_sorted(i, pivot));
			std::iter_swap(i, i - 1);
			--i;
			assert(is_sorted(b, i) && is_sorted(i, pivot));
		}

		++pivot;
		assert(std::is_sorted(b, pivot));
	}

}


template <class TIter>
void merge(TIter b, TIter m, TIter e, TIter buff) {
	const auto size = e - b;
	const auto old = buff;
	const auto e1 = m;

	while (b < e1 && m < e) {
		*buff++ = *b < *m ? *b++ : *m++;
	}
	buff = copy(b, e1, buff);
	buff = copy(m, e, buff);

	assert(buff - old == size);
}

template <class TIter>
void merge_sort(TIter b, TIter e, TIter buff) {
	auto size = e - b;
	if (size > 1) {
		auto m = b + size / 2;
		merge_sort(b, m, buff);
		merge_sort(m, e, buff + (size) / 2);
		std::copy(buff, buff + size, b);
		merge(b, m, e, buff);
	}
	else {
		std::copy(b, e, buff);
	}
}

template <class TIter>
void merge_sort2(TIter b, TIter e) {
	std::vector<int> buff(e - b);
	merge_sort(b, e, buff.begin());
	copy(buff.begin(), buff.end(), b);
}

/*
template <class TIter>
void quick_sort(TIter b, TIter e) {
	if (e - b < 2) {
		return;
	}
	auto p = select_pivot(b, e);
	partition(b, p, e);
	//assert(*max(b, p) <= *p && p < *min(p + 1, e));
	quick_sort(b, p);
	quick_sort(p + 1, e);
}*/
template <class TIter>
TIter partition1(TIter b, TIter p, TIter e) {
	assert(b < e);
	auto pivot = *p;
	swap(*(e - 1), *p);
	//ub - unprocessed begin
	auto ub = b;
	auto ue = e - 1;
	//[b, ub)[ub, ue}[ue,e)
	//[<p)[)[p<=)
	while (ub < ue) {
		if (*ub < pivot) {
			++ub;
		}
		else {
			--ue;
			swap(*ub, *ue);
		}
	}
	swap(*ub, *(e - 1));
	return ub;
}

template <class TIter>
TIter partition2(TIter b, TIter p, TIter e) {
	assert(b < e);
	auto pivot = *p;
	iter_swap(e - 1, p);
	auto b2 = b;
	auto e2 = b;
	// [b,b2)[b2,e2)[e2,e)
	//[<p] [p<=][unpr)
	while (e2 < e - 1) {
		if (*e2 < pivot) {
			swap(*b2, *e2);
			++b2;
		}
	}
	++e2;
	iter_swap(e - 1, b2);
	return b2;
}


template<class TIter>
TIter pivot_strategy(TIter b, TIter e) {
	assert(b < e);
	auto m = b + (e - b) / 2;
	auto last = e - 1;
	if (*m < *b) swap(b, m);
	if (*last < *m) swap(last, m);
	if (*m < *b) swap(b, m);
	return m;
}
template <class TIter>
void quick_sort1(TIter b, TIter e) {
	if (e - b < 1) {
		auto p = partition2(b, pivot_strategy(b,e), e);
		//assert(*max(b, p) <= *p && p < *min(p + 1, e));
		quick_sort1(b, p);
		quick_sort1(p + 1, e);
	}
}

template <class TSorter>
void test_sorting(TSorter my_sort) {

	typedef vector<int> Array;
	auto sort = [my_sort](vector<int> u) {
		my_sort(u.begin(), u.end());
		return u;
	};

	//test(Array(), sort, Array());       // degerate
	//test(Array({ 1 }), sort, Array({ 1 })); // trivial
	//test(Array({ 1, 2 }), sort, Array({ 1, 2 })); // trivial 2nd
	//test(Array({ 1, 2 }), sort, Array({ 2, 1 })); // trivial 2nd
	//test(Array({ 1, 1 }), sort, Array({ 1, 1 })); // trivial 2nd

	test(Array({ 1, 1, 1 }), sort, Array({ 1, 1, 1 }));
	test(Array({ 1, 2, 3 }), sort, Array({ 1, 2, 3 }));
	test(Array({ 1, 2, 3 }), sort, Array({ 3, 2, 1 }));
	test(Array({ 1, 2, 3 }), sort, Array({ 2, 3, 1 }));

	test(Array({ 0, 1, 5, 5, 6, 7, 8 }), sort, Array({ 8, 5, 1, 7, 6, 0, 5 }));

}

int main(int argc, char const *argv[])
{
	//test_search();
	//typedef std::vector<int> Array;
	//test_binary_search();
	//cout << *min_element1(v.begin(), v.end());
	//cout << binary_search_lower_bound(v, 8);
	//test_select_sort();
	//test_bubble_sort();
	typedef std::vector<int>::iterator Iter;
	/*test_sorting(naive_sort<Iter>);
	test_sorting(selection_sort<Iter>);
	test_sorting(bubble_sort1<Iter>);
	test_sorting(insertion_sort<Iter>);
	test_sorting(merge_sort2<Iter>);*/
	test_sorting(quick_sort1<Iter>);


	return 0;
}