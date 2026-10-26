// std::vector::push_back(const _Ty& _Val) -- VC7.1 Dinkumware vector<CHandleInfo>::push_back, the libpk pool's handle table: construct in place below capacity (_Ufill -> _Uninit_fill_n), else insert(end(), _Val) -> _Insert_n(end(), 1, _Val)
// Leaf: ?push_back@?$vector@VCHandleInfo@CHandleBasedMemoryPool@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool@@@std@@@std@@QAEXABVCHandleInfo@CHandleBasedMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29C30, length 98 bytes (padded to 112), thiscall, `ret 4`.
// Identity: the only caller is the pool's InternalAllocateHandle @ 0x00C28A80 (call 0x00C28AD1), which pushes a
//   CHandleInfo temporary with Header = 0 onto the handle table at pool+0x43C (Ego_r.pdb CHandleBasedMemoryPool::Handles,
//   CArray<CHandleBasedMemoryPool::CHandleInfo>; the libpk copy keeps a std::vector). The fill goes through the
//   NON-scalar _Uninit_fill_n @ 0x00C294B0 (its placement-new null test), which only a class element type selects --
//   an unsigned long vector would fill inline through the scalar path (as the CHeader* twin push_back @ 0x00C29B30 does).
//   Ego_r.pdb's name set carries the vector<CHandleBasedMemoryPool::CHandleInfo> instantiations (_Umove, _Ptr_cat
//   -> _Nonscalar_ptr_iterator_tag, _Uninit_fill_n); its _Xlen is the landed 0x00C28DC0.
// Callees: std::_Uninit_fill_n<CHandleInfo*, unsigned int, CHandleInfo, allocator<CHandleInfo> > @ 0x00C294B0 (__fastcall,
//   `ret 0Ch`, the non-scalar tag by value), vector::_Insert_n(iterator, size_type, const _Ty&) @ 0x00C298D0 (thiscall, `ret 0Ch`).
// Shape: Dinkumware size()/capacity() with their null-first tests (size in ecx, capacity in eax, unsigned compare); the empty
//   tag returned by _Ptr_cat is never stored, so VC pushes the dead `_Val` argument slot as the tag (`mov eax,[esp+8] ... push
//   eax`); insert's _Off and its return value are dead and fold away; _Mylast advanced from the cached register; the VC7.1
//   vector iterator is one pointer with no user copy constructor, so end() is pushed as a plain word.
// Label correction: the catalog's "CParticleProgram::AddTextureCoord" is propagated; the landed name was by address
//   (CVector_ulong_00c29c30, element type unknown).
inline void* __cdecl operator new(unsigned int, void* _Where) throw() { return (_Where); }

class CHandleBasedMemoryPool {
public:
    class CHeader;
    class CHandleInfo { public:                    // Ego_r.pdb: 4 bytes, a union of the block header and the free-handle link
        union { CHeader* Header; long NextFreeHandle; };
    };
};

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

template<class _OutIt, class _Diff, class _Ty> inline void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        *_First = _Val;
}

template<class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    template<class _Other> struct rebind { typedef allocator<_Other> other; };
    allocator() { }
    allocator(const allocator<_Ty>&) { }
    void construct(pointer _Ptr, const _Ty& _Val) { new ((void*)_Ptr) _Ty(_Val); }
};

template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void __fastcall _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag);
template<class _Ty, class _Diff, class _Tval, class _Alloc> inline
void _Uninit_fill_n(_Ty** _First, _Diff _Count, const _Tval& _Val, _Alloc&, _Scalar_ptr_iterator_tag)
{
    fill_n(_First, _Count, _Val);
}
template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}

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
        difference_type operator-(const const_iterator& _Right) const { return (_Myptr - _Right._Myptr); }
        _Tptr _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() { }
        iterator(pointer _Ptr) : const_iterator(_Ptr) { }
        iterator operator+(difference_type _Off) const { return (iterator(this->_Myptr + _Off)); }
    };
    iterator begin() { return (iterator(_Myfirst)); }
    iterator end() { return (iterator(_Mylast)); }
    size_type size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    size_type capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }
    void push_back(const _Ty& _Val)
    {
        if (size() < capacity())
            _Mylast = _Ufill(_Mylast, 1, _Val);
        else
            insert(end(), _Val);
    }
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        size_type _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (size_type)1, _Val);
        return (begin() + _Off);
    }
protected:
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);  // out of line
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }
    pointer _Myfirst;                                                    // +0x04
    pointer _Mylast;                                                     // +0x08
    pointer _Myend;                                                      // +0x0c
};
}

template void std::vector<CHandleBasedMemoryPool::CHandleInfo>::push_back(const CHandleBasedMemoryPool::CHandleInfo&);
