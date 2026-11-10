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
class CTCObjectAugmentations : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_OBJECT_AUGMENTATIONS; }
    long GetFreeSlot() const;                                        // 0x00765D50
    void AttachAugmentationToSlot(long augmentationDefIndex, long slot);   // 0x00768120
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);   // 0x007661C0
    static CWideString __fastcall GetAugmentationName(long augmentationDefIndex);   // 0x00765F90 ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z
};
#endif // FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
