// std::vector::_Insert_n(iterator _Where, size_type _Count, const CHandleBasedMemoryPool::CHandleInfo& _Val) -- VC7.1 Dinkumware vector<CHandleBasedMemoryPool::CHandleInfo>::_Insert_n (libpk handle table): insert _Count copies of _Val at _Where, reallocating by 1.5x when full
// Grade: DIFFER(352v542) -- catalog extent only: RELOCATION_MATCH over the true 542-byte function 0x00C298D0..0x00C29AEE (qx.py, /EHsc /Gr); re-cut the row and merge Catch@00c29a30 / Catch@00c29a3c into it, then it lands as is
// Leaf: ?_Insert_n@?$vector@VCHandleInfo@CHandleBasedMemoryPool@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool@@@std@@@std@@IAEXViterator@12@IABVCHandleInfo@CHandleBasedMemoryPool@@@Z
// Flags: /EHsc /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C298D0, thiscall `ret 0Ch`, EH frame (handler 0x01213490). The catalog row
//   is 352 bytes (up to the first `ret 0Ch` at 0x00C29A2D); the function is 542 bytes (0x00C298D0..0x00C29AEE): its two
//   catch funclets follow that ret (catalog rows "Catch@00c29a30" and "Catch@00c29a3c", tail-merged on the shared
//   `_RERAISE`), then the spill / hole branches of the body (0x00C29A45..0x00C29AEB, second `ret 0Ch`).
// Identity: callers vector<CHandleInfo>::insert(iterator, const T&) 0x00C29B80 (`_Insert_n(_Where, 1, _Val)`) and the landed
//   push_back 0x00C29C30 (std::vector<CHandleBasedMemoryPool::CHandleInfo>, pool handle table at +0x43C). Element: Ego_r.pdb
//   class CHandleBasedMemoryPool::CHandleInfo (4-byte union), non-scalar, so the copies go through _Uninit_copy /
//   _Uninit_fill_n with the allocator. The CHeader* twin is 0x00C295B0 (memmove paths). libpk is built /Gr: the PDB name
//   sets spell its free STL helpers `YI` (FableWin.pdb `??$fill@PAVCHandleInfo@CHandleBasedMemoryPool@@V12@@std@@YIX...`).
// Callees: vector::_Xlen 0x00C28DC0; operator new 0x00BFEA1A / operator delete 0x00BFE9BC (allocate / deallocate);
//   std::_Uninit_copy<CHandleInfo*,CHandleInfo*,allocator<CHandleInfo> > 0x00C29340; std::_Uninit_fill_n<CHandleInfo*,
//   unsigned int,CHandleInfo,allocator<CHandleInfo> > 0x00C294B0; vector::_Ufill 0x00C29840 (second site, not inlined);
//   std::copy_backward<CHandleInfo*,CHandleInfo*> 0x00C29250; std::fill<CHandleInfo*,CHandleInfo> 0x00C28FA0;
//   _CxxThrowException 0x00BFEB84 (`_RERAISE`).
void* __cdecl operator new(unsigned int);
void __cdecl operator delete(void*) throw();
inline void* __cdecl operator new(unsigned int, void* _Where) throw() { return (_Where); }

class CHandleBasedMemoryPool {
public:
    class CHeader;
    class CHandleInfo { public:                    // Ego_r.pdb: 4 bytes, a union of the block header and the free-handle link
        union { CHeader* Header; long NextFreeHandle; };
    };
};

class exception {                                                        // MSVCR71 (as the landed _Xlen 0x00C28DC0)
public:
    __declspec(dllimport) exception();
    __declspec(dllimport) exception(const exception&);
    __declspec(dllimport) virtual ~exception();
    __declspec(dllimport) virtual const char* what() const;
private:
    const char* _m_what;
    int _m_doFree;
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

template<class _FwdIt, class _Ty> inline
void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}

template<class _BidIt1, class _BidIt2> inline
_BidIt2 copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}

template<class _Ty> inline _Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof (_Ty)));
}
template<class _T1, class _T2> inline void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}
template<class _Ty> inline void _Destroy(_Ty* _Ptr)
{
    _Ptr->~_Ty();
}

template<class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef _Ty& reference;
    typedef const _Ty& const_reference;
    typedef unsigned int size_type;
    template<class _Other> struct rebind { typedef allocator<_Other> other; };
    allocator() { }
    allocator(const allocator<_Ty>&) { }
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
    pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
    size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof (_Ty); return (0 < _Count ? _Count : 1); }
};

template<class _Elem> struct char_traits { };

