// CThingFilter_IsAICreatureAwareOf::operator()(const CThingCreatureBase& thing) const -- the thing filter: is the filter's AI creature
//   aware of the thing (NCreatureAI::IsAwareOfThing without the line-of-sight check)
// Leaf: ??RCThingFilter_IsAICreatureAwareOf@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7610, 16 bytes, thiscall, `ret 4`:
//   `mov edx,[esp+4] ; mov ecx,[ecx] ; push 0 ; call 0x008645E0 ; ret 4`.
// Name: Ego_r.pdb 0x005A2FC0 + FableWin.pdb 0x01F94560 ??RCThingFilter_IsAICreatureAwareOf@@QBE_NABVCThingCreatureBase@@@Z; Ego_r.pdb
//   layout: PCreature (CThingAICreature*) +0x00, size 4; the landed CThingSearchTools instantiation 0x007F7340 binds this name.
//   Callee NCreatureAI::IsAwareOfThing 0x008645E0 (Ego_r.pdb / FableWin.pdb; engine/NCreatureAI.h). NF95 2026-11-19 (q11325): was
//   the stand-in CForwarder_Retail::ForwardToMember0_008645e0 calling Shim_008645e0 (catalog _Dest_val_..., propagated).
// The thing goes to IsAwareOfThing as its CThing base (CThingCreatureBase derives from CThing at offset 0: no adjustment).
#include "rebuild_abi.h"
#include "engine/NCreatureAI.h"
class CThingCreatureBase;
class CThingFilter_IsAICreatureAwareOf {
public:
    bool operator()(const CThingCreatureBase& thing) const;
private:
    CThingAICreature* PCreature;                            // +0x00
};
bool CThingFilter_IsAICreatureAwareOf::operator()(const CThingCreatureBase& thing) const
{
    return NCreatureAI::IsAwareOfThing(PCreature, (const CThing*)&thing, false);
}
