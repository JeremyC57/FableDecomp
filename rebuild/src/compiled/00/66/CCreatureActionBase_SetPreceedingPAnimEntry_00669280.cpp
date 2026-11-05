// CCreatureActionBase::SetPreceedingPAnimEntry(const CCountedPointer<CAnimationEntry>& entry) -- hand the action the animation entry of the action it takes over from (counted-pointer assignment into PPreceedingPAnimEntry at +0x7C)
// Leaf: ?SetPreceedingPAnimEntry@CCreatureActionBase@@QAEXABV?$CCountedPointer@VCAnimationEntry@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Item->Data are the header CCPPointerInfo's (same offsets).
// Flags: /O2 /Oy /W3
// Retail: Fable.exe @ 0x00669280, 83 bytes, `ret 4`; one caller 0x00664577 in CThingCreatureBase::SetCurrentAction
// (landed 0x006644F0), which calls it on the NEW action with the OLD action's member at +0x74. Byte family: 0x00669280 /
// 0x00992470 / 0x009FD870 differ only in the member offset; the manifest propagated NUISystem::CFrontEndScreen::
// AddActionOnLeftClicked (really 0x0054E4F0, vtable slot 71, +0x160) onto them -- 0x00669280 is in no vtable.
// Owner evidence: both objects in the caller are CCreatureActionBase (Ego_r.pdb ?SetCurrentAction@CThingCreatureBase@@
// QAE_NABVCCreatureActionBase@@@Z); Ego_r.pdb CCreatureActionBase (0xB4 bytes, retail-exact here, engine/CCreatureActionBase.h)
// has CCountedPointer<CAnimationEntry> PAnimEntry +0x74 and PPreceedingPAnimEntry +0x7C. The method name is descriptive
// from the member (no PDB method symbol survives for the setter); the class and member are proven.
// Body: Member = entry -- the inline CCountedPointer::operator= / ShareData: unless the info blocks match, release the old
// one (`--count`, re-read; at zero call its destroy thunk (+4) on the item (+8), then operator delete 0x00BFE9BC on the
// block), copy Data / Info, `++count` on the new block.
// Layout: engine/CCreatureActionBase.h flattens the counted pointers into *_Data / *_Info words, so the member is modelled
// locally as the CCountedPointer it is (offsets identical: +0x74 / +0x7C, size 0xB4).
// Shape: /O2; the source Data / Info bound to locals first (ebx / edi loaded before the compare).
#include "engine/CCountedPointer.h"
class CAnimationEntry;
class CCreatureActionBase {
public:
    unsigned char                    _pad00[0x74];
    CCountedPointer<CAnimationEntry> PAnimEntry;             // +0x74
    CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;  // +0x7C
    unsigned char                    _pad84[0xb4 - 0x84];
    void SetPreceedingPAnimEntry(const CCountedPointer<CAnimationEntry>& entry);
};
void CCreatureActionBase::SetPreceedingPAnimEntry(const CCountedPointer<CAnimationEntry>& entry)
{
    PPreceedingPAnimEntry = entry;
}
