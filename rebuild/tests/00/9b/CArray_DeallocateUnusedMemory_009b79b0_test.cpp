// Behaviour test for 0x009B79B0 CArray::DeallocateUnusedMemory (source body copied, callee stand-ins included).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CDefClassBase { public: class CSubDefInfo { public: unsigned long a, b; }; };
namespace std { template <class _T1, class _T2> class pair { public: _T1 first; _T2 second; }; }
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#endif

namespace std {
struct forward_iterator_tag { forward_iterator_tag() { } };
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void reserve(size_type __n);
    void _M_range_insert(iterator __pos, const_iterator __first, const_iterator __last, const forward_iterator_tag&);
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        _M_range_insert(__pos, __first, __last, forward_iterator_tag());
    }
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
    }
};

#pragma auto_inline(off)
// vector::reserve -- same-TU noinline body (STLport: allocate, copy, free the old block, adopt).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp = this->_M_end_of_storage.allocate(__n);
        for (pointer __s = this->_M_start, __d = __tmp; __s != this->_M_finish; ++__s, ++__d)
            new (__d) _Tp(*__s);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = __tmp;
        this->_M_finish = __tmp + __old_size;
        this->_M_end_of_storage._M_data = __tmp + __n;
    }
}
// vector::_M_range_insert(pos, first, last, forward_iterator_tag) -- same-TU noinline stand-in (append into the reserved
// room; never touches the tag).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos, const_iterator __first, const_iterator __last, const forward_iterator_tag&)
{
    for (; __first != __last; ++__first, ++this->_M_finish)
        new (this->_M_finish) _Tp(*__first);
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateMemory();
    void DeallocateUnusedMemory();
};

#pragma auto_inline(off)
// CArray::DeallocateMemory -- out of line in retail; same-TU noinline body (empty the array and free its block).
template <class T> __declspec(noinline) void CArray<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->_M_finish = this->_M_start;
        T* p = this->_M_start;
        this->_M_start = 0;
        this->_M_finish = 0;
        this->_M_end_of_storage._M_data = 0;
        if (p)
            free(p);
    }
}
#pragma auto_inline(on)

template <class T> void CArray<T>::DeallocateUnusedMemory()
{
    if (this->capacity() > this->size()) {
        CArray<T> temp;
        temp.reserve(this->size());
        temp.insert(temp.begin(), this->begin(), this->end());
        DeallocateMemory();
        this->swap(temp);
    }
}

template class CArray<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >;
typedef std::pair<unsigned long, CDefClassBase::CSubDefInfo> E;
static E Make(int i) { E e; e.first = 100 + i; e.second.a = 7 * i; e.second.b = 9 + i; return e; }
static int Same(const E& e, int i) { return e.first == (unsigned long)(100 + i) && e.second.a == (unsigned long)(7 * i) && e.second.b == (unsigned long)(9 + i); }

int main()
{
    int ok = 1;
    CArray<std::pair<unsigned long, CDefClassBase::CSubDefInfo> > v;
    v.reserve(8);
    for (int i = 0; i < 5; ++i) { std::pair<unsigned long, CDefClassBase::CSubDefInfo> x = Make(i); v.insert(v.end(), &x, &x + 1); }
    if (v.size() != 5 || v.capacity() != 8) ok = 0;
    v.DeallocateUnusedMemory();
    if (v.size() != 5 || v.capacity() != 5) ok = 0;
    for (int i = 0; i < 5; ++i) if (!Same(v.begin()[i], i)) ok = 0;
    v.DeallocateUnusedMemory();                 // already tight: nothing changes
    if (v.size() != 5 || v.capacity() != 5) ok = 0;
    CArray<std::pair<unsigned long, CDefClassBase::CSubDefInfo> > e;
    e.DeallocateUnusedMemory();                 // empty: nothing to do
    if (e.begin() != 0 || e.capacity() != 0) ok = 0;
    printf("009b79b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}