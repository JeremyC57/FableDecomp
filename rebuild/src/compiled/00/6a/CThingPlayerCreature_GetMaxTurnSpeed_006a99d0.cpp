// CThingPlayerCreature::GetMaxTurnSpeed() -- the movement def's MaxTurnSpeedHeavyWeapon while carrying a too-heavy weapon, else its MaxTurnSpeed
// Retail: Fable.exe 0x006A99D0, 34 bytes.
// Callee: IsCarryingTooHeavyWeapon(CThing*) 0x0085EF70 (__fastcall, `this` in ecx).
// Retail offsets: PPlayerMovementDef +0x1ec (Ego_r.pdb +0x218; the retail player creature is 0x2c
// narrower there), CPlayerMovementDef MaxTurnSpeed +0x68 / MaxTurnSpeedHeavyWeapon +0x6c (PDB +0x6c /
// +0x70: the retail CDefBase is 4 bytes narrower). Local model.
// Shape: two returns that each re-read the def pointer member (no local), through the def pointer's
// inline operator->.
struct CThing;
bool __fastcall IsCarryingTooHeavyWeapon(CThing* thing);   // 0x0085EF70

struct CPlayerMovementDef {
    unsigned char _pad00[0x68];
    float         MaxTurnSpeed;                          // +0x68
    float         MaxTurnSpeedHeavyWeapon;               // +0x6c
};

template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

struct CThingPlayerCreature {
    unsigned char                    _pad00[0x1ec];
    CDefPointer<CPlayerMovementDef>  PPlayerMovementDef; // +0x1ec
    float GetMaxTurnSpeed();
};

float CThingPlayerCreature::GetMaxTurnSpeed()
{
    if (IsCarryingTooHeavyWeapon((CThing*)this))
        return PPlayerMovementDef->MaxTurnSpeedHeavyWeapon;
    return PPlayerMovementDef->MaxTurnSpeed;
}