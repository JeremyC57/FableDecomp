// behaviour test for 0x005798C8 _STL::vector<NUISystem::CComponent*>::operator=: grow, shrink and fill-the-tail branches
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace NUISystem { class CComponent { public: int Id; }; }

static int g_allocs = 0, g_frees = 0;

namespace _STL {

struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { if (__n == 0) return 0; ++g_allocs; return (_Tp*)std::malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p) { ++g_frees; std::free(__p); } }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)   // stand-in for 0x00405BA0
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
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    vector(size_type __n, size_type __cap)                   // test helper: n slots of a capacity
    {
        this->_M_start = this->_M_end_of_storage.allocate(__cap);
        this->_M_finish = this->_M_start + __n;
        this->_M_end_of_storage._M_data = this->_M_start + __cap;
    }
    ~vector() { _M_clear(); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)   // stand-in for 0x005796D1
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

typedef _STL::vector<NUISystem::CComponent*> TSliders;

static NUISystem::CComponent g_items[8];

static void FillFrom(TSliders& v, int base) { for (unsigned int i = 0; i < v.size(); ++i) v._M_start[i] = &g_items[(base + i) % 8]; }
static bool Same(const TSliders& a, const TSliders& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (a._M_start[i] != b._M_start[i]) return false;
    return true;
}

int main()
{
    // grow: x is larger than the capacity -> a new block, the old one freed
    {
        TSliders x(5, 5); FillFrom(x, 0);
        TSliders v(1, 2); FillFrom(v, 3);
        int frees = g_frees;
        TSliders& r = (v = x);
        if (&r != &v || !Same(v, x) || v.capacity() != 5 || g_frees != frees + 1) { std::printf("005798c8_TEST FAIL\n"); return 1; }
    }
    // shrink: size() >= xlen -> copied over the prefix, capacity kept
    {
        TSliders x(2, 2); FillFrom(x, 4);
        TSliders v(4, 6); FillFrom(v, 0);
        int allocs = g_allocs;
        v = x;
        if (!Same(v, x) || v.capacity() != 6 || g_allocs != allocs) { std::printf("005798c8_TEST FAIL\n"); return 1; }
    }
    // fill the tail: size() < xlen <= capacity()
    {
        TSliders x(5, 5); FillFrom(x, 2);
        TSliders v(2, 7); FillFrom(v, 6);
        int allocs = g_allocs;
        v = x;
        if (!Same(v, x) || v.capacity() != 7 || g_allocs != allocs) { std::printf("005798c8_TEST FAIL\n"); return 1; }
    }
    // self-assignment leaves everything alone
    {
        TSliders v(3, 3); FillFrom(v, 1);
        NUISystem::CComponent* first = v._M_start[0];
        v = v;
        if (v.size() != 3 || v._M_start[0] != first) { std::printf("005798c8_TEST FAIL\n"); return 1; }
    }
    std::printf("005798c8_TEST PASS\n");
    return 0;
}