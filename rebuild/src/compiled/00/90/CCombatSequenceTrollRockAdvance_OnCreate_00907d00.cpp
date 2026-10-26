// CCombatSequence_TrollRockAdvance::OnCreate() -- queue walk-into, troll-pursue and walk-out-of actions, then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_TrollRockAdvance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00907D00, 256 bytes (incl. int3 padding).
// Callees: operator new 0x00BFEA1A, CCreatureAction_WalkInto ctor 0x00908400,
//          CCreatureAction_TrollPursue ctor 0x00908340, CCreatureAction_WalkOutOf ctor 0x009083A0, CCountedPointer::Reset 0x00905FD0,
//          CCombatSequenceBase::AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Shape: the landed one-action OnCreate family (0x00907BE0 TrollRockPursue) three times over one counted
// pointer local: it is default-constructed (both words zeroed, the stores scheduled past the first
// allocation's push), each `new X(owner, target)` (0x114-byte actions; ctors called with the sequence's
// owner +0x1c and target +0x20) is assigned through the out-of-line Reset and handed to AddAction; the
// destructor at the end is inlined (decrement the count block, destroy + delete it at zero).
// The engine header CCombatSequence_TrollRockAdvance.h is a flattened 0x2c-byte blob (no members), so
// the sequence is modelled locally like the landed relatives. Owner/target go through inline const
// getters GetCreature()/GetTarget(): direct member reads give a different eax/ecx/edx rotation for the
// AddAction address temporaries (eax,eax,eax instead of retail's eax,ecx,edx).
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

class CCreatureAction_WalkInto : public CCombatActionBase { public:
    unsigned char _body[0x110];             // sizeof = 0x114
    CCreatureAction_WalkInto(CThingCreatureBase* owner, CThing* target);   // 0x00908400
};
class CCreatureAction_TrollPursue : public CCombatActionBase { public:
    unsigned char _body[0x110];
    CCreatureAction_TrollPursue(CThingCreatureBase* owner, CThing* target);  // 0x00908340
};
class CCreatureAction_WalkOutOf : public CCombatActionBase { public:
    unsigned char _body[0x110];
    CCreatureAction_WalkOutOf(CThingCreatureBase* owner, CThing* target);  // 0x009083A0
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

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pCreature;        // +0x1c owner
    CThing* m_pTarget;                      // +0x20 target
    CThingCreatureBase* GetCreature() const { return m_pCreature; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCombatSequence_TrollRockAdvance : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_TrollRockAdvance::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_WalkInto(GetCreature(), GetTarget());
    AddAction(action);
    action = new CCreatureAction_TrollPursue(GetCreature(), GetTarget());
    AddAction(action);
    action = new CCreatureAction_WalkOutOf(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}