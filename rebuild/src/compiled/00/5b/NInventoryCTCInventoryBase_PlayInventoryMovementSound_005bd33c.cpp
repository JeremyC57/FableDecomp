// NInventory::CTCInventoryBase::PlayInventoryMovementSound() const -- when the game state exists, play the inventory def's movement sample non-positionally from the parent thing's sample bank (a CSoundInit with flag 0x20)
// Leaf: ?PlayInventoryMovementSound@
// Retail: Fable.exe @ 0x005BD33C, 100 bytes, `ret` (thiscall, no arguments). Ego_r.pdb
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h PSoundManager, slot 9 Get2DEmitter (the manager's 2-D
//   CASoundEmitter, slot 3 PlaySample) and engine/CSoundInit.h (GainFaderBits +0x14); was a local CGameState / CSoundPlayer /
//   CSoundInit model.
// `?PlayInventoryMovementSound@CTCInventoryBase@NInventory@@ABEXXZ` (private const). Callers: SelectNextCategory /
// SelectPreviousCategory / HighlightNextItem 0x005BDBB0, 0x005BDC24, 0x005BDCC6, 0x005BDCEE. Twin: 0x005BD3A0 (NoMovementSound).
// Callees: CThing::GetSampleBank() const @ 0x004C7A80 (catalog "CAnimateThing::GetMeshBank" is propagated; Ego_r
// `?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ`), CSoundInit::CSoundInit @ 0x0041C930 (0x20 bytes, Flags at +0x14), the game
// state's virtual slot 9 (+0x24, the sound player) and the player's slot 3 PlaySample(bank, sample, loop, init, attached).
// Globals: 0x013B8394 (the game state: no sound while it is null; as landed CThingSoundEmitter 0x00836840).
// Layout: retail CTCInventoryBase PDef (CDefPointer<const CInventoryDef>) at +0x68 (PDB +0x70: two 16-byte vectors are 12 in
// retail); CInventoryDef MovementSound +0x4C / NoMovementSound +0x50 (PDB +0x50/+0x54: the def base is 4 bytes narrower).
// Shape: frame-pointer size TU; the sound player is fetched into a local BEFORE the def pointer is copied (add-ref after the
// virtual call), the copy's add-ref is null-tested but its release is not (`dec [esi+4] ; jne ; call [vt+4]`: the local copy
// is released by hand on the already-dereferenced pointer).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CASoundBank;
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CThing.h"
class CInventoryDef { public:
    virtual void v000();
    virtual void Destroy() const;                       // slot 1 (+0x04)
    mutable long RefCount;                              // +0x04
    unsigned char _pad08[0x4C - 0x08];
    long MovementSound;                                 // +0x4C
    long NoMovementSound;                               // +0x50
};
namespace NInventory {
class CTCInventoryBase {
public:
    void*          __vftable;                           // +0x00
    CThing*        ParentThing;                         // +0x04 (CTCBase)
    unsigned char  _pad08[0x68 - 0x08];
    CInventoryDef* PDef;                                // +0x68 CDefPointer<const CInventoryDef>::Object
    void PlayInventoryMovementSound() const;
};
void CTCInventoryBase::PlayInventoryMovementSound() const
{
    if (PSoundManager) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        CSoundInit init;
        init.GainFaderBits = 0x20;
        CASoundEmitter& emitter = PSoundManager->Get2DEmitter();
        CInventoryDef* def = PDef;
        if (def)
            ++def->RefCount;
        emitter.PlaySample(bank, def->MovementSound, true, init, false);
        if (--def->RefCount == 0)
            def->Destroy();
    }
}
}