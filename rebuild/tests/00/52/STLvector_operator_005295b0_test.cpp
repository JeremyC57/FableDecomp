// Behaviour test for 0x005295B0: STLport vector<unsigned char>::operator=(x) -- reallocate-and-copy when x outgrows the
// capacity, else copy over the live bytes (and append the rest); self-assignment is a no-op.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int gMallocs = 0;
static int gFrees = 0;
namespace _STL {
struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { if (__n != 0) ++gMallocs; return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) { ++gFrees; std::free(__p); } }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __true_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    vector& operator=(const vector& __x);
    // test helpers
    void TestInit(const char* bytes, size_type n, size_type cap)
    {
        this->_M_start = cap ? this->_M_end_of_storage.allocate(cap) : 0;
        if (n)
            std::memcpy(this->_M_start, bytes, n);
        this->_M_finish = this->_M_start + n;
        this->_M_end_of_storage._M_data = this->_M_start + cap;
    }
    bool TestIs(const char* bytes, size_type n) const { return size() == n && (n == 0 || std::memcmp(this->_M_start, bytes, n) == 0); }
    pointer TestData() const { return this->_M_start; }
    void TestFree() { _M_clear(); }
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __true_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __true_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __true_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}
typedef _STL::vector<unsigned char> TBytes;

int main()
{
    bool ok = true;
    TBytes a, b, c, d, e;
    a.TestInit("abc", 3, 3);
    b.TestInit("0123456789", 10, 10);
    a = b;                                              // grow: new block of exactly 10, old freed
    ok = a.TestIs("0123456789", 10) && a.capacity() == 10 && a.TestData() != b.TestData() && gFrees == 1;
    c.TestInit("xyz", 3, 3);
    unsigned char* keep = a.TestData();
    a = c;                                              // shrink: same block, size 3
    ok = ok && a.TestIs("xyz", 3) && a.TestData() == keep && a.capacity() == 10;
    d.TestInit("QRSTUV", 6, 6);
    a = d;                                              // within capacity, longer than size: copy 3 + append 3
    ok = ok && a.TestIs("QRSTUV", 6) && a.TestData() == keep && a.capacity() == 10;
    a = a;                                              // self-assignment
    ok = ok && a.TestIs("QRSTUV", 6) && a.TestData() == keep;
    e.TestInit("", 0, 0);
    a = e;                                              // empty source
    ok = ok && a.TestIs("", 0) && a.TestData() == keep && gMallocs == 5;
    a.TestFree(); b.TestFree(); c.TestFree(); d.TestFree(); e.TestFree();
    ok = ok && gFrees == gMallocs;
    if (!ok) {
        std::printf("005295b0_TEST FAIL\n");
        return 1;
    }
    std::printf("005295b0_TEST PASS\n");
    return 0;
}