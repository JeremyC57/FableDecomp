// Behaviour test for 0x00A70BC0: vector<C3DTriangle2>::reserve
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};

namespace _STL {
class __false_type { public: __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _IsPODType;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector()
    {
        for (pointer __p = this->_M_start; __p != this->_M_finish; ++__p)
            __p->~_Tp();
    }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    void _M_clear()
    {
        for (pointer __p = this->_M_start; __p != this->_M_finish; ++__p)
            __p->~_Tp();
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(size_type __n)
    {
        if (capacity() < __n) {
            const size_type __old_size = size();
            pointer __tmp;
            if (this->_M_start) {
                __tmp = _M_allocate_and_copy(__n, (const _Tp*)this->_M_start, (const _Tp*)this->_M_finish);
                _M_clear();
            }
            else {
                __tmp = this->_M_end_of_storage.allocate(__n);
            }
            _M_set(__tmp, __tmp + __old_size, __tmp + __n);
        }
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

}

int main()
{
    int ok = 1;
    {
        _STL::vector<C3DTriangle2> v;
        v.reserve(0u);
        if (v._M_start != 0 || v.capacity() != 0)
            ok = 0;
        v.reserve(2u);                       // from nothing: plain allocate
        if (v._M_start == 0 || v.capacity() != 2 || v.size() != 0)
            ok = 0;
        for (int i = 0; i < 2; ++i) {
            C3DTriangle2 t;
            t.VertexIndex[0] = (unsigned short)i;
            t.VertexIndex[1] = (unsigned short)(i + 100);
            t.VertexIndex[2] = (unsigned short)(i + 200);
            *v._M_finish++ = t;
        }
        C3DTriangle2* old = v._M_start;
        v.reserve(2u);                       // enough already: untouched
        if (v._M_start != old || v.capacity() != 2)
            ok = 0;
        v.reserve(7u);                       // grow: copy and free the old block
        if (v.capacity() != 7 || v.size() != 2)
            ok = 0;
        for (int j = 0; j < 2; ++j)
            if (v._M_start[j].VertexIndex[0] != j || v._M_start[j].VertexIndex[1] != j + 100 || v._M_start[j].VertexIndex[2] != j + 200)
                ok = 0;
    }

    if (!ok) {
        std::printf("00a70bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a70bc0_TEST PASS\n");
    return 0;
}