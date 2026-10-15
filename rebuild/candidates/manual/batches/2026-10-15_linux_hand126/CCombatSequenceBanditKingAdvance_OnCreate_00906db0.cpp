// CCombatSequence_BanditKingAdvance::OnCreate() -- queue a new CCreatureAction_BanditKingAdvance(owner, target) through a counted pointer, then run the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00906DB0, 140 bytes (incl. padding).
// Callees: operator new 0x00BFEA1A, base action constructor 0x008BFD00, CCountedPointer::Reset 0x00905FD0,
//          AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
//
// Shape: the same counted-pointer OnCreate as the landed CCombatSequence_TrollRockPursue (0x00907BE0)
// family, but the action class is a thin derived class whose constructor is INLINE: it forwards the
// sequence's +0x1c/+0x20 pointers to the out-of-line base constructor (0x008BFD00) and then stores its
// own vtable (0x0128D924, RTTI CCreatureAction_BanditKingAdvance) -- that is why the new block is held
// in edi across the base call and the vtable store follows it. The counted pointer is default-
// constructed (zero stores before the allocation) and assigned through Reset; its destructor is inline.
// The engine header CCombatSequence_BanditKingAdvance.h only carries a flattened base blob, so the
// sequence is modelled locally with the two pointer members the body reads.
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CCombatActionBase {
    virtual void v00();
};

struct CCombatAction_008bfd00 : CCombatActionBase {
    unsigned char _body[0x110];             // sizeof = 0x114
    CCombatAction_008bfd00(CThingCreatureBase* owner, CThing* target);   // 0x008BFD00
};

struct CCreatureAction_BanditKingAdvance : CCombatAction_008bfd00 {
    CCreatureAction_BanditKingAdvance(CThingCreatureBase* owner, CThing* target)
        : CCombatAction_008bfd00(owner, target) { }
    virtual void v00();                     // vtable 0x0128D924
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
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCombatSequence_BanditKingAdvance : CCombatSequenceBase_008b0650 {
    virtual void OnCreate();
};

void CCombatSequence_BanditKingAdvance::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_BanditKingAdvance(m_pOwner, m_pTarget);
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}
