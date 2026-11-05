// std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >::pair(const std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >& other) -- copy constructor of the (binding name, entity script binding) record: out-of-line string copy, add-ref copy of the counted pointer
// Leaf: ??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members m_count->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB9C50, length 39 bytes.
// Identity (naming wave 20, chunk 686): FableWin.pdb ??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z;
//   the landed __introsort_loop 0x00CBDAD0 (chunk 686), __median 0x00CBA620, swap 0x00CBA8E0, __partial_sort 0x00CBD0F0, __linear_insert
//   0x00CBC3C0 and __insertion_sort 0x00CBCF70 of the CScriptManager binding table all call it under this spelling. Was the descriptive
//   SScriptBindingPair (the manifest "CScriptGameResourceObjectScriptedThingBase" is propagated).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (on +0, `other` pushed).
// Shape: no vtable store anywhere, so the record is not polymorphic: the first member is a CCharString copied out of line, the second
//   a counted pointer {Data +4, Info +8} copied by its inline copy constructor with the count block add-ref'd in place (`inc dword ptr [eax]`).
#include "engine/CCountedPointer.h"
class CEntityScriptBindingBase;
class CCharString { public:
    char* m_pData;
    CCharString(const CCharString& other);            // 0x0099EC30
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                        // +0x00 binding name
    _T2 second;                                       // +0x04 Data / +0x08 Info
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template struct pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >;
}
