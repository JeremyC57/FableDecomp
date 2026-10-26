// NInventory::CTCInventoryBase::PlayInventoryNoMovementSound() const -- when the game state exists, play the inventory def's no-movement sample non-positionally from the parent thing's sample bank (a CSoundInit with flag 0x20)
// Leaf: ?PlayInventoryNoMovementSound@
// Retail: Fable.exe @ 0x005BD3A0, 100 bytes, `ret` (thiscall, no arguments). Ego_r.pdb
// `?PlayInventoryNoMovementSound@CTCInventoryBase@NInventory@@ABEXXZ` (private const). No direct callers found by xref (the
// catalog's "CAnimateThing_PlaySoundAtMeshBank" is descriptive). Twin: 0x005BD33C (MovementSound, +0x4C).
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
class CSoundInit { public:                                     // 0x20 bytes (ctor 0x0041C930)
    float Volume;                                       // +0x00
    float Pitch;                                        // +0x04
    long  Delay;                                        // +0x08
    long  LoopCount;                                    // +0x0c
    long  Priority;                                     // +0x10
    long  Flags;                                        // +0x14
    long  Pan;                                          // +0x18
    long  Id;                                           // +0x1c
    CSoundInit();                                       // 0x0041C930
};
struct CSoundPlayer {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);   // slot 3 (+0x0c)
};
struct CGameState {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual CSoundPlayer* GetSoundPlayer();              // slot 9 (+0x24)
};
extern CGameState* g_pGameState;                        // 0x013B8394
class CThing { public:
    CASoundBank& GetSampleBank() const;                 // 0x004C7A80
};
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
    void PlayInventoryNoMovementSound() const;
};
void CTCInventoryBase::PlayInventoryNoMovementSound() const
{
    if (g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        CSoundInit init;
        init.Flags = 0x20;
        CSoundPlayer* player = g_pGameState->GetSoundPlayer();
        CInventoryDef* def = PDef;
        if (def)
            ++def->RefCount;
        player->PlaySample(bank, def->NoMovementSound, true, init, false);
        if (--def->RefCount == 0)
            def->Destroy();
    }
}
}