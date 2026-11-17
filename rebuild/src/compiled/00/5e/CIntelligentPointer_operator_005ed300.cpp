// CIntelligentPointer<const CThing>::operator=(const CIntelligentPointer<const CThing>& other) -- SetPItem(other.GetPItem()); return *this
// Leaf: ??4?$CIntelligentPointer@$$CBVCThing@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED300, thiscall `ret 4`: GetPItem 0x00A01B50 on the source, SetPItem 0x00A01B90 on this,
//   return this.
// Identity (agentNF54 name fix; was the stand-in non-template CIntelligentPointer::operator=(CIntelligentPointer*) returning a
//   pointer, ??4CIntelligentPointer@@QAEPAV0@PAV0@@Z, "pointee unproven"): FableWin.pdb 0x02057E90
//   ??4?$CIntelligentPointer@$$CBVCThing@@@@QAEAAV0@ABV0@@Z. Each of its three retail call sites (0x0085A332, 0x00957FD2 IsRunnable of
//   CAIStateGroup_MinionReactToBurglary, 0x00960E22 IsRunnable of CAIStateGroup_DeedReactionRoot) assigns from a temporary built just
//   before by 0x0056C860, the CIntelligentPointer<const CThing>(const CThing*) constructor (its stored vtable's RTTI descriptor is
//   .?AV?$CIntelligentPointer@$$CBVCThing@@@@; Ego_r.pdb 0x005A6190 + FableWin.pdb 0x01F2AE40), so the operator is that instance's.
// Shape: engine/CIntelligentPointer.h's retail operator= body (FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN), explicitly instantiated.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
class CThing;

template CIntelligentPointer<const CThing>& CIntelligentPointer<const CThing>::operator=(const CIntelligentPointer<const CThing>& other);
