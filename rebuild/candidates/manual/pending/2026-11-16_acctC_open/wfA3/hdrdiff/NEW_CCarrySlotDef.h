#ifndef FABLE_ENGINE_CCARRYSLOTDEF_H
#define FABLE_ENGINE_CCARRYSLOTDEF_H
// New shared header (agentC32, row 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation; the generated one sits in
// engine/_quarantine/): Ego_r.pdb CCarrySlotDef, 0x38 bytes (struct_layouts_egor.tsv: DummyPosName CDefString +0x28, DummyPosIndex
// long +0x2C, PrimarySlot bool +0x30, SecondaryDummyPosName CDefString +0x34; RECONCILE.tsv agrees from the landed Transfer
// 0x00452256), a global-index def (CDefinitionManager::GetDef<CCarrySlotDef> 0x006A3060). Retail reads +0x28 (GetString) and +0x34
// (operator!= / GetString) at 0x006A1A85 / 0x006A1BD3 / 0x006A1C57. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
class CPersistContext;
class CCarrySlotDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext&);               // Ego_r ?Transfer@CCarrySlotDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                // Ego_r ?Copy@CCarrySlotDef@@UAEXPBVCDefClassBase@@@Z
    CDefString DummyPosName;                                 // +0x28
    long       DummyPosIndex;                                // +0x2C
    bool       PrimarySlot;                                  // +0x30
    CDefString SecondaryDummyPosName;                        // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CCarrySlotDef) == 0x38);
#endif
