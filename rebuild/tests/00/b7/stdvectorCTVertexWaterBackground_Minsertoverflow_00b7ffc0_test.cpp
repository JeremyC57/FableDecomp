#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour test for 0x00B7FFC0 std::vector<CTVertexWaterBackground>::_M_insert_overflow: growth with one element at
// the end (push_back), one element in the middle, and a fill of several copies in the middle.
class CTVertexWaterBackground {
public:
    short X, Y;
    float Z;
    float ShoreLookUpArray[12];
};
static CTVertexWaterBackground MAKE(int k)
{
    CTVertexWaterBackground v; v.X = (short)k; v.Y = (short)(k + 1); v.Z = (float)k;
    for (int i = 0; i < 12; ++i) v.ShoreLookUpArray[i] = (float)(k * 100 + i);
    return v;
}

namespace stl {
struct __false_type { };
template <class T> const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> void _Construct(T1* p, const T2& v) { new (p) T1(v); }
template <class In, class Fwd> Fwd __uninitialized_copy(In first, In last, Fwd result, const __false_type&)
{
    Fwd cur = result;
    for (; first != last; ++first, ++cur) _Construct(&*cur, *first);
    return cur;
}
template <class Fwd, class Size, class T> Fwd __uninitialized_fill_n(Fwd first, Size n, const T& x, const __false_type&)
{
    Fwd cur = first;
    for (; n > 0; --n, ++cur) _Construct(&*cur, x);
    return cur;
}
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    int clears;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), clears(0) { }
    ~vector() { std::free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    void _M_clear() { ++clears; if (_M_start) std::free(_M_start); }
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    // The graded body (STLport 4.6 _vector.c).
    void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        const unsigned int __old_size = size();
        const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
        T* __new_start = __len != 0 ? (T*)std::malloc(__len * sizeof(T)) : 0;
        T* __new_finish = __new_start;
        __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        } else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
}

static int check(stl::vector<CTVertexWaterBackground>& v, const int* e, unsigned int n)
{
    if (v.size() != n) return 0;
    for (unsigned int i = 0; i < n; ++i) {
        const CTVertexWaterBackground& w = v._M_start[i];
        if (w.X != e[i] || w.Y != e[i] + 1 || w.Z != (float)e[i] || w.ShoreLookUpArray[11] != (float)(e[i] * 100 + 11)) return 0;
    }
    return 1;
}

int main()
{
    int bad = 0;
    stl::vector<CTVertexWaterBackground> v;
    v.push_back(MAKE(1));                                   // 0 -> len 1
    v.push_back(MAKE(2));                                   // 1 -> len 2
    v.push_back(MAKE(3));                                   // 2 -> len 4
    { int e[] = { 1, 2, 3 }; bad |= !check(v, e, 3) || v.capacity() != 4 || v.clears != 3; }
    v._M_insert_overflow(v._M_start + 1, MAKE(7), stl::__false_type(), 1, false);      // 3 + max(3,1) = 6
    { int e[] = { 1, 7, 2, 3 }; bad |= !check(v, e, 4) || v.capacity() != 6; }
    v._M_insert_overflow(v._M_start + 2, MAKE(9), stl::__false_type(), 5, false);      // 4 + max(4,5) = 9
    { int e[] = { 1, 7, 9, 9, 9, 9, 9, 2, 3 }; bad |= !check(v, e, 9) || v.capacity() != 9 || v.clears != 5; }
    v._M_insert_overflow(v._M_finish, MAKE(4), stl::__false_type(), 2, true);          // at end: no tail copy
    { int e[] = { 1, 7, 9, 9, 9, 9, 9, 2, 3, 4, 4 }; bad |= !check(v, e, 11) || v.capacity() != 18; }
    if (bad) { std::printf("00b7ffc0_TEST FAIL\n"); return 1; }
    std::printf("00b7ffc0_TEST PASS\n");
    return 0;
}