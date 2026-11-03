#ifndef FABLE_ENGINE_CTHINGBUILDING_H
#define FABLE_ENGINE_CTHINGBUILDING_H
#include "engine/CThingGameObject.h"
// Ego_r TPI inheritance, members and access; native IsPointInside 0082d000.
class CThingBuilding : public CThingGameObject {
protected:
    long ForceOpenCount;
#ifdef FABLE_RETAIL_CTHINGBUILDING
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854 CTCVillage::AddThingToVillage): Ego_r.pdb types the member `bool:1`
    // (struct_layouts_egor.tsv CThingBuilding +0xEC bool:1), and retail sets it with `or byte ptr [esi+0D4h],1`
    // (0x0065BC42) through FableWin.pdb's inline ?SetOverrideAutomaticLocking@CThingBuilding@@QAEX_N@Z (0x0215ED7D, emitted
    // in the CTCVillage TU right after AddThingToVillage). Same offset and size as the default bool.
    bool OverrideAutomaticLocking : 1;
#else
    bool OverrideAutomaticLocking;
#endif
public:
    static long __fastcall StatGetType() { return 3; }
#ifdef FABLE_RETAIL_CTHINGBUILDING
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingBuilding@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
    bool IsPointInside(const C3DVector&) const;
#ifdef FABLE_RETAIL_CTHINGBUILDING
    void SetOverrideAutomaticLocking(bool value) { OverrideAutomaticLocking = value; }   // FableWin ?SetOverrideAutomaticLocking@CThingBuilding@@QAEX_N@Z (inline)
    void LockAllDoors(bool lock, bool immediately);                  // 0x0082DB00 ?LockAllDoors@CThingBuilding@@QAEX_N0@Z (Ego_r / FableWin)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingBuilding)==0xd8);
#endif
