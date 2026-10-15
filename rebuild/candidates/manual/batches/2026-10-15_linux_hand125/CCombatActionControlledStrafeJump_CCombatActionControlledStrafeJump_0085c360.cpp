// CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump&) -- copy constructor
// Leaf: ??0CCombatAction_ControlledStrafeJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C360, 111 bytes. Called by Clone @ 0x0085C330.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: base copy constructor first, then this class's vtable 0x01276974, then the members copied in
// declaration order one register at a time (eax/ecx/edx rotation): two C3DVector facings (six dwords at
// +0x114..+0x128) and the AddedRollMode byte at +0x12c. Retail sizeof = 0x130 (the PDB header says 0x148:
// the retail CCombatActionBase is 0x18 bytes narrower, so the header's offsets do not apply here and a
// local model is used). The facings copy word by word through eax/ecx/edx, the shape of separate
// scalar members (a struct member would copy through `lea` base registers).
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCombatAction_ControlledStrafeJump : CCombatActionBase {   // vtable 0x01276974; sizeof 0x130
    float RequiredFacing_X;                 // +0x114 C3DVector RequiredFacing
    float RequiredFacing_Y;                 // +0x118
    float RequiredFacing_Z;                 // +0x11c
    float OriginalFacing_X;                 // +0x120 C3DVector OriginalFacing
    float OriginalFacing_Y;                 // +0x124
    float OriginalFacing_Z;                 // +0x128
    bool AddedRollMode;                     // +0x12c
    virtual void v00();
    CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other);
};

CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other)
    : CCombatActionBase(other),
      RequiredFacing_X(other.RequiredFacing_X), RequiredFacing_Y(other.RequiredFacing_Y),
      RequiredFacing_Z(other.RequiredFacing_Z), OriginalFacing_X(other.OriginalFacing_X),
      OriginalFacing_Y(other.OriginalFacing_Y), OriginalFacing_Z(other.OriginalFacing_Z),
      AddedRollMode(other.AddedRollMode)
{
}
