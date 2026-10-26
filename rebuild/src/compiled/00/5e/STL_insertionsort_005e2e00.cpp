// std::__insertion_sort(SPriorityEntry* first, SPriorityEntry* last, CContextSensitivePriorityCompare comp) -- STLport insertion sort over (priority, CDefPointer<const CContextSensitiveItemDef>) pairs: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2E00, 69 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x005E26E0 __linear_insert(first, next, value, pred) (landed as global_InsertSortedEntry005e26e0; catalogued as
// the propagated CHitParameters::SetPHitMaterialDef).
// Label: catalogued as `_Insertion_sort1<pair<long, CDefPointer<CContextSensitiveItemDef const> >*,
// CContextSensitivePriorityCompare>` (Dinkumware name); the code is STLport's `__insertion_sort`.
// Shape: STLport's pair copy constructor plus CDefPointer's add-ref copy constructor build the by-value `*i` in the
// argument area (`sub esp,8 ; mov ecx,esp`): the priority dword, then the pointee pointer with `inc [eax+4]`
// (CIVCountedPointeeBase::IVRefCount) behind a null test. Same model as the landed callee.
#include "engine/CIVCountedPointeeBase.h"

struct CDefPointer {                      // CDefPointer<const CContextSensitiveItemDef>: one counted pointee
    CIVCountedPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct SPriorityEntry {                   // std::pair<long, CDefPointer<const CContextSensitiveItemDef> >
    long        first;                    // +0x00 priority
    CDefPointer second;                   // +0x04
    SPriorityEntry(const SPriorityEntry& o) : first(o.first), second(o.second) { }
};

class CContextSensitivePriorityCompare { public: // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SPriorityEntry* first, SPriorityEntry* last, SPriorityEntry value, CContextSensitivePriorityCompare comp);   // 0x005E26E0

void __fastcall __insertion_sort(SPriorityEntry* first, SPriorityEntry* last, CContextSensitivePriorityCompare comp)
{
    if (first == last)
        return;
    for (SPriorityEntry* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}