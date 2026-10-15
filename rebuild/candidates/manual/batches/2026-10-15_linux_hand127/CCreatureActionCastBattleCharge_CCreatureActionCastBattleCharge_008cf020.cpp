// CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge&) -- copy constructor: base CCombatActionBase(other), own vtable (0x0128439C), then Override, Dir, Distance, Level and IsZTargetting copied member by member
// Leaf: ??0CCreatureAction_CastBattleCharge@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF020, length 111 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: `other` rides in edi and is pushed for the out-of-line base copy constructor; the vtable store comes first, then the
// members in declaration order, each copied through a rotating register (al / ecx / edx): the byte, the three direction
// words (C3DVector's memberwise user copy constructor), the two words, the trailing byte. The three padding bytes after
// Dir are skipped, so this is the memberwise copy and not a block copy. `ret 4`.
// Layout: byte-packed as the PDB header (CCreatureAction_CastBattleCharge.h: Override +0x12c, Dir +0x12d, Distance +0x13c,
// Level +0x140, IsZTargetting +0x144 -- 24 bytes high, the retail CCombatActionBase being 0x114 bytes), so `this` is modelled
// locally under #pragma pack(1) at +0x114 / +0x115 / +0x124 / +0x128 / +0x12c, as the landed sibling constructors @ 0x008CE2F0 / 0x008CE290.
struct C3DVector {                          // three floats; the memberwise user copy constructor copies each as a dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
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
    CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge& other);
};
#pragma pack(pop)

CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge& other)
    : CCombatActionBase(other)
    , Override(other.Override)
    , Dir(other.Dir)
    , Distance(other.Distance)
    , Level(other.Level)
    , IsZTargetting(other.IsZTargetting)
{
}
