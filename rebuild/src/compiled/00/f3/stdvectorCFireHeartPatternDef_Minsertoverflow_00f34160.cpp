// std::vector<CFireHeartPatternDef>::_M_insert_overflow(pointer position, const CFireHeartPatternDef& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert of the Fire Heart pattern defs: allocate size + max(size, fill_len) elements, copy-construct the head, x (once, or fill_len times) and the tail unless atend, destroy (virtual destructors) and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@IAEXPAVCFireHeartPatternDef@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F34160, 330 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): element CFireHeartPatternDef -- 0x34 stride (0x4EC4EC4F reciprocal, `imul ecx,ecx,34h`); the per-element
//   copies call _Construct 0x00D34EC0 (bound ??$_Construct@VCFireHeartPatternDef@@V1@@std@@YIX... by the landed
//   vector<CFireHeartPatternDef>::_M_allocate_and_copy 0x00D35020) and the inline single copy runs the CDefClassBase copy
//   constructor 0x0046644C, stores the CFireHeartPatternDef vtable 0x012CCC50 (slot 18 Transfer 0x00D35070, slot 19 Copy
//   0x00D34FB0, slot 20 GetSizeofClass 0x00D34EF0) and copies PadActive (vector<bool> copy ctor 0x00D34F00) at +0x28.
//   Only caller 0x00F340FD (the vector's fill insert). STLport 4.6 protected member (IAE), as the landed
//   vector<CArenaRoundDef> / vector<CConversation> / vector<CFeatDef> rows. Catalog "vector_reallocate_insert_ThingBuildingDef_map" is wrong.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, _Construct 0x00D34EC0 (__fastcall, out of line in the three loops),
//   CDefClassBase::CDefClassBase(const CDefClassBase&) 0x0046644C, vector<bool>::vector(const vector&) 0x00D34F00, the element's
//   virtual destructor (vtable slot 0, flags 0).
// Shape: STLport 4.6 vector.c _M_insert_overflow with _M_clear's _Destroy loop inlined (`__forceinline` _Destroy / __destroy_aux
//   / _M_clear). Retail calls _Construct out of line inside the __uninitialized_copy / __uninitialized_fill_n loops but expands
//   the fill_len == 1 construct in place (the landed sibling vector<CArenaRoundDef> 0x00F34FB0 has the same split): the loops'
//   _Construct is the out-of-line __fastcall declaration and the single construct is written as its placement-new body.
//   A plain inline _Construct inlines every copy (399v330); `#pragma inline_depth(1)` also stops the destroy loop (325v330).
// Layout: the retail polymorphic def-class chain from the shared header (FABLE_RETAIL_DEF_CLASS_BASE: CDefClassBase :
//   CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes); CFireHeartPatternDef has no user ctor/dtor in either PDB.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { };
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
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    vector(const vector& __x);                          // vector<bool>: 0x00D34F00
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
}   // namespace std

class CFireHeartPatternDef : public CDefClassBase {     // 0x34 bytes, vtable 0x012CCC50 (no RTTI locator)
public:
    virtual void Transfer(CPersistContext&);            // slot 18 0x00D35070
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00D34FB0
    virtual unsigned long GetSizeofClass();             // slot 20 0x00D34EF0
    std::vector<bool> PadActive;                        // +0x28 (STLport, 12 bytes)
};

namespace std {
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);           // 0x00D34EC0
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc>
__forceinline void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

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
        new ((void*)__new_finish) _Tp(__x);             // _Construct(__new_finish, __x), expanded in place
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CFireHeartPatternDef>::_M_insert_overflow(CFireHeartPatternDef*, const CFireHeartPatternDef&, const __false_type&, unsigned int, bool);
}   // namespace std