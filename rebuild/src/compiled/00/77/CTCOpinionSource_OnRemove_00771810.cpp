// CTCOpinionSource::OnRemove() -- the remove path: CleanupSource
// Leaf: ?OnRemove@CTCOpinionSource@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771810, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 2 of CTCOpinionSource's vtable 0x0126A81C (the slot holds this body in 1 vtable(s), owner CTCOpinionSource); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnRemove(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnRemove@CTCOpinionSource@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCOpinionSource::vfunc_2 (rebuild/src/compiled/00/77/gapscan_sub00771810_00771810.cpp).
// No shared header for CTCOpinionSource: minimal local definition (RTTI base CTCBase), the override and the CleanupSource it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCOpinionSource : public CTCBase {
public:
    virtual void OnRemove();
public:
    void CleanupSource();
};
void CTCOpinionSource::OnRemove()
{
    CleanupSource();
}
