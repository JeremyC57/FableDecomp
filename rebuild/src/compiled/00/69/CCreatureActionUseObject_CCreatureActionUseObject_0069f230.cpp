// CCreatureAction_UseObject::CCreatureAction_UseObject(CThingCreatureBase*, CThing*, bool) -- base CCreatureActionBase(creature, zeroed CSoundPair, 0, 0), own vtable, PUsedThing pointed at the used thing, the rest cleared and AllowTurn taken from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F230, length 125 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtables (rtti-vtables.tsv): CCreatureAction_UseObject 0x0125CEFC, CIntelligentPointer<CThing> 0x01238C6C.
// Layout (Ego_r.pdb, retail offsets 0xc lower; retail CCreatureActionBase is 0xa8 bytes): PUsedThing +0xa8,
// PreviousPhysicsComponent +0xb0, Dummy (CRightHandedSet, no constructor code) +0xb4, DummyPos +0xcc, Proximity +0xd8,
// UsingDummy/MovingToDummy/TurningToDummy/AllowTurn +0xdc..+0xdf.
// Shape: one zero register (ebx) feeds the two pushed base arguments, the sound pair and every cleared member. Proximity
// is stored from that integer register too, so it is modelled as a dword (a float member zeroed in an initialiser is an
// immediate store).
struct CThing;
struct CThingCreatureBase;
struct CTCPhysicsBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long a, long b);   // 0x00693C70
};

struct CCreatureAction_UseObject : CCreatureActionBase {
    CIntelligentPointer PUsedThing;         // +0xa8
    CTCPhysicsBase* PreviousPhysicsComponent;   // +0xb0
    unsigned char Dummy[0x18];              // +0xb4 CRightHandedSet
    float DummyPos[3];                      // +0xcc C3DVector
    long Proximity;                         // +0xd8 (float in the PDB; zeroed through the integer register)
    bool UsingDummy;                        // +0xdc
    bool MovingToDummy;                     // +0xdd
    bool TurningToDummy;                    // +0xde
    bool AllowTurn;                         // +0xdf
    virtual void v00();
    CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* usedThing, bool allowTurn);
};

CCreatureAction_UseObject::CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* usedThing, bool allowTurn)
    : CCreatureActionBase(creature, CSoundPair(), 0, 0)
    , PUsedThing(usedThing)
    , PreviousPhysicsComponent(0)
    , Proximity(0)
    , UsingDummy(false)
    , MovingToDummy(false)
    , TurningToDummy(false)
    , AllowTurn(allowTurn)
{
}