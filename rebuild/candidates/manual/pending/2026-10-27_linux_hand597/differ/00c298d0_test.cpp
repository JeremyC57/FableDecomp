#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
static int g_allocs = 0, g_frees = 0;
// tstd::vector::_Insert_n(iterator _Where, size_type _Count, const CHandleBasedMemoryPool::CHandleInfo& _Val) -- VC7.1 Dinkumware vector<CHandleBasedMemoryPool::CHandleInfo>::_Insert_n (libpk handle table): insert _Count copies of _Val at _Where, reallocating by 1.5x when full
// Leaf: ?_Insert_n@?$vector@VCHandleInfo@CHandleBasedMemoryPool@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool@@@std@@@std@@IAEXViterator@12@IABVCHandleInfo@CHandleBasedMemoryPool@@@Z
// Flags: /EHsc /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C298D0, thiscall `ret 0Ch`, EH frame (handler 0x01213490). The catalog row
//   is 352 bytes (up to the first `ret 0Ch` at 0x00C29A2D); the function is 542 bytes (0x00C298D0..0x00C29AEE): its two
//   catch funclets follow that ret (catalog rows "Catch@00c29a30" and "Catch@00c29a3c", tail-merged on the shared
//   `_RERAISE`), then the spill / hole branches of the body (0x00C29A45..0x00C29AEB, second `ret 0Ch`).
// Identity: callers vector<CHandleInfo>::insert(iterator, const T&) 0x00C29B80 (`_Insert_n(_Where, 1, _Val)`) and the landed
//   push_back 0x00C29C30 (tstd::vector<CHandleBasedMemoryPool::CHandleInfo>, pool handle table at +0x43C). Element: Ego_r.pdb
//   class CHandleBasedMemoryPool::CHandleInfo (4-byte union), non-scalar, so the copies go through _Uninit_copy /
//   _Uninit_fill_n with the allocator. The CHeader* twin is 0x00C295B0 (memmove paths). libpk is built /Gr: the PDB name
//   sets spell its free STL helpers `YI` (FableWin.pdb `??$fill@PAVCHandleInfo@CHandleBasedMemoryPool@@V12@@std@@YIX...`).
// Callees: vector::_Xlen 0x00C28DC0; operator new 0x00BFEA1A / operator delete 0x00BFE9BC (allocate / deallocate);
//   std::_Uninit_copy<CHandleInfo*,CHandleInfo*,allocator<CHandleInfo> > 0x00C29340; std::_Uninit_fill_n<CHandleInfo*,
//   unsigned int,CHandleInfo,allocator<CHandleInfo> > 0x00C294B0; vector::_Ufill 0x00C29840 (second site, not inlined);
//   std::copy_backward<CHandleInfo*,CHandleInfo*> 0x00C29250; std::fill<CHandleInfo*,CHandleInfo> 0x00C28FA0;
//   _CxxThrowException 0x00BFEB84 (`_RERAISE`).

class CHandleBasedMemoryPool {
public:
    class CHeader;
    class CHandleInfo { public:                    // Ego_r.pdb: 4 bytes, a union of the block header and the free-handle link
        union { CHeader* Header; long NextFreeHandle; };
    };
};

namespace tstd {
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
    ++g_allocs; return ((_Ty*)operator new(_Count * sizeof (_Ty)));
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
    void deallocate(pointer _Ptr, size_type) { ++g_frees; operator delete(_Ptr); }
    pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
    size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof (_Ty); return (0 < _Count ? _Count : 1); }
};

