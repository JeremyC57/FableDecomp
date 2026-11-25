// Leaf: ?GetMaxTurnSpeed@CThingPlayerCreature@@IBEMXZ
// CThingPlayerCreature::GetMaxTurnSpeed() -- the movement def's MaxTurnSpeedHeavyWeapon while carrying a too-heavy weapon, else its MaxTurnSpeed
// Retail: Fable.exe 0x006A99D0, 34 bytes.
// Callee: IsCarryingTooHeavyWeapon(CThing*) 0x0085EF70 (__fastcall, `this` in ecx).
// Retail offsets: PPlayerMovementDef +0x1ec (Ego_r.pdb +0x218; the retail player creature is 0x2c
// narrower there), CPlayerMovementDef MaxTurnSpeed +0x68 / MaxTurnSpeedHeavyWeapon +0x6c (PDB +0x6c /
// +0x70: the retail CDefBase is 4 bytes narrower). Local model.
// Shape: two returns that each re-read the def pointer member (no local), through the def pointer's
// inline operator->.
class CThing;
// Naming (NF439 gen, 2026-11-25): callee 0x0085EF70 binds its PDB spelling ?IsCarryingTooHeavyWeapon@NCreatureMode@@YI_NABVCThing@@@Z (T1 target; was the free stand-in ?IsCarryingTooHeavyWeapon@@YI_NPAVCThing@@@Z).
namespace NCreatureMode { bool __fastcall IsCarryingTooHeavyWeapon(CThing const& thing); }

class CPlayerMovementDef { public:
    unsigned char _pad00[0x68];
    float         MaxTurnSpeed;                          // +0x68
    float         MaxTurnSpeedHeavyWeapon;               // +0x6c
};

template <class T> class CDefPointer { public:
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

class CThingPlayerCreature { public:
    unsigned char                    _pad00[0x1ec];
    CDefPointer<CPlayerMovementDef>  PPlayerMovementDef; // +0x1ec
protected:
    float GetMaxTurnSpeed() const;   // NF35: Ego_r + FableWin access/const spelling (see the Leaf line)
public:
};

float CThingPlayerCreature::GetMaxTurnSpeed() const
{
    if (NCreatureMode::IsCarryingTooHeavyWeapon(*((CThing*)this)))
        return PPlayerMovementDef->MaxTurnSpeedHeavyWeapon;
    return PPlayerMovementDef->MaxTurnSpeed;
}