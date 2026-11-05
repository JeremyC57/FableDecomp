// _global::__pop_heap(SScriptPair* first, SScriptPair* last, SScriptPair* result, SScriptPair value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (UID, CCountedPointer<CActiveEntityScriptBase>) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBBA60, length 104 bytes (`ret 1Ch`: result, the 0x10-byte value, the comparator
// and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the catalog's "CTCCreatureModeManager::AddMode" is propagated and wrong. The callee 0x00CBB3B0 carries the
// manifest name `_Adjust_heap<std::pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> >*,int,...,CKeyPairCompareLess<...>>`,
// so this is the matching __pop_heap (sibling of the landed 0x00CBBAD0 for the CCharString-keyed pairs).
// Callees: the pair's assignment @ 0x00CBA520 (landed SCountedRecord_00cba520::operator=), __adjust_heap @ 0x00CBB3B0, the count
// block's destroy function (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC.
// Shape: the pair's copy ctor is implicit (two key words, then the counted pointer's inline copy ctor counting the block up);
// `last - first` divides by 16 with a plain `sar 4`; the by-value `value` is released by this callee with the block kept in esi
// (`--count == 0` test, no zero stores).
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
struct SScriptPair {                                    // std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >, 16 bytes
    unsigned __int64                         first;     // +0x00
    CCountedPointer<CActiveEntityScriptBase> second;    // +0x08
    SScriptPair& operator=(const SScriptPair& other);   // 0x00CBA520 (out of line)
};
struct CKeyPairCompareLess { };                         // CKeyPairCompareLess<unsigned __int64, CCountedPointer<...> >, empty, by value

void __fastcall __adjust_heap(SScriptPair* first, long holeIndex, long len, SScriptPair value,
                              CKeyPairCompareLess comp); // 0x00CBB3B0

void __fastcall __pop_heap(SScriptPair* first, SScriptPair* last, SScriptPair* result, SScriptPair value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}