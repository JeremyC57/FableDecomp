// CTCAtmosPlayer::OnKill() -- destroys the atmos sound emitter
// Leaf: ?OnKill@CTCAtmosPlayer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E35D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCAtmosPlayer's vtable 0x01270484 (the slot holds this body in 1 vtable(s), owner CTCAtmosPlayer); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCAtmosPlayer@@UAEXXZ; `ret` matches the signature.
// Was: CTCAtmosPlayer::vfunc_14 (rebuild/src/compiled/00/7e/CVolume_ReleaseVolume_007e35d0.cpp).
// No shared header for CTCAtmosPlayer: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members PSoundEmitter +0x0C,
// AtmosDefIndex +0x10).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CASoundManager.h"
class CASoundEmitter;
class CTCAtmosPlayer : public CTCBase {
public:
    virtual void OnKill();                                   // slot 14
    CASoundEmitter* PSoundEmitter;                           // +0x0C
    long AtmosDefIndex;                                      // +0x10
};
void CTCAtmosPlayer::OnKill()
{
    if (PSoundEmitter) {
        PSoundManager->DestroyPEmitter(PSoundEmitter);
        PSoundEmitter = 0;
    }
}
