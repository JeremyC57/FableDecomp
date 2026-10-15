// CCombatSequence_TrollBurrowPursue::OnCreate() -- queue a new CCombatAction_00908160(owner) through a counted pointer, then run the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00907AE0, 126 bytes (incl. padding).
// Callees: operator new 0x00BFEA1A, action constructor 0x00908160, CCountedPointer::Reset 0x00905FD0,
//          AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
//
// Shape: the landed CCombatSequence_TrollRockPursue (0x00907BE0) counted-pointer OnCreate, with a
// one-argument action constructor (0x00908160, its manifest label CCreatureAction_BattleChargeTurnLeft
// is propagated over identical bodies) that takes only the sequence owner at +0x1c. The counted
// pointer is default-constructed (both zero stores scheduled before the allocation), assigned through
// the out-of-line Reset, passed to AddAction by reference, and released inline (count decrement,
// destroy function with the object in ecx, delete of the count block). The engine header for the
// sequence only carries a flattened base blob, so the class is modelled locally.
// The owner is read through an inline const accessor (GetOwner): the plain member read gives the
// same instructions but colours the AddAction argument edx and the delete argument eax (retail: eax, ecx).
struct CThingCreatureBase;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CCombatActionBase {
    virtual void v00();
};

struct CCombatAction_00908160 : CCombatActionBase {
    unsigned char _body[0x110];             // sizeof = 0x114
    CCombatAction_00908160(CThingCreatureBase* owner);   // 0x00908160
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

struct CCombatSequence_TrollBurrowPursue : CCombatSequenceBase_008b0650 {
    virtual void OnCreate();
};

void CCombatSequence_TrollBurrowPursue::OnCreate()
{
    CCountedPointer action;
    action = new CCombatAction_00908160(GetOwner());
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}
