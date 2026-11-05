// std::sort_heap(SBindingPair* first, SBindingPair* last, CCompareString comp) -- SGI sort_heap over (name, CCountedPointer<CEntityScriptBindingBase>) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBC580, length 130 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32>>*,...>` is
// propagated over the byte-identical skeleton. The only non-string callee is the landed SGI `__pop_heap` @ 0x00CBBAD0
// of pair<CCharString, CCountedPointer<CEntityScriptBindingBase>> under CCompareString (stride 12); element model reused.
// Callees: __pop_heap @ 0x00CBBAD0 (out of line, __fastcall, `ret 18h`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the name string, then the
// counted pointer's inline copy (both words, `test ; je ; inc [count]`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

#include "engine/CCountedPointer.h"


class CEntityScriptBindingBase;

struct SBindingPair {                                   // std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >, 12 bytes
    CCharString                               first;    // +0x00
    CCountedPointer<CEntityScriptBindingBase> second;   // +0x04
    SBindingPair& operator=(const SBindingPair& other);   // 0x00CBA700 (out of line)
};

struct CCompareString { };                              // CCompareString<CCountedPointer<CEntityScriptBindingBase> >, empty, by value

void __fastcall __pop_heap(SBindingPair* first, SBindingPair* last, SBindingPair* result, SBindingPair value,
                           CCompareString comp, long*);   // 0x00CBBAD0

inline void __pop_heap_aux(SBindingPair* first, SBindingPair* last, SBindingPair*, CCompareString comp)
{
    __pop_heap(first, last - 1, last - 1, SBindingPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SBindingPair* first, SBindingPair* last, CCompareString comp)
{
    __pop_heap_aux(first, last, (SBindingPair*)0, comp);
}

void __fastcall sort_heap(SBindingPair* first, SBindingPair* last, CCompareString comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}