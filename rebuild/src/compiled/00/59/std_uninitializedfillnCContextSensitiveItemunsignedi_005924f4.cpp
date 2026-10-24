// std::__uninitialized_fill_n<CContextSensitiveItem*, unsigned int, CContextSensitiveItem>(first, n, x, const __false_type&) -- copy-construct n context-sensitive item records from one value into raw storage
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x005924F4, 32 bytes, `ret 8` (`/Gr` `__fastcall`: first in ecx, n in edx, the
// value and the empty `__false_type` reference on the stack; returns the end of the filled range in eax).
// Only caller: vector<CTCInventory::CContextSensitiveItem>::_M_insert_overflow @ 0x00593F39 (the several-element
// arm; one element is the inline `_Copy_Construct`). The 12-byte record (PDB CTCInventory::CContextSensitiveItem
// {ThingDefIndex, ButtonIndex, Priority}) is the static ContextSensitivePreferredItems list @ 0x013B8B40.
// Label: the catalog "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is a propagated Dinkumware spelling: the
// body is STLport 4.x `__uninitialized_fill_n` for a class element (STLport has no partial specialisation of
// the POD traits here, so every class is `__false_type`): `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`
// -- the unsigned `n > 0` test is `test edx,edx ; jbe`, the placement new is the `test eax,eax` null test around the
// three `movsd`, and the trivial destructor drops the `_STLP_UNWIND` cleanup.
// Shape: size TU (`#pragma optimize("s", on)`), frameless.
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

struct CContextSensitiveItem {                       // CTCInventory::CContextSensitiveItem, 12 bytes (Ego_r.pdb)
    long ThingDefIndex;                              // +0x00
    long ButtonIndex;                                // +0x04
    long Priority;                                   // +0x08
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CContextSensitiveItem* __fastcall __uninitialized_fill_n(CContextSensitiveItem*, size_t,
                                                                  const CContextSensitiveItem&, const __false_type&);
}