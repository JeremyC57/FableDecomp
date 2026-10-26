// CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(const CCreatureAction_TrollThrowProjectile&) -- copy constructor: base CCombatActionBase(other), own vtable, the projectile pointer, the target position, the flag byte and the throw frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7D80, length 127 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the inline CIntelligentPointer copy constructor,
// landed 0x008C5D40 shape, vtable 0x01238C6C). Own vtable 0x01281694.
// Shape: `ret 4`; memberwise copy in declaration order: the pointer (its address in ebx), TargetPos through C3DVector's memberwise copy constructor
// (three fixed-offset word moves through eax/ecx/edx), the flag byte through al, the last word.
// The PDB header (CCreatureAction_TrollThrowProjectile.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CThingCreatureBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }                          // leaves the words unset
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor (fixed-offset moves);
};                                                                                 // assignment stays implicit (re-based through `lea`)
class CThing { public:
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);  // 0x00857F30
    CCombatActionBase(const CCombatActionBase& other);                                                    // 0x0062F3B0
};
class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:   // vtable 0x01281694; retail sizeof 0x130
    CIntelligentPointer PProjectile;        // +0x114 CIntelligentPointer<CThing>
    C3DVector TargetPos;                    // +0x11c
    bool TargetIsPosition;                  // +0x128
    long ThrowFrame;                        // +0x12c
    virtual void v00();
    CCreatureAction_TrollThrowProjectile(const CCreatureAction_TrollThrowProjectile& other);
};

CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(const CCreatureAction_TrollThrowProjectile& other)
    : CCombatActionBase(other)
    , PProjectile(other.PProjectile)
    , TargetPos(other.TargetPos)
    , TargetIsPosition(other.TargetIsPosition)
    , ThrowFrame(other.ThrowFrame)
{
}