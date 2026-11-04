// CCombatAction_KickThingOnGround::GetNextPos() const -- the MoveToPos vector by value
// Leaf: ?GetNextPos@CCombatAction_KickThingOnGround@@UBE?AVC3DVector@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087E770, 33 bytes, thiscall, `ret 4` (the hidden result pointer).
// Identity (proven): slot 70 of vtable 0x01277D3C (RTTI .?AVCCombatAction_KickThingOnGround@@; the only vtable holding it); Ego_r.pdb
//   ?GetNextPos@CCombatAction_KickThingOnGround@@UBE?AVC3DVector@@XZ is the class's only C3DVector-returning virtual and MoveToPos (Ego_r.pdb +0x12C) is the member
//   read at retail +0x114.
// Shape (family M138, representative 0x0056E3B0): `return MoveToPos;` -- C3DVector's copy (three dword moves) into the result.
// Header debt: the class is modelled by the member's retail offset (its other members as opaque bytes).
#include "engine/C3DVector.h"
class CCombatAction_KickThingOnGround {
public:
    virtual ~CCombatAction_KickThingOnGround();
    virtual C3DVector GetNextPos() const;
    unsigned char _pad004[0x114 - 4];
    C3DVector MoveToPos;                                        // +0x114
};
C3DVector CCombatAction_KickThingOnGround::GetNextPos() const
{
    return MoveToPos;
}