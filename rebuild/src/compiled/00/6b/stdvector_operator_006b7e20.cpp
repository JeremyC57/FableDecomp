// std::vector::operator=(const vector<CGameMapCell>& x) -- STLport assignment of the 7-byte game-map cell vector: reallocate and copy when x outgrows the capacity, else assign over the live cells and copy-construct the rest; returns *this
// Leaf: ??4?$vector@UCGameMapCell@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7E20, 336 bytes (+ int3 padding), thiscall, `ret 4`, no EH frame.
// Identity: the catalog's "Vector_CopyAssign" is a descriptive auto label. The element is CGameMapCell (Ego_r.pdb:
//   AtmosThemes uchar[3], AtmosBlends uchar[2], SoundTheme schar, Music uchar; 7 bytes, alignment 1 -- the `/7`
//   reciprocal 0x92492493 and `imul ..,7`), the vector<CGameMapCell> of CGameMap; the only caller is 0x006B7729.
// Callees: malloc 0x00BFEA0E (_M_allocate_and_copy's allocate), __uninitialized_copy(first, last, result,
//   const __false_type&) 0x006B7B70 (twice), __copy(first, last, result, const random_access_iterator_tag&, int*)
//   0x006B79A0 (twice, the __copy_ptrs of the non-POD cell), free 0x00BFEA14 (_M_clear's deallocate). Both helpers are
//   landed `__fastcall` rows of the /Gr TU.
// Shape: STLport 4.6 vector.c `operator=` verbatim: `xlen > capacity()` -> `_M_allocate_and_copy` (allocate null test +
//   __uninitialized_copy) + `_M_clear()` (the trivial destroy vanishes, null-tested free) + start/end-of-storage; `size() >=
//   xlen` -> __copy_ptrs + the empty _Destroy; else __copy_ptrs over size() cells and __uninitialized_copy of the rest
//   (size() re-read after the call); then `finish = start + xlen`. VC7.1 hoists the common `push 0` (the null distance
//   pointer) of both __copy calls above the `jb`. The tag-taking callees are same-TU noinline bodies that never touch the
//   tag, so every empty tag is homed in the live x argument slot (`lea ..,[esp+18h]` / `[esp+1Ch]`) as retail does.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct CGameMapCell {                                       // 7 bytes, alignment 1
    unsigned char AtmosThemes[3];                           // +0
    unsigned char AtmosBlends[2];                           // +3
    signed char   SoundTheme;                               // +5
    unsigned char Music;                                    // +6
    CGameMapCell() { }
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

// Same-TU stand-ins for 0x006B7B70 / 0x006B79A0 (their own landed rows), kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                  // 0x006B7B70
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                  // 0x006B79A0
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }

    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
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
                                      (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CGameMapCell>& vector<CGameMapCell>::operator=(const vector<CGameMapCell>&);

} // namespace std