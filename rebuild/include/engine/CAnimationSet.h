#ifndef FABLE_ENGINE_CANIMATIONSET_H
#define FABLE_ENGINE_CANIMATIONSET_H
// CAnimationSet (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): a def's animation bank (the
// Animation member of CAppearanceDef +0x34). Ego_r.pdb layout (36 bytes): Anims +0x00 (CVectorMap<unsigned long, CAnimationEntry>,
// 0x10 in retail), DefaultFlags +0x10, DefaultTransitionTime +0x14, DefaultDelay +0x18, DefaultGroup +0x1C (CDefString) -- kept opaque
// here. Out of line: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z (Ego_r.pdb / FableWin.pdb,
// retail 0x005DC340: the entry with that name and variation index that the filter accepts; returned by reference).
#include "rebuild_abi.h"
class CCharString;
class CAnimationEntry;
class CAnimFilterBase;
class CAnimationSet {
public:
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
    unsigned char Anims[0x10];                               // +0x00 CVectorMap<unsigned long,CAnimationEntry,...>
    long          DefaultFlags;                              // +0x10
    long          DefaultTransitionTime;                     // +0x14
    long          DefaultDelay;                              // +0x18
    unsigned char DefaultGroup[0x4];                         // +0x1c CDefString
};
FABLE_STATIC_ASSERT(sizeof(CAnimationSet) == 0x20);
#endif // FABLE_ENGINE_CANIMATIONSET_H
