// NCreatureMode::CModeStrafe::OnDeactivate() -- forget the active weapon, deactivate the mode, then reset the strafe animation state
// Leaf: ?OnDeactivate@CModeStrafe@NCreatureMode@@UAEXXZ
// Retail: Fable.exe @ 0x008E9B60, 16 bytes, thiscall, tail `jmp`.
// Identity (proven): vtable slot 6 (+0x18) of NCreatureMode::CModeStrafe 0x01268588 (read from Fable.exe: slot 5 is the
// landed CModeStrafe::OnActivate 0x008E9A30, slot 3 FrameUpdate); 46 of the 48 CModeBase-derived classes overriding slot 6
// have an Ego_r.pdb / FableWin.pdb OnDeactivate, among them ?OnDeactivate@CModeStrafe@NCreatureMode@@UAEXXZ. The base slot-6
// body 0x00739E60 is `Active = false` (+0x18), inlined here.
// Label correction: catalog "NCreatureMode::CModeStrafe::vfunc_6".
// Callees: 0x008E97B0 (unlanded gap-scan row, no arguments, plain `ret`): a private CModeStrafe helper that walks the
// strafe animation entries; Ego_r.pdb lists the no-argument privates SetUpCombinationAnims /
// LockStrafingCombinationAnimsToMainAnim / UpdateWeaponState, but nothing ties 0x008E97B0 to one of them, so it is
// declared under a descriptive name.
// Layout: Ego_r.pdb CTCCreatureModeManager::CModeBase (ParentThing +4, WeaponBasedFilter +8, FramesBeforeRemovingSelf +0x14,
// Active +0x18, size 0x1C) and CModeStrafe (AnimMovementDistance +0x1C .. ActiveWeapon +0x38); no STL members before
// +0x38, so the PDB offsets are retail's. No engine header for either: local model.
// Shape: the inline base call is a plain store; the helper becomes a tail `jmp`.
class CThing;

class CCharString {
public:
    void* PStringData;                                      // 4 bytes in retail
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual void OnDeactivate() { Active = false; }

        CThing* ParentThing;                                // +0x04
        unsigned char WeaponBasedFilter[0xC];               // +0x08 CAnimFilterOfGroup
        long FramesBeforeRemovingSelf;                      // +0x14
        bool Active;                                        // +0x18
    };
};

namespace NCreatureMode {
class CModeStrafe : public CTCCreatureModeManager::CModeBase {
public:
    virtual void OnDeactivate();
private:
    void ResetStrafeAnims_008e97b0();                       // 0x008E97B0, descriptive name

    float AnimMovementDistance;                             // +0x1C
    CCharString StrafeAnimName;                             // +0x20
    long AnimIndex;                                         // +0x24
    bool LastUsable;                                        // +0x28
    unsigned char Filter[0xC];                              // +0x2C CAnimFilterOfGroup
    long ActiveWeapon;                                      // +0x38
};

void CModeStrafe::OnDeactivate()
{
    ActiveWeapon = -1;
    CModeBase::OnDeactivate();
    ResetStrafeAnims_008e97b0();
}
}