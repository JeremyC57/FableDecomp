// CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase*, const C3DVector&, float, long) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x0128439C), Override set, Dir copied from the vector, Distance and Level from the arguments, IsZTargetting cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE340, length 123 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the sound pair is zeroed with immediates in the frame and passed by reference (arguments right to left: the pair,
// 0x96, the owner); then the init-list members in declaration order: Override = 1 (sinks below the vtable store), the three
// direction words copied through registers from the reference (C3DVector's memberwise user copy constructor), Distance and Level from their
// argument slots, IsZTargetting = 0. `ret 10h`: four arguments.
// Layout: byte-packed as the PDB header (CCreatureAction_CastBattleCharge.h: Override +0x12c, Dir +0x12d, Distance +0x13c,
// Level +0x140, IsZTargetting +0x144 -- 24 bytes high, the retail CCombatActionBase being 0x114 bytes), so `this` is modelled
// locally under #pragma pack(1) at +0x114 / +0x115 / +0x124 / +0x128 / +0x12c, as the landed sibling constructors @ 0x008CE2F0 / 0x008CE290.
struct CThingCreatureBase;

struct C3DVector {                          // three floats; the memberwise user copy constructor copies each as a dword
    float X, Y, Z;                          //   straight into the member (a POD copy would form the member address once)
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (immediate frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

#pragma pack(push, 1)
struct CCreatureAction_CastBattleCharge : CCombatActionBase {   // vtable 0x0128439C; retail sizeof 0x130
    bool          Override;                 // +0x114 (PDB +0x12c)
    C3DVector     Dir;                      // +0x115 (PDB +0x12d), unaligned
    unsigned char _pad121[3];               // +0x121
    float         Distance;                 // +0x124 (PDB +0x13c)
    long          Level;                    // +0x128 (PDB +0x140)
    bool          IsZTargetting;            // +0x12c (PDB +0x144)
    unsigned char _pad12d[3];               // +0x12d
    virtual void v00();
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, const C3DVector& dir, float distance, long level);
};
#pragma pack(pop)

CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, const C3DVector& dir, float distance, long level)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Override(true)
    , Dir(dir)
    , Distance(distance)
    , Level(level)
    , IsZTargetting(false)
{
}