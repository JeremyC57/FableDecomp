// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<pair<unsigned long, C3DMeshVolume> >) STLport 4.6 insert of n copies of a mesh-volume map entry: shift the tail inside the spare capacity, else grow through the __false_type _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@KVC3DMeshVolume@@@std@@V?$allocator@U?$pair@KVC3DMeshVolume@@@std@@@2@@std@@QAEXPAU?$pair@KVC3DMeshVolume@@@2@IABU32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35010, 309 bytes, thiscall `ret 0Ch`.
// Identity (proven): STLport 4.6 vector<_Tp>::_M_fill_insert (public QAE); its only caller is the landed
//   vector<pair<unsigned long, C3DMeshVolume> >::resize(n, x) 0x00A35430 (0x00A35480); overflow callee 0x00A34F00.
//   Catalog label "Vector_Insert_Pair_16" is descriptive.
// Element (proven): std::pair<unsigned long, C3DMeshVolume> (16 bytes) -- Ego_r.pdb C3DMeshStats::MeshVolumes is
//   CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<..> > (C3DMeshVolume = CCharString Name +0,
//   CCountedPointer<CContainmentVolume> ConvexVolume +4); the copy is key, CCharString(const CCharString&) 0x0099EC30,
//   Data, Info with ++RefCount; the landed push_back 0x00A35360 / resize 0x00A35430 of the same vector and the bound
//   __copy_backward 0x00A33050 / __destroy_aux 0x00A33720 spell the same pair<unsigned long (K), C3DMeshVolume>.
// Callees: CCharString(const CCharString&) 0x0099EC30 (x_copy), __uninitialized_copy 0x00A32F40, __copy_backward<..,
//   int> 0x00A33050, fill<pair*, pair> 0x00A335E0, __uninitialized_fill_n 0x00A32FB0, the implicit
//   pair<unsigned long, C3DMeshVolume>::~pair 0x00A326D0 (x_copy, out of line), _M_insert_overflow 0x00A34F00.
// Shape: STLport 4.6 vector.c verbatim; pair has STLport's user copy constructor (inline), C3DMeshVolume's copy is
//   implicit. CCountedPointer's destructor carries its inline release (as the landed ~pair 0x00A326D0 body shows):
//   with a declared-only ~CCountedPointer VC7.1 inlines the small implicit ~pair and tail-merges the two arms; with
//   the real inline release ~pair stays an out-of-line call and both arms keep their own call + epilogue, as retail.
//   The four helpers get same-TU noinline bodies (same retail TU); at landing the retail COMDATs win.
inline void* operator new(unsigned int, void* p) { return p; }

void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo { public:                          // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other);
};

class CContainmentVolume;

class C3DMeshVolume {                                   // 0xC bytes (Ego_r.pdb)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                   // 0x00A32F40
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                   // 0x00A32FB0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                   // 0x00A33050
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                   // 0x00A335E0
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00A34F00
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
} // namespace std

template void std::vector<std::pair<unsigned long, C3DMeshVolume> >::_M_fill_insert(std::pair<unsigned long, C3DMeshVolume>*, unsigned int,
                                                                                const std::pair<unsigned long, C3DMeshVolume>&);