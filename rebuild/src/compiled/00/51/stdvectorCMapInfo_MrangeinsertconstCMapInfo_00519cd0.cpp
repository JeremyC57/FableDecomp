// std::vector<CMapInfo>::_M_range_insert<const CMapInfo*>(CMapInfo* pos, const CMapInfo* first, const CMapInfo* last, const forward_iterator_tag&) -- STLport vector insert(pos, first, last): shift the tail in place when capacity allows, else reallocate to size + max(size, n) and copy prefix, range and suffix
// Leaf: ??$_M_range_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00519CD0, 480 bytes, thiscall `ret 10h` (pos, first, last, the tag reference).
//   Callers: the inline insert(pos, first, last) wrapper 0x0051AB88 (jmp) and 0x0051B404 / 0x0051B5D0 (the CWorldMap
//   Maps array). The catalog label "Vector_InsertRange_Optimized" is descriptive.
// Callees: __uninitialized_copy @ 0x00513830 (CMapInfo copy constructor 0x005110B0 inside), __copy_backward @ 0x00513D40,
//   __copy @ 0x005150E0 (copy() reduced through __copy_aux / __copy_ptrs), malloc @ 0x00BFEA0E, __destroy_aux
//   @ 0x00515250, free @ 0x00BFEA14.
// Layout: CMapInfo (Ego_r.pdb, 72 bytes = `0x38E38E39 ; sar 4`, `lea [n+n*8] ; shl 3`); its copy constructor, assignment
//   and destructor are out of line, so only the size matters here.
// Shape: STLport 4.6 member-template `_M_range_insert(pos, first, last, const forward_iterator_tag&)` verbatim
//   (distance, the in-place branch with `advance(mid, elems_after)`, the reallocation with three uninitialized copies).
//   Every helper retail calls is a same-TU noinline body under `auto_inline(off)` that never touches its tag, so VC7.1
//   homes all the tag temporaries on the (still live) `first` argument slot, as retail does.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CMapInfo {                                      // 72 bytes (Ego_r.pdb member names; opaque member types)
public:
    long             Area[4];                         // +0x00 C2DBoxI
    void*            PMap[2];                         // +0x10 CCountedPointer<CMap>
    void*            LevelName;                       // +0x18 CCharString
    void*            LevelScriptName;                 // +0x1C CCharString
    unsigned long    MapUID;                          // +0x20
    bool             Used;                            // +0x24
    bool             Placed;                          // +0x25
    bool             Activated;                       // +0x26
    bool             Loading;                         // +0x27
    bool             LoadedOnPlayerProximity;         // +0x28
    void*            SerialiseScript;                 // +0x2C CCharString
    unsigned __int64 LevStamp;                        // +0x30
    unsigned __int64 TngStamp;                        // +0x38
    bool             IsSea;                           // +0x40
    CMapInfo(const CMapInfo& other);                  // 0x005110B0
    ~CMapInfo();                                      // (out of line)
    CMapInfo& operator=(const CMapInfo& other);       // (out of line)
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00513830
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x00513D40
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x005150E0
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                 // 0x00515250
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIterator, class _Distance>
inline void distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last, _Distance& __n)
{
    __n += __last - __first;
}

template <class _RandomAccessIterator, class _Distance>
inline void advance(_RandomAccessIterator& __i, _Distance __n)
{
    __i += __n;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }

    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last,
                         const forward_iterator_tag&)
    {
        if (__first != __last) {
            size_type __n = 0;
            distance(__first, __last, __n);

            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                    copy(__first, __last, __position);
                } else {
                    _ForwardIterator __mid = __first;
                    advance(__mid, __elems_after);
                    __uninitialized_copy(__mid, __last, this->_M_finish, _IsPODType());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            } else {
                const size_type __old_size = size();
                const size_type __len = __old_size + (max)(__old_size, __n);
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, _IsPODType());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, _IsPODType());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, _IsPODType());
                _Destroy(this->_M_start, this->_M_finish);
                this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
                this->_M_start = __new_start;
                this->_M_finish = __new_finish;
                this->_M_end_of_storage._M_data = __new_start + __len;
            }
        }
    }
};

template void vector<CMapInfo>::_M_range_insert<const CMapInfo*>(CMapInfo*, const CMapInfo*, const CMapInfo*,
                                                                 const forward_iterator_tag&);
}