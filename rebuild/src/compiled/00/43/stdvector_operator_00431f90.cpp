// std::vector::operator=(const vector& x) -- STLport 4.6 vector assignment for the 12-byte pair<unsigned long, CDefClassBase::CSubDefInfo>
// Retail: Fable.exe (TLC, VC7.1) @ 0x00431F90, 397 bytes, thiscall, `ret 4`, returns this.
// Identity: std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >::operator= -- the PDB name set holds
//   ??4?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$allocator@...@std@@QAEAAV01@ABV01@@Z; the callers are the
//   CParentDefClassBase-derived def assignments (SubDefInfoMap, e.g. landed 0x0046296E / 0x00432E60), and its callee
//   0x0043212B is the landed _M_allocate_and_copy of the same vector.
// Shape: STLport 4.6 verbatim. Three arms: grow (allocate+copy, _M_clear, new start / end of storage), shrink
//   (__copy_ptrs with __false_type = the counted __copy loop, trivial _Destroy), and copy-then-uninitialized-copy
//   (counted __copy over size() elements, then the placement-new loop with its null test). pair carries STLport's
//   explicit copy constructor (dword-wise copy in the uninitialized loop).
// Callees: _M_allocate_and_copy 0x0043212B, _M_clear 0x0043211D (the 14-byte `if (_M_start) free(_M_start)`).
// Leaf: ??4?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$allocator@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
#include <stddef.h>
#include <new>

class CDefClassBase {
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class T> class allocator {
public:
    void deallocate(T* p, size_t) { if (p) ::operator delete(p); }
};

template <class Value, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                         const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T>
__forceinline T* __copy_ptrs(const T* first, const T* last, T* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A>
class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};

template <class T, class A = allocator<T> >
class vector : protected _Vector_base<T, A> {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef size_t size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }

    protected:
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last);   // 0x0043212B
    public:
    protected:
    void _M_clear();                                                                     // 0x0043211D
    public:

    vector<T, A>& operator=(const vector<T, A>& x);
};

template <class T, class A>
vector<T, A>& vector<T, A>::operator=(const vector<T, A>& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
            _M_clear();
            this->_M_start = tmp;
            this->_M_end_of_storage._M_data = this->_M_start + xlen;
        }
        else if (size() >= xlen) {
            pointer i = __copy_ptrs((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0,
                                    (pointer)this->_M_start, _TrivialAss());
            _Destroy(i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)x._M_start, (const_pointer)x._M_start + size(), (pointer)this->_M_start,
                        _TrivialAss());
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, this->_M_finish,
                                 _IsPODType());
        }
        this->_M_finish = this->_M_start + xlen;
    }
    return *this;
}

template class vector<pair<unsigned long, CDefClassBase::CSubDefInfo> >;
}