#ifndef FABLE_ENGINE_CANIMFILTERANYGROUP_H
#define FABLE_ENGINE_CANIMFILTERANYGROUP_H
// CAnimFilterAnyGroup (new shared header, chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor): the
// animation filter that accepts an entry of any group. RTTI .?AVCAnimFilterAnyGroup@@ : CAnimFilterBase (ghidra_out/rtti_bases.tsv);
// retail vtable 0x0127261C: slot 0 operator() 0x00834CC0, slot 1 Clone 0x008350D0, slot 2 scalar deleting dtor 0x00834CD0
// (rebuild/manifest/rtti-vtables.tsv). FableWin.pdb ??0CAnimFilterAnyGroup@@QAE@XZ, ??1CAnimFilterAnyGroup@@UAE@XZ,
// ??RCAnimFilterAnyGroup@@UBE_NABVCAnimationEntry@@@Z, ??_GCAnimFilterAnyGroup@@UAEPAXI@Z. Ctor and dtor inline (0x008A8D21 stores
// the vtable, 0x008A8D34 the CAnimFilterBase vtable 0x0122FFCC). No members.
#include "rebuild_abi.h"
#include "engine/CAnimFilterBase.h"
class CAnimationEntry;
class CAnimFilterAnyGroup : public CAnimFilterBase {
public:
    CAnimFilterAnyGroup() { }
    virtual bool operator()(const CAnimationEntry& entry) const;         // slot 0  0x00834CC0
    virtual CAnimFilterBase* Clone() const;                              // slot 1  0x008350D0
};
FABLE_STATIC_ASSERT(sizeof(CAnimFilterAnyGroup) == 4);
#endif // FABLE_ENGINE_CANIMFILTERANYGROUP_H
