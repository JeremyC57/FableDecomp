// CCombatSequence_TrollOpeningGroundPound::OnCreate() -- clear the +0x28 word, queue a new CCombatAction_00908000(owner, target) through a counted pointer, then run the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00907930, 137 bytes (incl. padding).
// Callees: operator new 0x00BFEA1A, action constructor 0x00908000, CCountedPointer::Reset 0x00905FD0,
//          AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
//
// Shape: the landed CCombatSequence_TrollRockPursue (0x00907BE0) counted-pointer OnCreate preceded
// by a body store `m_28 = 0` (the sequence's own word right after the 0x28-byte base, scheduled
// between `push 0x114` and the counted pointer's zero stores). The two-argument action constructor
// @ 0x00908000 (manifest label CCombatAction_StabThing, propagated) takes the owner (+0x1c) and the
// target (+0x20). Counted pointer: default-constructed, assigned through Reset, released inline.
// The engine header for the sequence only carries a flattened base blob, so it is modelled locally.
class CThingCreatureBase;
class CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

class CCombatActionBase { public:
    virtual void v00();
};

struct CCombatAction_00908000 : CCombatActionBase {
    unsigned char _body[0x110];             // sizeof = 0x114
    CCombatAction_00908000(CThingCreatureBase* owner, CThing* target);   // 0x00908000
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object);  // 0x00905FD0
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CCombatSequenceBase_008b0650 {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    unsigned char _pad24[4];
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCombatSequence_TrollOpeningGroundPound : CCombatSequenceBase_008b0650 {
    long m_poundState;                      // +0x28 cleared on create
    virtual void OnCreate();
};

void CCombatSequence_TrollOpeningGroundPound::OnCreate()
{
    m_poundState = 0;
    CCountedPointer action;
    action = new CCombatAction_00908000(m_pOwner, m_pTarget);
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}