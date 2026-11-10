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
#ifdef FABLE_RETAIL_CANIMATIONSET_ANIMS
// Retail opt-in (chunk 1014, row 0x005DCC00 GetRandomAnim): Anims as the Ego_r.pdb CVectorMap<unsigned long, CAnimationEntry> (0x14-byte
// pairs; retail 0x10 bytes: CArray + compare + dirty). Needs the complete CAnimationEntry (engine/CAnimationEntry.h) first.
#include "engine/CVectorMap.h"
#endif
class CAnimationSet {
public:
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
#ifdef FABLE_RETAIL_CANIMATIONSET_ANIMS
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    // Ego_r.pdb ?FindFirstAnimWithName@CAnimationSet@@QBE (public const; retail 0x005DC020 ?FindFirstAnimWithName@CAnimationSet@@QBEPBU?$pair@KVCAnimationEntry@@@std@@ABVCCharString@@@Z,
    // returns the first pair with the name's key) and ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z.
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const;                                   // 0x005DC020
    const CAnimationEntry& GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter,
                                         long avoidBankIndex) const;                                                 // 0x005DCC00
    TAnimMap Anims;                                          // +0x00
#else
    unsigned char Anims[0x10];                               // +0x00 CVectorMap<unsigned long,CAnimationEntry,...>
#endif
    long          DefaultFlags;                              // +0x10
    long          DefaultTransitionTime;                     // +0x14
    long          DefaultDelay;                              // +0x18
    unsigned char DefaultGroup[0x4];                         // +0x1c CDefString
};
FABLE_STATIC_ASSERT(sizeof(CAnimationSet) == 0x20);
#endif // FABLE_ENGINE_CANIMATIONSET_H
