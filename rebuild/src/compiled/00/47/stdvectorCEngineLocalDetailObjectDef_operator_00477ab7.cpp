// std::vector<CEngineLocalDetailObjectDef>::operator=(const vector<CEngineLocalDetailObjectDef>& x) -- STLport copy assignment of a detail-object def vector: reallocate and copy, or assign in place and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4?$vector@VCEngineLocalDetailObjectDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477AB7, 323 bytes, thiscall `ret 4`, returns `this`; size TU with a frame pointer.
// Identity: the only caller is the def's member-wise copy 0x00477AAB; stride 0x88 with the element copy constructor
//   0x0047756C and assignment 0x00477964 (both landed). The catalog label "CEngineLocalDetailObjectDef_vector_Assign"
//   is right about the behaviour.
// Callees: _M_allocate_and_copy(n, first, last) 0x00477C18 (catalog "CEngineLocalDetailObjectDef_Allocate"),
//   _M_clear 0x00477BFC (landed as "CActiveFile::OnReadFinished"), CEngineLocalDetailObjectDef::operator= 0x00477964,
//   _Destroy(first, last) 0x004774B9 (__fastcall: the virtual-destructor loop), the copy constructor 0x0047756C.
// Shape: STLport 4.x `vector::operator=` verbatim: `__copy_ptrs` for a non-trivial assignment is the SGI counted
//   random-access `__copy` (signed `idiv` by 0x88, `jle` guard), `__uninitialized_copy` the null-checked placement copy.
// Model: CEngineLocalDetailObjectDef (vtable 0x0123634C, retail sizeof 0x88: the 0x3C-byte CParentDefClassBase, then the
//   detail-object values and PDB flag bits) is a local class (PDB `V`); the vector is the real STLport 4.6
//   vector<_Tp, allocator<_Tp> > over _Vector_base / _STLP_alloc_proxy, so its helpers bind the family's symbols:
//   _M_clear 0x00477BFC, _Destroy<CEngineLocalDetailObjectDef*> 0x004774B9 (as erase 0x00477E36 binds it) and the
//   out-of-line member template _M_allocate_and_copy<const CEngineLocalDetailObjectDef*> 0x00477C18.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

class CParentDefClassBase { public:                                // retail 0x3C (engine/CParentDefClassBase.h is the Ego_r 0x40 layout)
    unsigned char _body[0x3c - 4];                           // +0x04
    virtual ~CParentDefClassBase();
};

class CEngineLocalDetailObjectDef : public CParentDefClassBase {   // vtable 0x0123634C, retail sizeof 0x88
public:
    unsigned long Fields[(0x88 - 0x3c) / 4];                 // +0x3c
    CEngineLocalDetailObjectDef(const CEngineLocalDetailObjectDef& other);              // 0x0047756C
    CEngineLocalDetailObjectDef& operator=(const CEngineLocalDetailObjectDef& other);   // 0x00477964
};

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);             // 0x004774B9

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00477C18
    public:
    protected:
    void _M_clear();                                                                                 // 0x00477BFC
    public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template class vector<CEngineLocalDetailObjectDef>;

}