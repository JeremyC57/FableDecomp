// CCombatSequence_WillLightning::OnCreate() -- queue a new CCreatureAction_CastWillLightningSpell(owner, 2, 2.0f) through a counted pointer, then run the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00909140, 133 bytes (incl. padding).
// Callees: operator new 0x00BFEA1A, CCreatureAction_CastWillLightningSpell ctor 0x008BC8C0,
//          CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650,
//          operator delete 0x00BFE9BC.
//
// Shape: the landed counted-pointer OnCreate family (CCombatSequence_TrollRockPursue 0x00907BE0).
// The 0x11C-byte action takes (owner, long, float) per its manifest prototype; the float 2.0f is
// pushed as the immediate 0x40000000. Counted pointer default-constructed, assigned through
// Reset, released inline. The engine header for the sequence only carries a flattened base blob.
// The owner goes through an inline const accessor: a direct member read colours the AddAction
// argument edx and the delete argument eax (retail: eax, ecx), as in TrollBurrowPursue 0x00907AE0.
class CThingCreatureBase;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

class CCombatActionBase { public:
    virtual void v00();
};

class CCreatureAction_CastWillLightningSpell : public CCombatActionBase { public:
    unsigned char _body[0x118];             // sizeof = 0x11c
    CCreatureAction_CastWillLightningSpell(CThingCreatureBase* owner, long level, float duration);   // 0x008BC8C0
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
    void* m_pTarget;                        // +0x20
    CThingCreatureBase* GetOwner() const { return m_pOwner; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCombatSequence_WillLightning : CCombatSequenceBase_008b0650 {
    virtual void OnCreate();
};

void CCombatSequence_WillLightning::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_CastWillLightningSpell(GetOwner(), 2, 2.0f);
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}