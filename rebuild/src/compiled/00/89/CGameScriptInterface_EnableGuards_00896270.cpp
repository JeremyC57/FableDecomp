// CGameScriptInterface::EnableGuards(const CScriptThing& village, bool enable) const -- script call: forwards to CTCVillage::EnableGuards on a live village thing
// Leaf: ?EnableGuards@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00896270, 85 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 443 holds this body; slot 443 is
//   ?EnableGuards@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (ghidra_out/cgsi_slots_aligned.tsv). Callee 0x0065C45A
//   is ?EnableGuards@CTCVillage@@QAEX_N@Z (both PDBs; the manifest labels it CTCVillage::EnableGuards).
// Body: t = thing.GetPThing() (CScriptThing slot 11, +0x2c, no validity check); if t, not dead (+0x91 bit 0) and holding
//   interface 0x22 (UsingComponents word 1 bit 2, `test byte [eax+24h],4`): GetTC through the non-const LowerBound
//   0x0040F020 (key in the dead `thing` argument slot), then the CTCVillage call. `this` is unused.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString;
class CTCVillage {                                      // interface 0x22 (CTCVillage::GetInterfaceType not landed; the PDB names of the callee pin the class)
public:
    unsigned char _pad00[0xd4];
    long HeroEnemyOfGuardsUntil;                         // +0xd4 (descriptive: the frame IncreaseHeroEnemyOfGuardsUntil extends)
    void SetVillageLimbo(bool limbo);                    // 0x0065C36E
    void EnableGuards(bool enable);                      // 0x0065C45A
    void EnableVillagerDefTypes(bool enable, const CCharString& defType);   // 0x0065B142
    void ClearHeroEnemyOfGuards() { HeroEnemyOfGuardsUntil = 0; }
};
class CGameScriptInterface {
public:
    virtual void EnableGuards(const CScriptThing& village, bool enable) const;
};
void CGameScriptInterface::EnableGuards(const CScriptThing& village, bool enable) const
{
    CThing* t = village.GetPThing();
    CTCVillage* tc;
    if (t && !t->IsDead() && t->GetTC(TCI_VILLAGE, &tc))
        tc->EnableGuards(enable);
}