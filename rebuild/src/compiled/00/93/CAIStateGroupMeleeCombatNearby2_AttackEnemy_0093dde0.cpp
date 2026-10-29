// CAIStateGroup_MeleeCombatNearby2::AttackEnemy() -- mark the creature ready to attack, then while not stopping (and still runnable, a combatant and within ring range of the target) post a pump-combat-actions brain output looking at the enemy and yield; terminate if the group starts stopping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DDE0, length 479 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (three times), the brain world accessor @ 0x008FCFC0
// (combat manager +0x4c), CCombatManager::SetAttackerAsReadyToAttack @ 0x006EA450, Combat_CheckRingRangeToTarget @
// 0x006E94D0, CAIStateGroupBase::IsStopping @ 0x008FD010, own virtual slot 14 IsStillRunnable(bool), CBaseIntelligentPointer
// ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, operator delete @ 0x00BFE9BC (the
// action count block), CAIBrain::SetBrainOutput @ 0x0088B8F0, the brain's virtual slot 2 (Yield), Terminate @ 0x008FCF40,
// ~CBrainOutput @ 0x006D60C0 (out of line on the stopping exit only).
// Shape: the CBrainOutput constructor and CBrainOutput::SetAsLookAtThing (0x0093E760, this TU) are inline; the output's
// destructor is inlined at the loop's block end and called out of line on the early return (the inline-once band); the
// CIntelligentPointer<const CThing> vtable 0x01237F1C and zero live in ebp/ebx across the loop; the 8-byte look-at
// temporary inside the loop gives the `and esp,-8` frame.
// Layout: CBrainOutput from Ego_r.pdb (landed 0x00835620 / 0x006D60C0 models); the state group is local (the engine
// header flattens the base), PEnemy +0x14.
class CThing;

class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CCreatureActionBase;
struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};
struct CCountedPointer_CCreatureActionBase {    // {object, count block}
    CCreatureActionBase* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    CCountedPointer_CCreatureActionBase() : m_pObject(0), m_pCount(0) { }
    __forceinline ~CCountedPointer_CCreatureActionBase()
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
    __forceinline void Reset(CCreatureActionBase* object)
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = object;
    }
};
class C3DVector { public: float X, Y, Z; };
class CNavigationInput { public:                       // 24 bytes (Ego_r.pdb): Destination is left to the caller
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c (float zero stores are immediates, the long uses the zero register)
    long MovementType;                          // +0x10 EMovementType
    float Speed;                                // +0x14
    CNavigationInput() : Proximity(0.0f), MovementType(0), Speed(0.0f) { }
};
class CBrainOutput { public:
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CIntelligentPointer                 LookAtThing;      // +0x40
    CBrainOutput()
        : NavigationInput(), PAction(), LookAtThing()
    {
        Types = 0;
        LookFlags = 0;
        PAction.Reset(0);
    }
    ~CBrainOutput();                            // 0x006D60C0 (body below, this TU)
    void SetAsLookAtThing(const CThing* thing, long lookFlags)
    {
        Types |= 4;
        {
            CIntelligentPointer pointer;
            pointer.SetPItem(thing);
            LookAtThing.SetPItem(((CThing*)pointer.GetPItem()));
        }
        LookFlags = lookFlags;
    }
};
inline CBrainOutput::~CBrainOutput()
{
}

class CThingCreatureBase { public:
    bool IsRegisteredCombatant();               // 0x00662410
};

class CCombatManager { public:
    void SetAttackerAsReadyToAttack(CThingCreatureBase* attacker, bool ready);   // 0x006EA450
    bool Combat_CheckRingRangeToTarget(CThingCreatureBase* creature);           // 0x006E94D0
};
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() const { return CombatManager; }
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool checkEnemy);  // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    void AttackEnemy();
};

void CAIStateGroup_MeleeCombatNearby2::AttackEnemy()
{
    if (PBrain->PCreature->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->CombatManager->SetAttackerAsReadyToAttack(creature, true);
    }
    while (!IsStopping()) {
        if (!IsStillRunnable(true))
            return;
        if (!PBrain->PCreature->IsRegisteredCombatant())
            return;
        if (!PBrain->PCreature->IsRegisteredCombatant())
            return;
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        if (!world->GetPCombatManager()->Combat_CheckRingRangeToTarget(creature))
            return;
        CBrainOutput output;
        output.Types |= 0x20;
        output.SetAsLookAtThing(((CThing*)PEnemy.GetPItem()), 0x11);
        PBrain->SetBrainOutput(output);
        PBrain->Yield();
        if (IsStopping()) {
            Terminate();
            return;
        }
    }
}