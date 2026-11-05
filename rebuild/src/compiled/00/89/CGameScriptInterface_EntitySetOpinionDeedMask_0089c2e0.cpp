// CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing& thing, const CCharString& mask) const -- when the script thing's thing exists and has a CTCCreatureOpinionOfHero (interface 0x4F), pass the mask to its SetOpinionDeedMask
// Leaf: ?EntitySetOpinionDeedMask@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089c2e0, 77 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 613; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?EntitySetOpinionDeedMask@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z; its callee 0x006822E0 is bound ?SetOpinionDeedMask@CTCCreatureOpinionOfHero@@QAEXABVCCharString@@@Z (Ego_r.pdb); CTCCreatureOpinionOfHero::GetInterfaceType() is 79 (0x4F, landed).
// Callees: CScriptThing virtual slot 11 (+0x2C, GetThing), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::
//   LowerBound(const ETCInterfaceType&) 0x0040F020, CTCCreatureOpinionOfHero::SetOpinionDeedMask(const CCharString&) 0x006822E0.
// Body: `CThing* t = thing.GetPThing(); if (t && t->GetInterface(&opinion, 0x4F)) opinion->SetOpinionDeedMask(mask);` (the inlined GetInterface: the component lands in eax, then ecx) -- the
//   presence test (bit 15 of bitset word 2) (`test ch,ch ; jns`), the lookup key homed in the dead thing-argument slot.
// Header debt: engine/CScriptThing.h, CThing and engine/CTCCreatureOpinionOfHero.h are flat layouts; modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString;
class CTCCreatureOpinionOfHero {
public:
    void SetOpinionDeedMask(const CCharString& mask);                             // 0x006822E0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing& thing, const CCharString& mask) const
{
    CThing* t = thing.GetPThing();
    CTCCreatureOpinionOfHero* opinion;
    if (t && t->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion))
        opinion->SetOpinionDeedMask(mask);
}