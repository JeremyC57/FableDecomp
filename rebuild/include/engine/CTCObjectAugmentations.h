#ifndef FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
#define FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
// New shared header (chunk 1002, CTCInventoryWeapons rows 0x005C4671 / 0x005CB2B0): the retail CTCObjectAugmentations over
// FABLE_RETAIL_CTCBASE (RTTI .?AVCTCObjectAugmentations@@ : CTCBase). Members are not modelled; only what the rows call, in the
// Ego_r.pdb / FableWin.pdb spellings: ?GetFreeSlot@CTCObjectAugmentations@@QBEJXZ (retail 0x00765D50),
// ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z (0x00768120), the static __fastcall ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@JV?$allocator@J@std@@@std@@@Z
// (0x007661C0) and ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z (0x00765F90). StatGetInterfaceType: TCI_OBJECT_AUGMENTATIONS (191;
// FableWin ??$PeekTC@$$CBVCTCObjectAugmentations@@@CThing@@..., retail tests bit 0xBF before its component-map lookup).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
#include "engine/CDefPointer.h"
class CObjectAugmentationsDef;
class CThingBaseDef;
#endif
class CTCObjectAugmentations : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_OBJECT_AUGMENTATIONS; }
    long GetFreeSlot() const;                                        // 0x00765D50
    void AttachAugmentationToSlot(long augmentationDefIndex, long slot);   // 0x00768120
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);   // 0x007661C0
    static CWideString __fastcall GetAugmentationName(long augmentationDefIndex);   // 0x00765F90 ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_DAMAGEMULTIPLIER
    // Retail opt-in (agentNF10, CTCInventoryWeapons rows 0x005C7913 / 0x005C7B10 / 0x005C7D0D / 0x005C7F07): Ego_r.pdb / FableWin.pdb
    // ?GetDamageMultiplier@CTCObjectAugmentations@@QBEMXZ, out of line in retail at 0x00766140 (landed). Declaration only.
    float GetDamageMultiplier() const;                               // 0x00766140
#endif
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
    // Retail opt-in (agentNF10, rows 0x00768010 OnInitialActivate / 0x00768120 AttachAugmentationToSlot): the first Ego_r.pdb members
    // (struct_layouts_egor.tsv; PDef +0x0C, LoadedFromSaveGame +0x10, Augmentations +0x14 are at the same offsets in retail: the
    // 12-byte STLport vector only moves the later maps) and the PDB spellings the rows define or call:
    // ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ (CTCBase slot 6), the private ?AttachAugmentationParticles@..@@AAEXXZ (0x007677E0)
    // and ?SetupAugmentationSounds@..@@AAEXXZ (0x00766C00). Retail reads LoadedFromSaveGame `[esi+10h]` and PDef `[esi+0Ch]`
    // (0x00768016..), walks Augmentations `[esi+14h]` / `[esi+18h]` (GetFreeSlot 0x00765D50). Requires FABLE_RETAIL_DEF_CLASS_BASE.
    virtual void OnInitialActivate();                                // slot 6 0x00768010
private:
    void AttachAugmentationParticles();                              // 0x007677E0
    void SetupAugmentationSounds();                                  // 0x00766C00
public:
    CDefPointer<const CObjectAugmentationsDef> PDef;                 // +0x0C
    bool LoadedFromSaveGame;                                         // +0x10
    std::vector<CDefPointer<const CThingBaseDef> > Augmentations;    // +0x14
#endif
};
#endif // FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
