// CCombatSequence_BanditAttackNear::OnCreate() -- queue an interruptable near attack (action code 150) and a leap back (code 50), then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x009067B0, 230 bytes (240 with padding), virtual `?OnCreate@CCombatSequence_BanditAttackNear@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x114-byte actions), CCombatActionBase::CCombatActionBase(owner, thing, code, sound)
//   0x00857F30 under the inlined CCreatureAction_InterruptableNearAttack ctor (vtable 0x01262D94, rtti-vtables.tsv),
//   then the inlined CCreatureAction_LeapBack ctor (code 50, vtable 0x01263024; landed out of line at 0x00906A80),
//   CCountedPointer::Reset 0x00905FD0, CCombatSequenceBase::AddAction 0x008B0B10, base OnCreate 0x008B0650,
//   operator delete 0x00BFE9BC (count block in the inlined counted-pointer dtor).
// Layout: sequence owner +0x1c, target +0x20 (as the landed sibling OnCreate rows, e.g. 0x00908780); engine header
//   CCombatSequence_BanditAttackNear.h is the PDB size only, so the landed sibling model is used.
// Shape: one counted-pointer local reassigned for both actions; the sound pair is the zeroed temporary of the inline ctor.
// Leaf: OnCreate
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

struct CCreatureAction_InterruptableNearAttack : CCombatActionBase {   // vtable 0x01262D94
    unsigned char _body[0x110];
    virtual void v00();
    CCreatureAction_InterruptableNearAttack(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};

struct CCreatureAction_LeapBack : CCombatActionBase {            // vtable 0x01263024
    unsigned char _body[0x110];
    virtual void v00();
    CCreatureAction_LeapBack(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 50, CSoundPair()) { }
};

struct CCombatSequence_BanditAttackNear : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_BanditAttackNear::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableNearAttack(m_pOwner, m_pTarget);
    AddAction(action);
    action = new CCreatureAction_LeapBack(m_pOwner, m_pTarget);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}