// CTCTargetingPlayer::GetFOVToBeInToBeTargetable() const -- the targeting arc (def degrees as turns): the PC third-person aiming arc while the player is in that aiming mode, the normal arc otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008025D0, 49 bytes (+ int3 padding), plain `ret` (__fastcall, no
//   arguments), float result on the x87 stack. No direct caller or vtable slot in the manifest (an inline-able const
//   helper of the targeting TU kept out of line).
// Name: Ego_r.pdb `?GetFOVToBeInToBeTargetable@CTCTargetingPlayer@@IBEMXZ` sits between SetAsTargetNextThing and
//   GetPTargetedThing in the donor, exactly where 0x008025D0 sits between the landed SetAsTargetNextThing 0x00802590 and
//   ClearLockedTarget 0x00802650 in retail; the catalog label "GetModifierValue" is descriptive.
// Leaf: GetFOVToBeInToBeTargetable
// Callees: CThing::GetPlayer 0x004C7A10. Constant: 1/360 (0.0027777778f) @ 0x01238E00 (degrees -> turns).
// Layout (local model, as the landed siblings 0x008026A0 / 0x00802420): CTCBase ParentThing +0x04; CTCTargetingAI
//   PSubDef (the CTargetingDef) +0x30 -- engine/CTCTargetingPlayer.h flattens the base to pads there. Ego_r.pdb
//   CTargetingDef: TargetArc +0x30, TargetArcPCThirdPersonAimingMode +0x34. Retail CPlayer
//   PCProjectileWeaponThirdPersonAimingMode +0x21A (PDB +0x222; the landed CPlayer rows use +0x21A).
// Shape: two complete `return def->x * K;` arms (`fld [def+N] ; fmul [K]`, the def loaded into a different register on
//   each arm because the test's flag byte was held in cl).
class CPlayer { public:
    unsigned char _pad000[0x21a];
    bool          PCProjectileWeaponThirdPersonAimingMode;   // +0x21A
};

class CThing { public:
    CPlayer* GetPlayer();                                    // 0x004C7A10
};

class CTargetingDef { public:
    unsigned char _pad00[0x30];
    float         TargetArc;                                 // +0x30 (degrees)
    float         TargetArcPCThirdPersonAimingMode;          // +0x34 (degrees)
};

class CTCTargetingAI { public:
    void*          __vftable;                                // +0x00
    CThing*        ParentThing;                              // +0x04 (CTCBase)
    unsigned char  _pad08[0x30 - 0x08];
    CTargetingDef* PSubDef;                                  // +0x30
};

class CTCTargetingPlayer : public CTCTargetingAI { public:
    float GetFOVToBeInToBeTargetable() const;
};

float CTCTargetingPlayer::GetFOVToBeInToBeTargetable() const
{
    if (ParentThing->GetPlayer()->PCProjectileWeaponThirdPersonAimingMode)
        return PSubDef->TargetArcPCThirdPersonAimingMode * (1.0f / 360.0f);
    return PSubDef->TargetArc * (1.0f / 360.0f);
}