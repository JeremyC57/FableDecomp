// CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(const CCombatAction_KickThingOnGround&) -- copy constructor
// Leaf: ??0CCombatAction_KickThingOnGround
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880120, 112 bytes. Called by Clone @ 0x008800F0.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: base copy constructor first, then this class's vtable 0x01277D3C, then the seven members copied
// in declaration order one register at a time (eax/ecx/edx rotation): MoveToPos (C3DVector, +0x114),
// Movement (C3DVector, +0x120) and MaxRunningSpeed (+0x12c). Retail sizeof = 0x130; the PDB header
// (sizeof 0x148) places the same members 0x18 bytes higher because the retail CCombatActionBase is
// 0x18 bytes narrower, so a local model is used. The vectors copy word by word through eax/ecx/edx,
// the shape of separate scalar members (a struct member would copy through `lea` base registers).
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCombatAction_KickThingOnGround : CCombatActionBase {   // vtable 0x01277D3C; sizeof 0x130
    float MoveToPos_X;                      // +0x114 C3DVector MoveToPos
    float MoveToPos_Y;                      // +0x118
    float MoveToPos_Z;                      // +0x11c
    float Movement_X;                       // +0x120 C3DVector Movement
    float Movement_Y;                       // +0x124
    float Movement_Z;                       // +0x128
    float MaxRunningSpeed;                  // +0x12c
    virtual void v00();
    CCombatAction_KickThingOnGround(const CCombatAction_KickThingOnGround& other);
};

CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(const CCombatAction_KickThingOnGround& other)
    : CCombatActionBase(other),
      MoveToPos_X(other.MoveToPos_X), MoveToPos_Y(other.MoveToPos_Y), MoveToPos_Z(other.MoveToPos_Z),
      Movement_X(other.Movement_X), Movement_Y(other.Movement_Y), Movement_Z(other.Movement_Z),
      MaxRunningSpeed(other.MaxRunningSpeed)
{
}
