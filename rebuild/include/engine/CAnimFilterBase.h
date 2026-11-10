#ifndef FABLE_ENGINE_CANIMFILTERBASE_H
#define FABLE_ENGINE_CANIMFILTERBASE_H
// CAnimFilterBase (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): the animation-bank lookup
// filter. Retail vtable 0x0122FFCC (read from Fable.exe): slot 0 _purecall = ??RCAnimFilterBase (operator()(const CAnimationEntry&)
// const), slot 1 _purecall = Clone() const, slot 2 0x0041CCE4 scalar deleting destructor (stores this vtable, then frees). FableWin.pdb
// / Ego_r.pdb: ??0CAnimFilterBase@@QAE@XZ, ??1CAnimFilterBase@@UAE@XZ (virtual, inline), ??4CAnimFilterBase@@QAEAAV0@ABV0@@Z. No members.
#include "rebuild_abi.h"
class CAnimationEntry;
class CAnimFilterBase {
public:
    CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;                        // slot 1
    virtual ~CAnimFilterBase() { }                                     // slot 2 (scalar deleting 0x0041CCE4)
};
#endif // FABLE_ENGINE_CANIMFILTERBASE_H
