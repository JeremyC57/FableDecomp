// std::vector::_M_insert_overflow(CMap::CScriptedMapBrushUpdateArea* position, const CMap::CScriptedMapBrushUpdateArea& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the map's scripted-brush update areas (timer + brush name): allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00826450, 249 bytes, thiscall `ret 14h`.
// Identity: callers CMap::ApplyScriptedMapBrush 0x00820B80 (landed; its inline push_back calls this address as the
//   overflow path) and the out-of-line push_back 0x008268F0; both operate on CMap::ScriptedMapBrushUpdateAreas (retail
//   +0x10, Ego_r.pdb vector<CMap::CScriptedMapBrushUpdateArea>). The catalog's "StdVector_ReallocateAndReserve" is
//   descriptive. Same body shape as the landed pair<int,CCharString> instantiation 0x009CAD20.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x00824650 (__fastcall,
//   `ret 8`, called for the head and the tail), __uninitialized_fill_n(first, n, x, const __false_type&) 0x00824690,
//   CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout: CMap::CScriptedMapBrushUpdateArea, 8 bytes: float Timer +0, CCharString Name +4 (the landed 0x00820B80 model).
// Shape: STLport 4.6 `_M_insert_overflow` verbatim: `(max)(__old_size, __fill_len)` by reference (the old size gets the
//   `push ecx` frame word), allocate inline (`n != 0 ? malloc(n * 8) : 0`, the block homed on the fill_len slot once fill_len sits in edi),
//   both __uninitialized_copy calls and __uninitialized_fill_n out of line (same-TU noinline bodies that never touch the
//   tag, so the tag temporaries share the `atend` slot), the single-element _Construct inline, the _Destroy loop inline
//   (the element's implicit dtor = ~CCharString on +4), `if (p) free(p)`, then _M_set.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

class CMap {
public:
    class CScriptedMapBrushUpdateArea {              // 8 bytes
    public:
        float       Timer;                           // +0x00
        CCharString Name;                            // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

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
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CMap::CScriptedMapBrushUpdateArea>::_M_insert_overflow(
    CMap::CScriptedMapBrushUpdateArea*, const CMap::CScriptedMapBrushUpdateArea&, const __false_type&, unsigned int,
    bool);
}