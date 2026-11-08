#ifndef FABLE_ENGINE_CINPUTPROCESSTARGETLOCKCYCLETARGETS_H
#define FABLE_ENGINE_CINPUTPROCESSTARGETLOCKCYCLETARGETS_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessTargetLockCycleTargets : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x0123767C;
// retail sizeof 0x38 (`push 38h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessTargetLockCycleTargets +0x1C8).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessTargetLockCycleTargets@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// LastButtonPressure and ButtonPressureThresholdForClick start at 0.0f, ClickAcceptancePulseTimer at -1, AllowPulsesOnly false (`mov [edi+28h],ebx ; mov [edi+2Ch],ebx ; mov dword ptr [edi+30h],0FFFFFFFFh ; mov [edi+34h],bl`).
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessTargetLockCycleTargets : public CInputProcessGameBase {
public:
    CInputProcessTargetLockCycleTargets(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init), LastButtonPressure(0.0f), ButtonPressureThresholdForClick(0.0f), ClickAcceptancePulseTimer(-1), AllowPulsesOnly(false) { }
    float LastButtonPressure;                                       // +0x28
    float ButtonPressureThresholdForClick;                          // +0x2c
    long ClickAcceptancePulseTimer;                                 // +0x30
    bool AllowPulsesOnly;                                           // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessTargetLockCycleTargets) == 0x38);
#endif // FABLE_ENGINE_CINPUTPROCESSTARGETLOCKCYCLETARGETS_H
