// CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, thing, 150, zeroed CSoundPair), own vtable, an empty projectile pointer, then the target thing's current position with the position flag clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7400, length 123 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (the member at +0x114, then the inline CIntelligentPointer vtable 0x01238C6C), CThing::GetPosition @ 0x004C73D0. Own vtable 0x01281694.
// Shape: `ret 8`; the thing argument is kept in ebp for GetPosition; the sound pair is zeroed with immediates; TargetPos (a whole-vector BODY
// assignment through `lea edx,[esi+11Ch]`) and the cleared flag byte follow, as in the three-argument overload 0x008C7390.
// Leaf: ??0CCreatureAction_TrollThrowProjectile@@QAE@PAUCThingCreatureBase@@PAUCThing@@@Z
// The PDB header (CCreatureAction_TrollThrowProjectile.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
struct CThingCreatureBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }                          // leaves the words unset
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor (fixed-offset moves);
};                                                                                 // assignment stays implicit (re-based through `lea`)
struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CSoundPair {
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

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);  // 0x00857F30
    CCombatActionBase(const CCombatActionBase& other);                                                    // 0x0062F3B0
};
struct CCreatureAction_TrollThrowProjectile : CCombatActionBase {   // vtable 0x01281694; retail sizeof 0x130
    CIntelligentPointer PProjectile;        // +0x114 CIntelligentPointer<CThing>
    C3DVector TargetPos;                    // +0x11c
    bool TargetIsPosition;                  // +0x128
    long ThrowFrame;                        // +0x12c
    virtual void v00();
    CCreatureAction_TrollThrowProjectile(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(CThingCreatureBase* owner, CThing* thing)
    : CCombatActionBase(owner, thing, 150, CSoundPair())
{
    TargetPos = thing->GetPosition();
    TargetIsPosition = false;
}