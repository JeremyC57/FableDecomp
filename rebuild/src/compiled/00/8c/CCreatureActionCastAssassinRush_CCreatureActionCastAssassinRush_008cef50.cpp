// CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush&) -- copy constructor: base CCombatActionBase(other), own vtable (0x01284254), then Override, Dir, Distance, Level, the add-ref'd FinalAction counted pointer, UseCollision and IsZTargetting copied member by member
// Leaf: ??0CCreatureAction_CastAssassinRush@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CEF50, length 153 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: `other` rides in edi and is pushed for the out-of-line base copy constructor; the vtable store comes first, then the
// members in declaration order through rotating registers: the byte, the three direction words (C3DVector's memberwise user
// copy constructor), Distance, Level, then FinalAction (CCountedPointer<CCreatureActionBase>: Data copied, Info copied and
// its count block add-ref'd in place with `inc dword ptr [eax]` when non-null -- the landed 0x009775F0 family), and the two
// trailing bytes (the second stored after `pop edi`). `ret 4`.
// Layout: byte-packed as the PDB header (CCreatureAction_CastAssassinRush.h: Override +0x12c .. IsZTargetting +0x14d -- 24
// bytes high, the retail CCombatActionBase being 0x114 bytes), so `this` is modelled locally under #pragma pack(1) at
// +0x114 / +0x115 / +0x124 / +0x128 / +0x12c / +0x134 / +0x135.
class CCreatureActionBase;

class CCPPointerInfo { public:
    long m_count;                           // +0x00 shared reference count
};

struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

class C3DVector { public:                          // three floats; the memberwise user copy constructor copies each as a dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

#pragma pack(push, 1)
class CCreatureAction_CastAssassinRush : public CCombatActionBase { public:   // vtable 0x01284254; retail sizeof 0x138
    bool          Override;                 // +0x114 (PDB +0x12c)
    C3DVector     Dir;                      // +0x115 (PDB +0x12d), unaligned
    unsigned char _pad121[3];               // +0x121
    float         Distance;                 // +0x124 (PDB +0x13c)
    long          Level;                    // +0x128 (PDB +0x140)
    CCountedPointer_CCreatureActionBase FinalAction;   // +0x12c (PDB +0x144)
    bool          UseCollision;             // +0x134 (PDB +0x14c)
    bool          IsZTargetting;            // +0x135 (PDB +0x14d)
    unsigned char _pad136[2];               // +0x136
    virtual void v00();
    CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush& other);
};
#pragma pack(pop)

CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush& other)
    : CCombatActionBase(other)
    , Override(other.Override)
    , Dir(other.Dir)
    , Distance(other.Distance)
    , Level(other.Level)
    , FinalAction(other.FinalAction)
    , UseCollision(other.UseCollision)
    , IsZTargetting(other.IsZTargetting)
{
}