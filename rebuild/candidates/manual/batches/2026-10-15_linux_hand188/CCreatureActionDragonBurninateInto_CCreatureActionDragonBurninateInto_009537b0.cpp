// CCreatureAction_DragonBurninateInto::CCreatureAction_DragonBurninateInto(CThingCreatureBase*, const vector<C3DVector>&, const C3DVector&, float, float, const C3DVector&, const C3DVector&) -- base CCombatActionBase(owner, 250, the owner's "SWORD_SWING" sound pair), then the burn path fields from the arguments
// Leaf: ??0CCreatureAction_DragonBurninateInto@@QAE@PAUCThingCreatureBase@@ABUCVectorC3DVector@@ABUC3DVector@@MM22@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009537B0, length 208 bytes, `ret 1Ch`. Vtable 0x01294374.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("SWORD_SWING" @ 0x0127AC80), GetSoundPair(CThingCreatureBase*, const CCharString&)
// 0x008561A0 (`__fastcall`, CSoundPair by hidden pointer), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&)
// 0x00858030, std::vector<C3DVector> copy constructor 0x006F0810 (manifest "Vector_CopyFrom").
// Shape: the literal is an implicit CCharString temporary built before the base arguments and destroyed after the base call;
// init-list members in declaration order (C3DVector's memberwise user copy ctor; floats copied as dwords).
// Layout: PDB header + 0x18 = retail offsets (the vector is 12 bytes in retail, 16 in the PDB build).
struct CThingCreatureBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {                             // two sound ids (user ctor: returned through a hidden pointer)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase* creature, const CCharString& name);   // 0x008561A0

struct C3DVector {                              // three floats; the memberwise user copy constructor copies each as a dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CVectorC3DVector {                       // std::vector<C3DVector> (retail 12 bytes)
    C3DVector* _First;
    C3DVector* _Last;
    C3DVector* _End;
    CVectorC3DVector(const CVectorC3DVector& other);   // 0x006F0810 (out-of-line copy constructor)
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    CCombatActionBase(const CCombatActionBase& other);                                        // 0x0062F3B0
};

struct CCreatureAction_DragonBurninateInto : CCombatActionBase {   // vtable 0x01294374; retail sizeof 0x14c
    C3DVector        PreviousPoint;             // +0x114 (PDB +0x12c)
    CVectorC3DVector MovementPoints;            // +0x120 (PDB +0x138)
    float            BurnStart;                 // +0x12c (PDB +0x148)
    float            BurnStop;                  // +0x130 (PDB +0x14c)
    C3DVector        FlameStartPos;             // +0x134 (PDB +0x150)
    C3DVector        FlameEndPos;               // +0x140 (PDB +0x15c)
    virtual void v00();
    CCreatureAction_DragonBurninateInto(CThingCreatureBase* owner, const CVectorC3DVector& movementPoints, const C3DVector& previousPoint,
                                        float burnStart, float burnStop, const C3DVector& flameStartPos, const C3DVector& flameEndPos);
    CCreatureAction_DragonBurninateInto(const CCreatureAction_DragonBurninateInto& other);
};

CCreatureAction_DragonBurninateInto::CCreatureAction_DragonBurninateInto(CThingCreatureBase* owner, const CVectorC3DVector& movementPoints,
    const C3DVector& previousPoint, float burnStart, float burnStop, const C3DVector& flameStartPos, const C3DVector& flameEndPos)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING"))
    , PreviousPoint(previousPoint)
    , MovementPoints(movementPoints)
    , BurnStart(burnStart)
    , BurnStop(burnStop)
    , FlameStartPos(flameStartPos)
    , FlameEndPos(flameEndPos)
{
}
