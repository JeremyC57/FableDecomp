#ifndef FABLE_ENGINE_CANIMFILTERBASE_H
#define FABLE_ENGINE_CANIMFILTERBASE_H
// CAnimFilterBase (new shared header, chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the abstract animation
// filter CAnimationSet lookups take (Ego_r.pdb ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
// and its siblings). RTTI CAnimFilterBase, vtable 0x0122FFCC = { _purecall 0x00BFEA08, _purecall 0x00BFEA08, scalar deleting dtor
// 0x0041CCE4 }: slot 0 the filter test operator()(const CAnimationEntry&) const and slot 1 Clone() const are pure (the derived
// slots are named by Ego_r.pdb ??R / ?Clone@CAnimFilterNoGroup@@), slot 2 the virtual destructor. The destructor is inline and
// empty: a derived temporary's destruction leaves only the store of this vtable (0x0060B460).
#include "rebuild_abi.h"
class CAnimationEntry;
class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;     // slot 0
    virtual CAnimFilterBase* Clone() const = 0;                          // slot 1
    virtual ~CAnimFilterBase() { }                                       // slot 2 (scalar deleting 0x0041CCE4)
};
FABLE_STATIC_ASSERT(sizeof(CAnimFilterBase) == 0x4);
#endif // FABLE_ENGINE_CANIMFILTERBASE_H
