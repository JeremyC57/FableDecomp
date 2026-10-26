// std::vector::_Ufill(CHandleBasedMemoryPool::CHandleInfo* _Ptr, unsigned int _Count, const CHandleBasedMemoryPool::CHandleInfo& _Val) -- VC7.1 Dinkumware vector helper: copy-construct _Count copies of _Val into raw storage at _Ptr through the allocator and return _Ptr + _Count
// Leaf: ?_Ufill@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C29840, 38 bytes, thiscall `ret 0Ch` (libpk middleware: Dinkumware STL in namespace std).
// Callees: _Uninit_fill_n(_First, _Count, _Val, _Al, _Nonscalar_ptr_iterator_tag) 0x00C294B0 (landed; __fastcall: _First ecx,
//   _Count edx, the rest on the stack).
// Identity: the landed 0x00C294B0 names this caller vector<CHandleBasedMemoryPool::CHandleInfo>::_Ufill; Ego_r.pdb
//   CHandleBasedMemoryPool::CHandleInfo is a 4-byte union of the block header pointer and the free-handle link. The catalog
//   label "vector<unsigned int>::_Ufill" carries the right member name with a propagated element type.
// Shape: Dinkumware `_Ufill(_Ptr, _Count, _Val) { _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval); return (_Ptr +
//   _Count); }` with the `_Uninitialized_fill_n` front end inline (`_Uninit_fill_n(..., _Ptr_cat(_First, _First))`): the
//   allocator is the vector's first word (ecx itself), the empty tag is pushed from whatever register holds a value.
inline void* __cdecl operator new(unsigned int, void* _Where) throw() { return (_Where); }

class CHandleBasedMemoryPool {
public:
    class CHeader;
    class CHandleInfo { public:                        // Ego_r.pdb: 4 bytes
        union { CHeader* Header; long NextFreeHandle; };
    };
};

namespace std {
struct _Nonscalar_ptr_iterator_tag { };

template<class _T1, class _T2> inline _Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}

template<class _Ty> class allocator {
public:
    allocator() { }
};

template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void __fastcall _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al,
                               _Nonscalar_ptr_iterator_tag);                                     // 0x00C294B0

template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}

template<class _Ty, class _Alloc> class _Vector_val {
public:
    _Vector_val(_Alloc _Al = _Alloc()) : _Alval(_Al) { }
    _Alloc _Alval;                              // +0x0 (the empty allocator padded to a word)
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector : public _Vector_val<_Ty, _Ax> {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    pointer _Myfirst;                           // +0x4
    pointer _Mylast;                            // +0x8
    pointer _Myend;                             // +0xC
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }
};

template CHandleBasedMemoryPool::CHandleInfo* vector<CHandleBasedMemoryPool::CHandleInfo>::_Ufill(
    CHandleBasedMemoryPool::CHandleInfo*, unsigned int, const CHandleBasedMemoryPool::CHandleInfo&);
}