// std::vector<CTargetPoint>::_M_insert_overflow(CTargetPoint* position, const CTargetPoint& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the player-targeting target points: allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, destroy (release each thing pointer) and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008092D0, 256 bytes, thiscall `ret 14h`.
// Identity: the overflow path of the landed push_back 0x00809970 (`push 1 ; push 1 ; push &tag ; push x ; push
//   finish`) and three more NPlayerTargeting sites (0x008044C6, 0x00804615, 0x00804C94). The element is the 28-byte
//   NPlayerTargeting::CTargetPoint of the landed __uninitialized_copy 0x00806C00. The catalog's "Vector_Insert_Range" is
//   descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x00806C00 (__fastcall,
//   both copies out of line), CTargetPoint(const CTargetPoint&) 0x00806AE0 (out of line: the fill_len == 1 _Construct),
//   __uninitialized_fill_n(first, n, x, const __false_type&) 0x00806C80 (__fastcall), ~CBaseIntelligentPointer
//   0x00A01C10 (the implicit ~CTargetPoint: PThing's base destructor, called directly), free 0x00BFEA14.
// Layout (Ego_r.pdb): CTargetPoint = PThing CIntelligentPointer<CThing> +0, Index +8, TargetingPosition C3DVector +0xC,
//   FrameRejected +0x18.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim (`(max)` by reference, the old size in the `push ecx` word, the new
//   block on the dead fill_len slot, the tags and the saved finish of the _Destroy loop on the dead atend slot); the
//   two out-of-line helpers are same-TU `auto_inline(off)` + noinline template bodies (declared-only, VC7.1 gives the
//   new block its own frame word).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;                                     // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
};

class C3DVector {
public:
    float X, Y, Z;
};

namespace NPlayerTargeting {
class CTargetPoint {                                        // 28 bytes
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long                        Index;                      // +0x08
    C3DVector                   TargetingPosition;          // +0x0C
    long                        FrameRejected;              // +0x18
    CTargetPoint(const CTargetPoint& other);                // 0x00806AE0
};
}
using NPlayerTargeting::CTargetPoint;

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
                                                                  _ForwardIter __result, const __false_type&)   // 0x00806C00
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                        // 0x00806C80
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
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

template void vector<CTargetPoint>::_M_insert_overflow(CTargetPoint*, const CTargetPoint&, const __false_type&,
                                                       unsigned int, bool);
}