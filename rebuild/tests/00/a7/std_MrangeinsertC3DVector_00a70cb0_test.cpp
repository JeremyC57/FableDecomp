#include <cstdio>
#include <cstdlib>
#include <new>

// Test model of the STLport 4.6 vector growth/insert algorithms the row compiles (same statement order),
// over a plain element so it runs under g++ (64-bit) and VC7.1 alike.
struct C3DVector { float X, Y, Z; };
static C3DVector MakeElem(int k) { C3DVector v; v.X = (float)k; v.Y = 2.0f * k; v.Z = 0.0f; return v; }
static int KeyOf(const C3DVector& v) { return v.Y == 2.0f * v.X ? (int)v.X : -1; }

template <class T> struct TestVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    TestVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static T* ucopy(const T* first, const T* last, T* result)
    {
        for (; first != last; ++first, ++result)
            new (result) T(*first);
        return result;
    }
    static T* ufill(T* first, unsigned int n, const T& x)
    {
        for (; n > 0; --n, ++first)
            new (first) T(x);
        return first;
    }
    static T* copy_backward(T* first, T* last, T* result)
    {
        for (int n = (int)(last - first); n > 0; --n)
            *--result = *--last;
        return result;
    }
    static void fill(T* first, T* last, const T& v)
    {
        for (; first != last; ++first)
            *first = v;
    }
    static T* copy(const T* first, const T* last, T* result)
    {
        for (int n = (int)(last - first); n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    void _M_insert_overflow(T* position, const T& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* new_finish = ucopy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) T(x);
            ++new_finish;
        } else
            new_finish = ufill(new_finish, fill_len, x);
        if (!atend)
            new_finish = ucopy(position, _M_finish, new_finish);
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        if (_M_start)
            std::free(_M_start);
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
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                } else {
                    ufill(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    ucopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            } else
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
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_backward(position, old_finish - n, old_finish);
                    copy(first, last, position);
                } else {
                    const T* mid = first + elems_after;
                    ucopy(mid, last, _M_finish);
                    _M_finish += n - elems_after;
                    ucopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    copy(first, mid, position);
                }
            } else {
                const unsigned int old_size = size();
                const unsigned int len = old_size + (old_size < n ? n : old_size);
                T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
                T* new_finish = ucopy(_M_start, position, new_start);
                new_finish = ucopy(first, last, new_finish);
                new_finish = ucopy(position, _M_finish, new_finish);
                for (T* p = _M_start; p != _M_finish; ++p)
                    p->~T();
                if (_M_start)
                    std::free(_M_start);
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    }
    void reserve_exact(unsigned int n)   // test setup only
    {
        T* p = (T*)std::malloc(n * sizeof(T));
        T* f = ucopy(_M_start, _M_finish, p);
        for (T* q = _M_start; q != _M_finish; ++q)
            q->~T();
        if (_M_start)
            std::free(_M_start);
        _M_start = p;
        _M_finish = f;
        _M_end_of_storage = p + n;
    }
    void push(int k) { if (_M_finish == _M_end_of_storage) reserve_exact(size() * 2 + 1); new (_M_finish) T(MakeElem(k)); ++_M_finish; }
    void clear() { for (T* q = _M_start; q != _M_finish; ++q) q->~T(); if (_M_start) std::free(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; }
};

static int check(TestVector<C3DVector>& v, const int* expect, int n)
{
    if ((int)v.size() != n)
        return 0;
    for (int i = 0; i < n; ++i)
        if (KeyOf(v._M_start[i]) != expect[i])
            return 0;
    return 1;
}

int main()
{
    int ok = 1;
    static const int srcKeys[] = { 20, 21, 22 };
    C3DVector src[3] = { MakeElem(20), MakeElem(21), MakeElem(22) };
    (void)srcKeys;
    {   // spare capacity, elems_after > n
        TestVector<C3DVector> v; v.reserve_exact(16);
        for (int i = 0; i < 5; ++i) v.push(i);
        v._M_range_insert(v._M_start + 1, src, src + 2);
        static const int e[] = { 0, 20, 21, 1, 2, 3, 4 };
        ok &= check(v, e, 7); v.clear();
    }
    {   // spare capacity, elems_after <= n
        TestVector<C3DVector> v; v.reserve_exact(16);
        for (int i = 0; i < 3; ++i) v.push(i);
        v._M_range_insert(v._M_start + 2, src, src + 3);
        static const int e[] = { 0, 1, 20, 21, 22, 2 };
        ok &= check(v, e, 6); v.clear();
    }
    {   // reallocate
        TestVector<C3DVector> v; v.reserve_exact(2);
        for (int i = 0; i < 2; ++i) v.push(i);
        v._M_range_insert(v._M_start + 1, src, src + 3);
        static const int e[] = { 0, 20, 21, 22, 1 };
        ok &= check(v, e, 5);
        ok &= v._M_end_of_storage - v._M_start == 5;
        v.clear();
    }

    if (!ok) { std::printf("00a70cb0_TEST FAIL\n"); return 1; }
    std::printf("00a70cb0_TEST PASS\n");
    return 0;
}