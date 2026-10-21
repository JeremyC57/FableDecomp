// CCombatSequenceJacksCombo::OnCreate() -- queue Jack's three-hit combo: three CCreatureAction_InterruptableMidAttack(owner, target) actions (code 150, no sounds) through AddAction, then the base OnCreate; the counted-pointer temporary is released inline
// Leaf: ?OnCreate@CCombatSequenceJacksCombo@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090A1F0, 321 bytes (336 with padding).
// Callees: operator new 0x00BFEA1A (0x118 bytes), CCombatActionBase(CThingCreatureBase*, CThing*, long, const
//          CSoundPair&) 0x00857F30 (CCreatureAction_InterruptableMidAttack vtable 0x01262EDC, RTTI), the counted
//          pointer's Reset 0x00905FD0, CCombatSequenceBase::AddAction 0x008B0B10 (label std::vector::push_back is
//          propagated), CCombatSequenceBase::OnCreate 0x008B0650, operator delete 0x00BFE9BC.
//
// Shape: the landed CCombatSequence OnCreate family (0x009067B0): one default-constructed counted pointer
// assigned three times through Reset, the action's inline ctor passing a zeroed CSoundPair temporary; the
// zero register (ebx) is hoisted over all three allocations.
// Layout: local model (engine/CCombatSequenceJacksCombo.h is a flattened 0x2c-byte base blob); CCombatSequenceBase
// owner +0x1c / target +0x20 as the landed family.
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CSoundPair {                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
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

struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCreatureAction_InterruptableMidAttack : CCombatActionBase {   // vtable 0x01262EDC
    unsigned char _body[0x114];             // sizeof = 0x118
    virtual void v00();
    CCreatureAction_InterruptableMidAttack(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};

struct CCombatSequenceJacksCombo : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequenceJacksCombo::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}