struct length_error {
    explicit length_error(const char* _Message) : _What(_Message) { }
    const char* _What;
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


typedef CHandleBasedMemoryPool::CHandleInfo HI;
class CTestVec : public tstd::vector<HI> {
public:
    CTestVec() { _Myfirst = _Mylast = _Myend = 0; }
    void InsertN(unsigned int pos, unsigned int n, long v) { HI h; h.NextFreeHandle = v; _Insert_n(iterator(_Myfirst + pos), n, h); }
    void InsertSelf(unsigned int pos, unsigned int n, unsigned int from) { _Insert_n(iterator(_Myfirst + pos), n, _Myfirst[from]); }
    long At(unsigned int i) const { return _Myfirst[i].NextFreeHandle; }
    unsigned int Cap() const { return capacity(); }
};

static bool check(const CTestVec& v, const long* e, unsigned int n)
{
    if (v.size() != n) return false;
    for (unsigned int i = 0; i < n; ++i) if (v.At(i) != e[i]) return false;
    return true;
}

int main()
{
    CTestVec v;
    v.InsertN(0, 0, 9);                                   // _Count == 0: nothing
    if (v.size() != 0 || g_allocs != 0) { std::printf("00c298d0_TEST FAIL 0\n"); return 1; }
    v.InsertN(0, 2, 7);                                   // reallocate from empty: capacity 2
    { long e[] = { 7, 7 }; if (!check(v, e, 2) || v.Cap() != 2 || g_allocs != 1 || g_frees != 0) { std::printf("00c298d0_TEST FAIL 1\n"); return 1; } }
    v.InsertN(1, 1, 3);                                   // reallocate: 2 + 1 = 3 (grow by 50%)
    { long e[] = { 7, 3, 7 }; if (!check(v, e, 3) || v.Cap() != 3 || g_allocs != 2 || g_frees != 1) { std::printf("00c298d0_TEST FAIL 2\n"); return 1; } }
    v.InsertN(3, 3, 5);                                   // reallocate: max(3 + 1, 6) = 6
    { long e[] = { 7, 3, 7, 5, 5, 5 }; if (!check(v, e, 6) || v.Cap() != 6) { std::printf("00c298d0_TEST FAIL 3\n"); return 1; } }
    v.InsertN(0, 3, 1);                                   // reallocate: 6 + 3 = 9
    { long e[] = { 1, 1, 1, 7, 3, 7, 5, 5, 5 }; if (!check(v, e, 9) || v.Cap() != 9) { std::printf("00c298d0_TEST FAIL 4\n"); return 1; } }
    v.InsertN(0, 1, 0);                                   // reallocate: 9 + 4 = 13
    if (v.Cap() != 13 || v.size() != 10) { std::printf("00c298d0_TEST FAIL 5\n"); return 1; }
    v.InsertN(8, 2, 4);                                   // fits, spills nothing: hole branch (2 <= 10 - 8)
    { long e[] = { 0, 1, 1, 1, 7, 3, 7, 5, 4, 4, 5, 5 }; if (!check(v, e, 12)) { std::printf("00c298d0_TEST FAIL 6\n"); return 1; } }
    int allocs = g_allocs;
    v.InsertSelf(11, 1, 4);                               // fits, value from inside the sequence (_Tmp copy), 1 <= 1: hole
    { long e[] = { 0, 1, 1, 1, 7, 3, 7, 5, 4, 4, 5, 7, 5 }; if (!check(v, e, 13) || g_allocs != allocs) { std::printf("00c298d0_TEST FAIL 7\n"); return 1; } }
    CTestVec w;
    w.InsertN(0, 4, 2);                                   // capacity 4
    w.InsertN(0, 1, 1);                                   // capacity 6, size 5
    w.InsertN(4, 1, 8);                                   // fits: 1 <= 5 - 4 -> hole
    w.InsertN(5, 0, 8);
    { long e[] = { 1, 2, 2, 2, 8, 2 }; if (!check(w, e, 6) || w.Cap() != 6) { std::printf("00c298d0_TEST FAIL 8\n"); return 1; } }
    CTestVec s;
    s.InsertN(0, 1, 1);                                   // cap 1
    s.InsertN(1, 1, 2);                                   // cap 2
    s.InsertN(2, 1, 3);                                   // cap 3
    s.InsertN(3, 1, 4);                                   // cap 4
    s.InsertN(4, 1, 5);                                   // cap 6, size 5 {1,2,3,4,5}
    s.InsertN(4, 1, 9);                                   // fits, 1 <= 1: hole -> {1,2,3,4,9,5}
    { long e[] = { 1, 2, 3, 4, 9, 5 }; if (!check(s, e, 6) || s.Cap() != 6) { std::printf("00c298d0_TEST FAIL 9\n"); return 1; } }
    CTestVec t;
    t.InsertN(0, 1, 1); t.InsertN(1, 1, 2); t.InsertN(2, 1, 3); t.InsertN(3, 1, 4);   // cap 4, size 4
    t.InsertN(4, 1, 5);                                   // cap 6, size 5 {1,2,3,4,5}
    t.InsertN(4, 1, 6);                                   // hole: {1,2,3,4,6,5}, cap 6 size 6
    t.InsertN(6, 3, 7);                                   // cap 9 size 9 {1,2,3,4,6,5,7,7,7}
    t.InsertN(9, 4, 8);                                   // cap 13 size 13
    t.InsertN(13, 1, 8);                                  // cap 19 size 14
    t.InsertN(13, 3, 9);                                  // fits (17 <= 19), spills: 3 > 14 - 13
    { long e[] = { 1, 2, 3, 4, 6, 5, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 8 };
      if (!check(t, e, 17) || t.Cap() != 19) { std::printf("00c298d0_TEST FAIL 10\n"); return 1; } }
    t.InsertN(1, 2, 0);                                   // fits (19), hole: 2 <= 16
    { long e[] = { 1, 0, 0, 2, 3, 4, 6, 5, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 8 };
      if (!check(t, e, 19) || t.Cap() != 19) { std::printf("00c298d0_TEST FAIL 11\n"); return 1; } }
    std::printf("00c298d0_TEST PASS\n");
    return 0;
}
