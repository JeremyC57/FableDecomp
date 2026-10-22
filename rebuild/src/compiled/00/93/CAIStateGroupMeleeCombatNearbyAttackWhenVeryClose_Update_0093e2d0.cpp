// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Update() -- with a living enemy: register as its combatant, refresh perception, strafe when in combat mode, signal ready-to-attack and mark the creature ready; then while the enemy lives, the group neither stops nor stops being runnable and the creature stays a combatant, post a look-at-enemy brain output and yield; finally clear the ready-to-attack mark and terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E2D0, length 654 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::RegisterCombatant @ 0x006EDC50 /
// SetAttackerAsReadyToAttack @ 0x006EA450 (world +0x4c), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
// 0x34 = CTCPerceiveThing, bit 20 of +0x24, its virtual slot 9), CThingCreatureBase::IsInCombatMode @ 0x006621A0 /
// IsCombatant @ 0x00662410, AddStrafeMode @ 0x0093D7A0, own virtual slots 14 IsStillRunnable(bool) / 15 OnReadyToAttack,
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0 / IsStopping @ 0x008FD010 / Terminate @ 0x008FCF40, operator delete
// @ 0x00BFE9BC (the action count block), CAIBrain::SetBrainOutput @ 0x0088B8F0, the brain's virtual slot 2 (Yield),
// ~CBrainOutput @ 0x006D60C0 (out of line on the stopping exit only).
// Shape: the CBrainOutput model of the landed sibling 0x0093DDE0 (inline ctor and SetAsLookAtThing, inline-once dtor);
// the enemy pointer address stays in ebx, zero in ebp (also the `false` of the final SetAttackerAsReadyToAttack).
// Layout: engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h flattens the base, and the function makes virtual
// calls through slots 14/15, so the class is modelled locally (PBrain +0x04, PEnemy +0x14).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCombatManager;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    CCombatManager* GetCombatManager();         // 0x00661ED0
    bool IsInCombatMode();                      // 0x006621A0
    bool IsCombatant();                         // 0x00662410
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCreatureActionBase;
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
struct C3DVector { float X, Y, Z; };
struct CNavigationInput {                       // 24 bytes (Ego_r.pdb): Destination is left to the caller
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c (float zero stores are immediates, the long uses the zero register)
    long MovementType;                          // +0x10 EMovementType
    float Speed;                                // +0x14
    CNavigationInput() : Proximity(0.0f), MovementType(0), Speed(0.0f) { }
};
struct CBrainOutput {
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
            LookAtThing.SetPItem(pointer.GetPItem());
        }
        LookFlags = lookFlags;
    }
};
inline CBrainOutput::~CBrainOutput()
{
}

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCPerceiveThing {                       // interface 0x34
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Refresh();                     // slot 9 (+0x24, descriptive)
};
struct CCombatManager {
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
    void SetAttackerAsReadyToAttack(CThing* creature, bool ready);         // 0x006EA450
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThing* PCreature;                          // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11
    virtual void Update();                      // slot 12
    virtual void v13();
    virtual bool IsStillRunnable(bool flag);    // slot 14 (+0x38)
    virtual void OnReadyToAttack();             // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    void AddStrafeMode();                       // 0x0093D7A0
};
struct CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose : CAIStateGroup_MeleeCombatNearbyBase {
    virtual void Update();
};

void CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Update()
{
    if (!IS_THING_ALIVE(PEnemy)) {
        Terminate();
        return;
    }
    CThing* creature = GetCreature();
    creature->GetCombatManager()->RegisterCombatant(creature, PEnemy.GetPItem(), true);
    CTCPerceiveThing* perceive;
    if (GetCreature()->GetInterface(&perceive, 0x34))
        perceive->Refresh();
    if (GetCreature()->IsInCombatMode())
        AddStrafeMode();
    OnReadyToAttack();
    if (GetCreature()->IsCombatant()) {
        CThing* attacker = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(attacker, true);
    }
    while (IS_THING_ALIVE(PEnemy) && !IsStopping() && IsStillRunnable(true) && GetCreature()->IsCombatant()) {
        CBrainOutput output;
        output.Types |= 0x20;
        output.SetAsLookAtThing(PEnemy.GetPItem(), 0x11);
        PBrain->SetBrainOutput(output);
        PBrain->Yield();
        if (IsStopping()) {
            Terminate();
            return;
        }
    }
    if (GetCreature()->IsCombatant()) {
        CThing* attacker = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(attacker, false);
    }
    Terminate();
}