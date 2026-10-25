// Behaviour test for 0x009B54E0 vector<pair<unsigned long, unsigned long> >::_M_range_insert (STLport forward range insert).
#include <cstdio>
#include <cstdlib>
#include <new>

struct Pair {
    unsigned int first;
    unsigned int second;
    Pair(unsigned int a, unsigned int b) : first(a), second(b) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};

template <class T> T* UninitCopy(const T* first, const T* last, T* result)
{
    for (; first != last; ++first, ++result) new (result) T(*first);
    return result;
}
template <class T> T* CopyBackward(const T* first, const T* last, T* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
template <class T> T* Copy(const T* first, const T* last, T* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}

template <class T> struct Vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_range_insert(T* pos, const T* first, const T* last)
    {
        if (first != last) {
            unsigned int n = (unsigned int)(last - first);
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                const unsigned int elems_after = (unsigned int)(_M_finish - pos);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    UninitCopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    CopyBackward<T>(pos, old_finish - n, old_finish);
                    Copy(first, last, pos);
                } else {
                    const T* mid = first + elems_after;
                    UninitCopy(mid, last, _M_finish);
                    _M_finish += n - elems_after;
                    UninitCopy<T>(pos, old_finish, _M_finish);
                    _M_finish += elems_after;
                    Copy(first, mid, pos);
                }
            } else {
                const unsigned int old_size = size();
                const unsigned int len = old_size + (old_size < n ? n : old_size);
                T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
                T* new_finish = UninitCopy<T>(_M_start, pos, new_start);
                new_finish = UninitCopy(first, last, new_finish);
                new_finish = UninitCopy<T>(pos, _M_finish, new_finish);
                if (_M_start) std::free(_M_start);
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    }
};

static Pair P(unsigned int k) { return Pair(k, k * 10); }

static int Check(const Vec<Pair>& v, const unsigned int* want, unsigned int n, unsigned int cap)
{
    if (v.size() != n || (unsigned int)(v._M_end_of_storage - v._M_start) != cap) return 0;
    for (unsigned int i = 0; i < n; ++i)
        if (v._M_start[i].first != want[i] || v._M_start[i].second != want[i] * 10) return 0;
    return 1;
}

static void Make(Vec<Pair>& v, unsigned int n, unsigned int cap)
{
    v._M_start = (Pair*)std::malloc(cap * sizeof(Pair));
    for (unsigned int i = 0; i < n; ++i) new (v._M_start + i) Pair(P(i + 1));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
}

int main()
{
    Pair src[3] = { P(7), P(8), P(9) };
    int ok = 1;
    Vec<Pair> v;
    Make(v, 4, 8);                                       // in place, elems_after 3 > n 2
    v._M_range_insert(v._M_start + 1, src, src + 2);
    { unsigned int w[] = { 1, 7, 8, 2, 3, 4 }; ok &= Check(v, w, 6, 8); }
    std::free(v._M_start);
    Make(v, 4, 8);                                       // in place, elems_after 1 <= n 3
    v._M_range_insert(v._M_start + 3, src, src + 3);
    { unsigned int w[] = { 1, 2, 3, 7, 8, 9, 4 }; ok &= Check(v, w, 7, 8); }
    std::free(v._M_start);
    Make(v, 2, 3);                                       // reallocate: 2 + max(2, 3) = 5
    v._M_range_insert(v._M_start + 1, src, src + 3);
    { unsigned int w[] = { 1, 7, 8, 9, 2 }; ok &= Check(v, w, 5, 5); }
    v._M_range_insert(v._M_start, src, src);             // empty range: untouched
    { unsigned int w[] = { 1, 7, 8, 9, 2 }; ok &= Check(v, w, 5, 5); }
    std::free(v._M_start);
    if (!ok) { std::printf("009b54e0_TEST FAIL\n"); return 1; }
    std::printf("009b54e0_TEST PASS\n");
    return 0;
}