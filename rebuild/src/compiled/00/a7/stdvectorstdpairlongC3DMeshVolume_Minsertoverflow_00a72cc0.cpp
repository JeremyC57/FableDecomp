// std::vector<std::pair<long, C3DMeshVolume> >::_M_insert_overflow(std::pair<long, C3DMeshVolume>* position, const std::pair<long, C3DMeshVolume>& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the long-keyed mesh-volume map entries (16 bytes): move into a size + max(size, fill_len) block with x inserted fill_len times at position
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JVC3DMeshVolume@@@std@@V?$allocator@U?$pair@JVC3DMeshVolume@@@std@@@2@@std@@QAEXPAU?$pair@JVC3DMeshVolume@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A72CC0, 258 bytes, thiscall `ret 14h` (a /Gr TU: the helpers are __fastcall).
// Identity: callers CPhysicsMesh::GetContainmentDataFromFileData (0x00A6D7EF) and the landed push_back 0x00A734D0, itself
//   called only by 0x00A737B0 (catalogued "C3DAnimFile::AddKeyedChunk", propagated: it is the map's Add, setting the Dirty
//   byte +0x0D); the 16-byte element is CPhysicsMesh::NavigationContainmentVolumes' entry, Ego_r.pdb
//   CVectorMap<long, C3DMeshVolume, CKeyPairCompareLess<long, C3DMeshVolume> > (C3DMeshVolume = CCharString Name +
//   CCountedPointer<CContainmentVolume> ConvexVolume), matching the landed copy helper label
//   "UninitCopy_PairULongMeshVolume" 0x00A6FE00. The binding spelling (pair<long, SNamedChunk>) is a stand-in.
//   Catalog label "std_Vector_Reallocate_Refcounted" is descriptive.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (allocator), __uninitialized_copy 0x00A6FE00 (x2), __uninitialized_fill_n
//   0x00A6FE70, CCharString copy ctor 0x0099EC30 (the fill_len == 1 construct: pair's copy ctor, C3DMeshVolume's
//   memberwise user copy, the inline counted-pointer copy `inc [Info]`), __destroy_aux 0x00A70E80 (the _M_clear destroy,
//   out of line).
// Shape: the landed 0x00624B90 STLport `_M_insert_overflow` model with an out-of-line `__destroy_aux`.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                         // 0x0099EC30
    ~CCharString();                                                // 0x0099EAE0
};

class CCPPointerInfo {
public:
    long RefCount;                                                 // +0x00
};

template <class T> class CCountedPointer {                         // 8 bytes
public:
    T*              Data;                                          // +0x00
    CCPPointerInfo* Info;                                          // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer();
};

class CContainmentVolume;

class C3DMeshVolume {                                              // 12 bytes (Ego_r.pdb)
public:
    CCharString                         Name;                      // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;              // +0x04
    C3DMeshVolume(const C3DMeshVolume& o) : Name(o.Name), ConvexVolume(o.ConvexVolume) { }   // FableWin.pdb ??0C3DMeshVolume@@QAE@ABV0@@Z
    ~C3DMeshVolume();                                              // FableWin.pdb ??1C3DMeshVolume@@QAE@XZ
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{                                                                  // 0x00A70E80
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                  // 0x00A6FE00
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                  // 0x00A6FE70
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<pair<long, C3DMeshVolume> >::_M_insert_overflow(pair<long, C3DMeshVolume>*,
    const pair<long, C3DMeshVolume>&, const __false_type&, unsigned int, bool);

}