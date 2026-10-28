// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<pair<unsigned long, C3DMeshVolume> >) STLport 4.6 growth insert: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@KVC3DMeshVolume@@@std@@V?$allocator@U?$pair@KVC3DMeshVolume@@@std@@@2@@std@@IAEXPAU?$pair@KVC3DMeshVolume@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34F00, 258 bytes, thiscall `ret 14h`.
// Identity (proven): STLport 4.6 vector<_Tp>::_M_insert_overflow(.., const __false_type&, ..), protected (IAE); callers
//   are this vector's _M_fill_insert 0x00A35010 and the landed push_back 0x00A35360 (which already binds 0x00A34F00
//   under this name), plus 0x00A2FDC6 (C3DMeshStats loading). Catalog label "Vector_Reallocate_Pair" is descriptive.
// Element (proven): std::pair<unsigned long, C3DMeshVolume> (16 bytes) -- Ego_r.pdb C3DMeshStats::MeshVolumes is
//   CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<..> > (C3DMeshVolume = CCharString Name +0,
//   CCountedPointer<CContainmentVolume> ConvexVolume +4); the copy is key, CCharString(const CCharString&) 0x0099EC30,
//   Data, Info with ++RefCount; the landed push_back 0x00A35360 / resize 0x00A35430 of the same vector and the bound
//   __copy_backward 0x00A33050 / __destroy_aux 0x00A33720 spell the same pair<unsigned long (K), C3DMeshVolume>.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy<pair*, pair*> 0x00A32F40 (head and tail),
//   __uninitialized_fill_n<pair*, unsigned int, pair> 0x00A32FB0, __destroy_aux<pair*> 0x00A33720 (_M_clear).
// Shape: STLport 4.6 vector.c verbatim; the single fill copy is the inline pair copy. The three helpers sit in this
//   retail TU and get same-TU noinline bodies (STLport 4.6 text) so the tags share the atend slot and new_start is
//   homed in the dead fill_len slot, as in 0x00A3D060. At landing the retail COMDATs win.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

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
    ~CCountedPointer();
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
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
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
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                   // 0x00A33720
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
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
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std

template void std::vector<std::pair<unsigned long, C3DMeshVolume> >::_M_insert_overflow(std::pair<unsigned long, C3DMeshVolume>*,
    const std::pair<unsigned long, C3DMeshVolume>&, const std::__false_type&, unsigned int, bool);