// std::vector::_M_insert_overflow(CTCInventoryMap::CUIMiniMapMarker* position, const CUIMiniMapMarker& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for mini-map markers: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006182F0, 304 bytes (299 without padding), thiscall `ret 14h`. Callers: the
//   MiniMapMarkers push_back sites in CTCInventoryMap (0x00603C14 .. 0x00605466), _M_fill_insert 0x00618860 and 0x00618D05.
// Callees: malloc 0x00BFEA0E (allocate), __uninitialized_copy(.., const __false_type&) 0x00613DD0 (twice),
//   CCharString copy ctor 0x0099EC30 (the fill_len == 1 _Construct, implicit marker copy inlined),
//   __uninitialized_fill_n(.., const __false_type&) 0x00613E30, ~CCharString 0x0099EAE0 (the inlined _Destroy loop of
//   _M_clear), free 0x00BFEA14 (deallocate).
// Label correction: the catalog "Vector_ReallocateForInsert" -- this is STLport 4.6 `_M_insert_overflow`.
// Model: PDB CTCInventoryMap::CUIMiniMapMarker (28 bytes) with the IMPLICIT copy constructor (inlined into _Construct here;
//   the 0x00618860 TU calls its out-of-line COMDAT 0x00612980) and implicit destructor.
// Shape: STLport 4.6 verbatim: const-reference `(max)` (`lea ecx,[esp+24h] / jb / lea ecx,[esp+10h]`), allocate =
//   `n ? malloc(n * 28) : 0`, _M_clear = inlined _Destroy loop + `if (_M_start) free(_M_start)`. Every `__false_type()`
//   temporary is homed in the atend argument slot although atend is read later (the empty tag is never written): the
//   two tag-taking helpers are same-TU noinline bodies (auto_inline off).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                                       // 0x0099EC30
    ~CCharString();                                                              // 0x0099EAE0
};

struct C2DVector {
    float X, Y;
};

class CTCInventoryMap {
public:
    class CUIMiniMapMarker {
    public:
        long        Region;                                                      // +0x00
        CCharString UIDefName;                                                   // +0x04
        C2DVector   Position;                                                    // +0x08
        long        DefNameLength;                                               // +0x10
        long        GraphicIndex;                                                // +0x14
        float       Angle;                                                       // +0x18
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00613DD0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x00613E30
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CTCInventoryMap::CUIMiniMapMarker>::_M_insert_overflow(CTCInventoryMap::CUIMiniMapMarker*, const CTCInventoryMap::CUIMiniMapMarker&, const __false_type&, unsigned int, bool);
}