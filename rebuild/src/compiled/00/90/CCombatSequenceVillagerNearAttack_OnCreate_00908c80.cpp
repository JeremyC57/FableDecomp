// CCombatSequence_VillagerNearAttack::OnCreate() -- queue an interruptable near attack (action code 150) and a CCombatAction_0097d050-constructed follow-up, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00908C80, 211 bytes, virtual `?OnCreate@CCombatSequence_VillagerNearAttack@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x114-byte actions), CCombatActionBase::CCombatActionBase(owner, thing, code, sound)
//   0x00857F30 under the inlined CCreatureAction_InterruptableNearAttack ctor (vtable 0x01262D94, rtti-vtables.tsv),
//   the second action's ctor 0x0097D050 (the second 0x114-byte action, named by its constructor),
//   CCountedPointer::Reset 0x00905FD0, CCombatSequenceBase::AddAction 0x008B0B10, base OnCreate 0x008B0650,
//   operator delete 0x00BFE9BC (count block in the inlined counted-pointer dtor).
// Layout: sequence owner +0x1c, target +0x20 (as the landed sibling OnCreate rows, e.g. 0x00908780); engine header
//   CCombatSequence_VillagerNearAttack.h is the PDB size only, so the landed sibling model is used.
// Shape: one counted-pointer local reassigned for both actions; the sound pair is the zeroed temporary of the inline ctor.
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
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
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCreatureAction_InterruptableNearAttack : CCombatActionBase {   // vtable 0x01262D94
    unsigned char _body[0x110];
    virtual void v00();
    CCreatureAction_InterruptableNearAttack(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};

struct CCombatAction_0097d050 : CCombatActionBase {
    unsigned char _body[0x110];             // sizeof = 0x114
    CCombatAction_0097d050(CThingCreatureBase* owner, CThing* thing);   // 0x0097D050
};

struct CCombatSequence_VillagerNearAttack : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_VillagerNearAttack::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableNearAttack(GetCreature(), GetTarget());
    AddAction(action);
    action = new CCombatAction_0097d050(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}