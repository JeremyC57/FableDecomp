#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour test for 0x00AE6E60 std::vector<C3DQuad2>::_M_fill_insert: both in-capacity arms, n == 0 and the overflow path.
class C3DQuad2 { public: unsigned short VertexIndex[4]; C3DQuad2() { } };
typedef C3DQuad2 ELEM;
static C3DQuad2 MAKE(int k) { C3DQuad2 q; for (int i = 0; i < 4; ++i) q.VertexIndex[i] = (unsigned short)(k * 10 + i); return q; }
#define KEY(q) ((q).VertexIndex[0] / 10)

namespace stl {
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& v) { new (p) T1(v); }
template <class In, class Fwd> Fwd __uninitialized_copy(In first, In last, Fwd result, const __false_type&)
{
    Fwd cur = result;
    for (; first != last; ++first, ++cur) _Construct(&*cur, *first);
    return cur;
}
template <class Fwd, class Size, class T> Fwd uninitialized_fill_n(Fwd first, Size n, const T& x)
{
    Fwd cur = first;
    for (; n > 0; --n, ++cur) _Construct(&*cur, x);
    return cur;
}
template <class Bi1, class Bi2> Bi2 __copy_backward_ptrs(Bi1 first, Bi1 last, Bi2 result, const __false_type&)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
template <class Fwd, class T> void fill(Fwd first, Fwd last, const T& v) { for (; first != last; ++first) *first = v; }

template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); std::free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void reserve(unsigned int n)   // test helper: allocate spare capacity
    {
        T* p = (T*)std::malloc(n * sizeof(T));
        T* e = __uninitialized_copy(_M_start, _M_finish, p, __false_type());
        for (T* q = _M_start; q != _M_finish; ++q) q->~T();
        std::free(_M_start);
        _M_start = p; _M_finish = e; _M_end_of_storage = p + n;
    }
    int overflow_calls;
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool = false)
    {
        ++overflow_calls;
        unsigned int old = size();
        unsigned int len = old + (old > fill_len ? old : fill_len);
        T* p = (T*)std::malloc(len * sizeof(T));
        T* e = __uninitialized_copy(_M_start, pos, p, __false_type());
        e = uninitialized_fill_n(e, fill_len, x);
        e = __uninitialized_copy(pos, _M_finish, e, __false_type());
        for (T* q = _M_start; q != _M_finish; ++q) q->~T();
        std::free(_M_start);
        _M_start = p; _M_finish = e; _M_end_of_storage = p + len;
    }
    // The graded body (STLport 4.6 vector.c _M_fill_insert).
    void _M_fill_insert(T* __position, unsigned int __n, const T& __x)
    {
        if (__n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
                T __x_copy = __x;
                const unsigned int __elems_after = _M_finish - __position;
                T* __old_finish = _M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                    _M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    fill(__position, __position + __n, __x_copy);
                }
                else {
                    uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                    _M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                    _M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }
};
}

static int check(stl::vector<ELEM>& v, const int* expect, unsigned int n)
{
    if (v.size() != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (KEY(v._M_start[i]) != expect[i]) return 0;
    return 1;
}

int main()
{
    int bad = 0;
    stl::vector<ELEM> v; v.overflow_calls = 0;
    v._M_fill_insert(v._M_finish, 2, MAKE(1));                    // empty: grows through _M_insert_overflow
    { int e[] = { 1, 1 }; bad |= !check(v, e, 2) || v.overflow_calls != 1; }
    v.reserve(16);
    v._M_fill_insert(v._M_finish, 3, MAKE(2));                    // append inside capacity (elems_after 0)
    { int e[] = { 1, 1, 2, 2, 2 }; bad |= !check(v, e, 5); }
    v._M_fill_insert(v._M_start + 1, 2, MAKE(7));                 // elems_after 4 > n 2: slide tail, fill hole
    { int e[] = { 1, 7, 7, 1, 2, 2, 2 }; bad |= !check(v, e, 7); }
    v._M_fill_insert(v._M_start + 5, 4, MAKE(9));                 // elems_after 2 <= n 4: fill past end, move tail
    { int e[] = { 1, 7, 7, 1, 2, 9, 9, 9, 9, 2, 2 }; bad |= !check(v, e, 11); }
    v._M_fill_insert(v._M_start, 0, MAKE(5));                     // n == 0: no change
    { int e[] = { 1, 7, 7, 1, 2, 9, 9, 9, 9, 2, 2 }; bad |= !check(v, e, 11) || v.overflow_calls != 1; }
    v._M_fill_insert(v._M_start + 2, 6, MAKE(4));                 // 11 + 6 > 16: overflow path again
    { int e[] = { 1, 7, 4, 4, 4, 4, 4, 4, 7, 1, 2, 9, 9, 9, 9, 2, 2 }; bad |= !check(v, e, 17) || v.overflow_calls != 2; }
    for (unsigned int i = 0; i < v.size(); ++i) if (v._M_start[i].VertexIndex[3] != v._M_start[i].VertexIndex[0] + 3) bad = 1;
    if (bad) { std::printf("00ae6e60_TEST FAIL\n"); return 1; }
    std::printf("00ae6e60_TEST PASS\n");
    return 0;
}