template<class _Elem, class _Traits, class _Ax> class basic_string {     // MSVCP71 imports
public:
    __declspec(dllimport) basic_string(const _Elem* _Ptr);
    __declspec(dllimport) basic_string(const basic_string& _Right);
    __declspec(dllimport) ~basic_string();
    __declspec(dllimport) const _Elem* c_str() const;
private:
    _Ax _Alval;
    union { _Elem _Buf[16]; _Elem* _Ptr; } _Bx;
    unsigned int _Mysize;
    unsigned int _Myres;
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

class logic_error : public exception {
public:
    explicit logic_error(const string& _Message) : _Str(_Message) { }
    virtual ~logic_error() throw() { }
    virtual const char* what() const throw() { return (_Str.c_str()); }
private:
    string _Str;
};

class length_error : public logic_error {
public:
    explicit length_error(const string& _Message) : logic_error(_Message) { }
    virtual ~length_error() throw() { }
};

template<class _FwdIt, class _Alloc> inline
void _Destroy_range(_FwdIt _First, _FwdIt _Last, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template<class _FwdIt, class _Alloc> inline
void _Destroy_range(_FwdIt _First, _FwdIt _Last, _Alloc& _Al)
{
    _Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
}

template<class _InIt, class _FwdIt, class _Alloc> inline
_FwdIt _Uninit_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    _FwdIt _Next = _Dest;
    try {
        for (; _First != _Last; ++_Dest, ++_First)
            _Al.construct(_Dest, *_First);
    } catch (...) {
        for (; _Next != _Dest; ++_Next)
            _Al.destroy(_Next);
        throw;
    }
    return (_Dest);
}
template<class _InIt, class _FwdIt, class _Alloc> inline
_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al)
{
    return (_Uninit_copy(_First, _Last, _Dest, _Al, _Ptr_cat(_First, _Dest)));
}

template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    _FwdIt _Next = _First;
    try {
        for (; 0 < _Count; --_Count, ++_First)
            _Al.construct(_First, _Val);
    } catch (...) {
        for (; _Next != _First; ++_Next)
            _Al.destroy(_Next);
        throw;
    }
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
    typedef vector<_Ty, _Ax> _Myt;
    typedef _Vector_val<_Ty, _Ax> _Mybase;
    typedef typename _Mybase::_Alty _Alloc;
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
    size_type max_size() const { return (this->_Alval.max_size()); }
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        size_type _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (size_type)1, _Val);
        return (begin() + _Off);
    }
protected:
    void _Destroy(pointer _First, pointer _Last)
    {
        _Destroy_range(_First, _Last, this->_Alval);
    }
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val)
    {   // insert _Count * _Val at _Where
        _Ty _Tmp = _Val;    // in case _Val is in sequence
        size_type _Capacity = capacity();

        if (_Count == 0)
            ;
        else if (max_size() - size() < _Count)
            _Xlen();    // result too long
        else if (_Capacity < size() + _Count)
        {   // not enough room, reallocate
            _Capacity = max_size() - _Capacity / 2 < _Capacity
                ? 0 : _Capacity + _Capacity / 2;    // try to grow by 50%
            if (_Capacity < size() + _Count)
                _Capacity = size() + _Count;
            pointer _Newvec = this->_Alval.allocate(_Capacity);
            pointer _Ptr = _Newvec;

            try {
                _Ptr = _Ucopy(_Myfirst, _Where._Myptr, _Newvec);    // copy prefix
                _Ptr = _Ufill(_Ptr, _Count, _Tmp);    // add new stuff
                _Ucopy(_Where._Myptr, _Mylast, _Ptr);    // copy suffix
            } catch (...) {
                _Destroy(_Newvec, _Ptr);
                this->_Alval.deallocate(_Newvec, _Capacity);
                throw;
            }

            _Count += size();
            if (_Myfirst != 0)
            {   // destroy and deallocate old array
                _Destroy(_Myfirst, _Mylast);
                this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
            }
            _Myend = _Newvec + _Capacity;
            _Mylast = _Newvec + _Count;
            _Myfirst = _Newvec;
        }
        else if ((size_type)(_Mylast - _Where._Myptr) < _Count)
        {   // new stuff spills off end
            _Ucopy(_Where._Myptr, _Mylast,
                _Where._Myptr + _Count);    // copy suffix

            try {
                _Ufill(_Mylast, _Count - (_Mylast - _Where._Myptr),
                    _Tmp);    // insert new stuff off end
            } catch (...) {
                _Destroy(_Where._Myptr + _Count, _Mylast + _Count);
                throw;
            }

            _Mylast += _Count;
            fill(_Where._Myptr, _Mylast - _Count,
                _Tmp);    // insert up to old end
        }
        else
        {   // new stuff can all be assigned
            pointer _Oldend = _Mylast;
            _Mylast = _Ucopy(_Oldend - _Count, _Oldend,
                _Mylast);    // copy suffix
            copy_backward(_Where._Myptr, _Oldend - _Count,
                _Oldend);    // copy hole
            fill(_Where._Myptr, _Where._Myptr + _Count,
                _Tmp);    // insert into hole
        }
    }
    template<class _Iter>
    pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)
    {
        return (_Uninitialized_copy(_First, _Last, _Ptr, this->_Alval));
    }
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }
    void _Xlen() const
    {   // report a length_error (0x00C28DC0, out of line: it has an EH frame)
        throw length_error("vector<T> too long");
    }
    pointer _Myfirst;                                                    // +0x04
    pointer _Mylast;                                                     // +0x08
    pointer _Myend;                                                      // +0x0c
};
}

template std::vector<CHandleBasedMemoryPool::CHandleInfo>::iterator
    std::vector<CHandleBasedMemoryPool::CHandleInfo>::insert(iterator, const CHandleBasedMemoryPool::CHandleInfo&);
