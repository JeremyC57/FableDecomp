#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;                         // live element objects (copy ctor ++, dtor --)

struct SULongLongPair {
    unsigned int first;
    int second;
    SULongLongPair(int k) : first((unsigned int)k), second(-k) { ++g_live; }
    SULongLongPair(const SULongLongPair& o) : first(o.first), second(o.second) { ++g_live; }
    SULongLongPair& operator=(const SULongLongPair& o) { first = o.first; second = o.second; return *this; }
    ~SULongLongPair() { --g_live; }
};
static SULongLongPair MakeElem(int k) { return SULongLongPair(k); }
static int KeyOf(const SULongLongPair& e) { return (int)e.first; }
static bool CheckElem(const SULongLongPair& e, int k) { return e.second == -k; }

// STLport 4.6 vector<T> growth / insert paths, as the source reconstructs them (element by element, __false_type).
template <class T> struct TVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    TVector(unsigned int cap) : _M_start((T*)malloc(cap * sizeof(T))), _M_finish(_M_start), _M_end_of_storage(_M_start + cap) { }
    ~TVector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); free(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; }
    static T* __uninitialized_copy(const T* first, const T* last, T* result)
    { for (; first != last; ++first, ++result) new (result) T(*first); return result; }
    static T* __uninitialized_fill_n(T* first, unsigned int n, const T& x)
    { for (; n > 0; --n, ++first) new (first) T(x); return first; }
    static T* __copy_backward(T* first, T* last, T* result)
    { while (first != last) *--result = *--last; return result; }
    static void fill(T* first, T* last, const T& x) { for (; first != last; ++first) *first = x; }
    static T* copy(const T* first, const T* last, T* result) { for (; first != last; ++first, ++result) *result = *first; return result; }
    void _M_insert_overflow(T* position, const T& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        T* new_start = (T*)malloc(len * sizeof(T));
        T* new_finish = __uninitialized_copy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) T(x);
            ++new_finish;
        }
        else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x);
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish);
        _M_clear();
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    void _M_fill_insert(T* position, unsigned int n, const T& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                T x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    __uninitialized_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    __copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                }
                else {
                    __uninitialized_fill_n(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    __uninitialized_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            }
            else
                _M_insert_overflow(position, x, n, false);
        }
    }
    void _M_range_insert(T* position, const T* first, const T* last)
    {
        if (first != last) {
            unsigned int n = (unsigned int)(last - first);
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    __uninitialized_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    __copy_backward(position, old_finish - n, old_finish);
                    copy(first, last, position);
                }
                else {
                    const T* mid = first + elems_after;
                    __uninitialized_copy(mid, last, _M_finish);
                    _M_finish += n - elems_after;
                    __uninitialized_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    copy(first, mid, position);
                }
            }
            else {
                const unsigned int old_size = size();
                const unsigned int len = old_size + (old_size > n ? old_size : n);
                T* new_start = (T*)malloc(len * sizeof(T));
                T* new_finish = __uninitialized_copy(_M_start, position, new_start);
                new_finish = __uninitialized_copy(first, last, new_finish);
                new_finish = __uninitialized_copy(position, _M_finish, new_finish);
                _M_clear();
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    }
};

typedef SULongLongPair TElem;

static void Fill(TVector<TElem>& v, int count)
{
    for (int i = 0; i < count; ++i) {
        new (v._M_finish) TElem(MakeElem(i + 1));
        ++v._M_finish;
    }
}

static bool Same(const TVector<TElem>& v, const int* keys, int count)
{
    if ((int)v.size() != count || g_live != count)
        return false;
    for (int i = 0; i < count; ++i)
        if (KeyOf(v._M_start[i]) != keys[i])
            return false;
    return true;
}

static int Fail()
{
    std::printf("00a39610_TEST FAIL\n");
    return 1;
}

int main()
{
    TElem src[3] = { MakeElem(7), MakeElem(8), MakeElem(9) };
    {   // elems_after > n
        TVector<TElem> v(10);
        Fill(v, 5);
        v._M_range_insert(v._M_start + 1, src, src + 2);
        const int k[] = { 1, 7, 8, 2, 3, 4, 5 };
        g_live -= 3;
        if (!Same(v, k, 7)) return Fail();
        g_live += 3;
    }
    {   // elems_after <= n
        TVector<TElem> v(10);
        Fill(v, 3);
        v._M_range_insert(v._M_start + 2, src, src + 3);
        const int k[] = { 1, 2, 7, 8, 9, 3 };
        g_live -= 3;
        if (!Same(v, k, 6)) return Fail();
        g_live += 3;
    }
    {   // no room: reallocate to old + max(old, n)
        TVector<TElem> v(2);
        Fill(v, 2);
        v._M_range_insert(v._M_start + 1, src, src + 3);
        const int k[] = { 1, 7, 8, 9, 2 };
        g_live -= 3;
        if (!Same(v, k, 5) || v._M_end_of_storage - v._M_start != 5) return Fail();
        g_live += 3;
        v._M_range_insert(v._M_start, src, src);           // empty range: no-op
        g_live -= 3;
        if (!Same(v, k, 5)) return Fail();
        g_live += 3;
    }
    if (g_live != 3) return Fail();
    if (!CheckElem(src[0], 7)) return Fail();
    std::printf("00a39610_TEST PASS\n");
    return 0;
}