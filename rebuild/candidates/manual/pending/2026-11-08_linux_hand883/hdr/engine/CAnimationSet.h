#ifndef FABLE_ENGINE_CANIMATIONSET_H
#define FABLE_ENGINE_CANIMATIONSET_H
// CAnimationSet (new shared header, chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the named animations of an
// appearance def. Ego_r.pdb layout (struct_layouts_egor.tsv, 36 bytes with the 16-byte debug CVectorMap): Anims (CVectorMap<unsigned
// long, CAnimationEntry>), DefaultFlags, DefaultTransitionTime, DefaultDelay (long), DefaultGroup (CDefString); retail 0x20 (CVectorMap
// 0x10). Only the lookup the row calls is declared: Ego_r.pdb ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z,
// out of line at 0x005DC340 (Ego_r donor order GetNoAnims 0x0063BC30 < AnimExists 0x0063BCC0 < GetAnim 0x0063BD40; retail
// 0x005DC2E0 is the has-animation test the landed CThingDeadCreature::SetupDeadAnimation calls just before it).
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CAnimationEntry.h"
#include "engine/CVectorMap.h"
#include "engine/CDefString.h"
class CAnimFilterBase;
class CAnimationSet {
public:
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
    CVectorMap<unsigned long, CAnimationEntry> Anims;                    // +0x00
    long DefaultFlags;                                                   // +0x10
    long DefaultTransitionTime;                                          // +0x14
    long DefaultDelay;                                                   // +0x18
    CDefString DefaultGroup;                                             // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CAnimationSet) == 0x20);
#endif // FABLE_ENGINE_CANIMATIONSET_H
