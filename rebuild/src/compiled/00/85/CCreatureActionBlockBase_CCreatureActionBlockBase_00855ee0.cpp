// CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase&) -- copy constructor: base CCombatActionBase(other), own vtable, BlowStrength and MovementDirection copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855EE0, length 75 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0 (manifest).
// The manifest label "CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef" is propagated and rejected: the
// vtable stored is 0x01275F3C, CCreatureAction_BlockBase's (rtti-vtables.tsv), and the landed
// CCreatureAction_BlockRespond copy constructor (0x00855E80) calls this body as its base copy.
// Layout: PDB BlowStrength +0x12c, MovementDirection (C3DVector) +0x130 over a 0x12c-byte CCombatActionBase; the
// retail base is 0x114 bytes (engine/CCreatureAction_BlockBase.h exists but carries the PDB 0x13c layout and no
// copy constructor, so the class is modelled locally as in the landed BlockRespond copy).
// Shape: BlowStrength through eax, then C3DVector's memberwise copy constructor (one register per dword:
// ecx, edx, eax), `mov eax,esi` returns `this`.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_BlockBase : CCombatActionBase {  // vtable 0x01275F3C, retail sizeof 0x124
    long      BlowStrength;                     // +0x114
    C3DVector MovementDirection;                // +0x118
    virtual void v00();
    CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other);
};

CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other)
    : CCombatActionBase(other)
    , BlowStrength(other.BlowStrength)
    , MovementDirection(other.MovementDirection)
{
}