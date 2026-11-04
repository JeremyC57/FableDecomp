// std::vector<CHandleBasedMemoryPool_012b4098::CHeader*>::_Insert_n(iterator _Where, size_type _Count, const _Ty& _Val) -- VC7.1 Dinkumware vector insert of _Count copies: reallocate by 50% growth when full (copy prefix, fill, copy suffix), else spill past the end or shift the tail and fill the hole
// Leaf: ?_Insert_n@?$vector@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C295B0, 481 bytes, thiscall `ret 0Ch`, built /Gr (free helpers __fastcall).
// Identity: VC7.1 Dinkumware vector::_Insert_n of the libpk pool's defragment stack (landed push_back 0x00C29B30, _Xlen 0x00C28E50).
// Callees: _Xlen 0x00C28E50, operator new / delete, memmove (IAT), _Ufill 0x00C29580, _Ucopy 0x00C29480 (out of line at two sites,
//   inline at the two reallocation sites), copy_backward 0x00C29290, fill 0x00C28FC0.
// Shape: `_Xlen` is declared __declspec(noreturn): Dinkumware defines it in-class (always throws), so VC7.1 lets the next test follow.
class CHandleBasedMemoryPool_012b4098 {
public:
    class CHeader;                         // a pool block header (32 bytes)
};
typedef unsigned int size_t;
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, size_t count);   // IAT 0x01440144 (MSVCR71)
void* __cdecl operator new(size_t size);                                                            // 0x00BFEA1A
void __cdecl operator delete(void* p);                                                              // 0x00BFE9BC

namespace std {
struct _Nonscalar_ptr_iterator_tag { };
struct _Scalar_ptr_iterator_tag { };
struct random_access_iterator_tag { };
template<class _Category, class _Ty, class _Diff, class _Pointer, class _Reference>
struct iterator { };
template<class _Ty, class _Diff, class _Pointer, class _Reference>
struct _Ranit : public iterator<random_access_iterator_tag, _Ty, _Diff, _Pointer, _Reference> { };
template<class _T1, class _T2> inline _Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template<class _Ty> inline _Scalar_ptr_iterator_tag _Ptr_cat(_Ty**, _Ty**)
{
    _Scalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template<class _FwdIt, class _Ty> void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val);                // 0x00C28FC0 (out of line)
template<class _BidIt1, class _BidIt2> _BidIt2 copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest);  // 0x00C29290 (out of line)
template<class _Ty1, class _Ty2, class _Alloc> inline
_Ty2 _Uninit_copy(_Ty1 _First, _Ty1 _Last, _Ty2 _Dest, _Alloc&, _Scalar_ptr_iterator_tag)
{
    size_t _Count = (size_t)(_Last - _First);
    return ((_Ty2)memmove(&*_Dest, &*_First, _Count * sizeof (*_First)) + _Count);
}
template<class _InIt, class _FwdIt, class _Alloc> inline
_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al)
{
    return (_Uninit_copy(_First, _Last, _Dest, _Al, _Ptr_cat(_First, _Dest)));
}
template<class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    template<class _Other> struct rebind { typedef allocator<_Other> other; };
    allocator() { }
    allocator(const allocator<_Ty>&) { }
    pointer allocate(size_type _Count) { return ((pointer)operator new(_Count * sizeof (_Ty))); }
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
    size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof (_Ty); return (0 < _Count ? _Count : 1); }
};

template<class _Ty, class _Alloc> class _Vector_val {
protected:
    _Vector_val(_Alloc _Al = _Alloc()) : _Alval(_Al) { }
    typedef typename _Alloc::template rebind<_Ty>::other _Alty;
    _Alty _Alval;                                                        // +0x00 (empty, padded to a word)
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector : public _Vector_val<_Ty, _Ax> {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Ty* _Tptr;
    typedef _Ty& reference;
    typedef _Ty* pointer;
    typedef const _Ty* _Ctptr;
    typedef const _Ty& const_reference;
    class const_iterator : public _Ranit<_Ty, difference_type, _Ctptr, const_reference> {
    public:                                  // VC7.1: the vector's own iterators wrap one pointer, no user copy constructor
        const_iterator() { _Myptr = 0; }
        const_iterator(_Tptr _Ptr) { _Myptr = _Ptr; }
        _Tptr _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() { }
        iterator(pointer _Ptr) : const_iterator(_Ptr) { }
    };
    size_type capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }
    size_type size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    size_type max_size() const { return (this->_Alval.max_size()); }
protected:
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);
    template<class _Iter> pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)
    {
        return (_Uninitialized_copy(_First, _Last, _Ptr, this->_Alval));
    }
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val);     // 0x00C29580 (out of line)
    __declspec(noreturn) void _Xlen() const;                             // 0x00C28E50 (always throws length_error)
    pointer _Myfirst;                                                    // +0x04
    pointer _Mylast;                                                     // +0x08
    pointer _Myend;                                                      // +0x0c
};

template<class _Ty, class _Ax>
void vector<_Ty, _Ax>::_Insert_n(iterator _Where, size_type _Count, const _Ty& _Val)
{
    _Ty _Tmp = _Val;
    size_type _Capacity = capacity();

    if (_Count == 0)
        ;
    else if (max_size() - size() < _Count)
        _Xlen();
    else if (_Capacity < size() + _Count) {
        _Capacity = max_size() - _Capacity / 2 < _Capacity ? 0 : _Capacity + _Capacity / 2;
        if (_Capacity < size() + _Count)
            _Capacity = size() + _Count;
        pointer _Newvec = this->_Alval.allocate(_Capacity);
        pointer _Ptr = _Newvec;

        _Ptr = _Ucopy(_Myfirst, _Where._Myptr, _Newvec);
        _Ptr = _Ufill(_Ptr, _Count, _Tmp);
        _Ucopy(_Where._Myptr, _Mylast, _Ptr);

        _Count += size();
        if (_Myfirst != 0)
            this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
        _Myend = _Newvec + _Capacity;
        _Mylast = _Newvec + _Count;
        _Myfirst = _Newvec;
    } else if ((size_type)(_Mylast - _Where._Myptr) < _Count) {
        _Ucopy(_Where._Myptr, _Mylast, _Where._Myptr + _Count);
        _Ufill(_Mylast, _Count - (_Mylast - _Where._Myptr), _Tmp);
        _Mylast += _Count;
        fill(_Where._Myptr, _Mylast - _Count, _Tmp);
    } else {
        pointer _Oldend = _Mylast;
        _Mylast = _Ucopy(_Oldend - _Count, _Oldend, _Mylast);
        copy_backward(_Where._Myptr, _Oldend - _Count, _Oldend);
        fill(_Where._Myptr, _Where._Myptr + _Count, _Tmp);
    }
}
}

template void std::vector<CHandleBasedMemoryPool_012b4098::CHeader*>::_Insert_n(
    std::vector<CHandleBasedMemoryPool_012b4098::CHeader*>::iterator, unsigned int, CHandleBasedMemoryPool_012b4098::CHeader* const&);