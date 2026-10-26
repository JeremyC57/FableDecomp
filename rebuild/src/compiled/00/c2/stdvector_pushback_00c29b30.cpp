// std::vector::push_back(const _Ty& _Val) -- VC7.1 Dinkumware vector<CHeader*>::push_back, the pool's defragment block list: store in place below capacity (_Ufill -> the scalar fill_n, inline), else insert(end(), _Val) -> _Insert_n(end(), 1, _Val)
// Leaf: ?push_back@?$vector@PAVCHeader@CHandleBasedMemoryPool@@V?$allocator@PAVCHeader@CHandleBasedMemoryPool@@@std@@@std@@QAEXABQAVCHeader@CHandleBasedMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29B30, length 77 bytes, thiscall, `ret 4`.
// Identity: the only caller is the libpk CHandleBasedMemoryPool::PartialDefragment @ 0x00C28870 (call 0x00C289F8; Ego_r.pdb
//   ?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z), which walks the free lists (pool+0x3C, Ego_r FreeLists
//   CHeader*[256]), searches its local vector (+0x24 allocator, +0x28/+0x2C/+0x30) for the header pointer and pushes the
//   header when absent. The scalar element type is why the fill is an inline store (no placement-new null test, unlike the
//   CHandleInfo twin @ 0x00C29C30). Ego_r.pdb's name set carries the vector<CHandleBasedMemoryPool::CHeader*> instantiations
//   (_Umove, _Ptr_cat -> _Scalar_ptr_iterator_tag, _Uninit_fill_n); its _Xlen is the landed 0x00C28E50.
// Callees: vector::_Insert_n(iterator, size_type, const _Ty&) @ 0x00C295B0 (thiscall, `ret 0Ch`; it calls _Xlen 0x00C28E50).
// Shape: the size (null first -> 0) in edx, the capacity (null first -> the slow path) in eax, an unsigned compare; the VC7.1
//   vector iterator is one pointer with no user copy constructor, pushed as a plain word.
// Label correction: the catalog's "CMirroredTextureHighlightsChunk::AddVertexIndex" is propagated; the landed name was by
//   address (CVector_ulong_00c29b30, element type unknown).
class CHandleBasedMemoryPool {
public:
    class CHeader;                         // a pool block header (32 bytes)
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

template void std::vector<CHandleBasedMemoryPool::CHeader*>::push_back(CHandleBasedMemoryPool::CHeader* const&);
