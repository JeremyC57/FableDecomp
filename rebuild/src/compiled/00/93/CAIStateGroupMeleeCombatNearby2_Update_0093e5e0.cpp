// CAIStateGroup_MeleeCombatNearby2::Update() -- melee combat loop: finish the current action, register with the combat manager against a live enemy, refresh perception, queue the combat message, then while the enemy lives and the group runs: move to the combat position, attack and mark the creature ready to attack again; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E5E0, length 384 bytes (+ int3 padding).
// Leaf: ?Update@CAIStateGroup_MeleeCombatNearby2@@MAEXXZ
// Callees: CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction @ 0x0093D140 / ShouldGoStraightIntoStrafe @ 0x0093D180
//          / AddStrafeMode @ 0x0093D7A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy +0x14), CAIStateGroupBase::
//          GetPWorld @ 0x008FCFC0, CCombatManager::RegisterCombatant @ 0x006EDC50 / SetAttackerAsReadyToAttack @ 0x006EA450
//          (world +0x4c), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of
//          +0x24, its virtual slot 9), AlertAlliesInVicinity @ 0x0093DFC0 (bool, result unused), CThingCreatureBase::IsInCombatMode
//          @ 0x006621A0 / IsInMeleeAsAttacker @ 0x00662410, CAIStateGroupBase::IsStopping @ 0x008FD010, virtual IsStillRunnable(bool) (slot 14)
//          / OnReadyToAttack (slot 15), MoveToEnemyCreatureCombatPosition @ 0x0093E090, AttackEnemy @ 0x0093DDE0,
//          CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the early dead-enemy exit comes before the ebp/edi pushes; the strafe flag lives in bl; the loop re-tests the
//          enemy, the stop flag, IsStillRunnable and combat at its head.
// Layout: engine/CAIStateGroup_MeleeCombatNearby2.h is flattened (vptr spelled as a data member) and this function makes
//          virtual calls through slots 14/15, so the class is modelled locally (PBrain +0x04, PEnemy +0x14).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
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
    bool IsInCombatMode();                      // 0x006621A0
};
class CThingCreatureBase : public CThing { public:
    bool IsInMeleeAsAttacker() const;           // 0x00662410 (Ego_r.pdb ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ)
};
bool __fastcall IsAbleToMove(CThing* thing);                  // 0x0085F3E0
void __fastcall RemoveMovementBlockingModes(CThing* thing);   // 0x0085F600

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCPerceiveThing { public:                       // interface 0x34
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Refresh();                     // slot 9 (+0x24, descriptive)
};
class CCombatManager { public:
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
    void SetAttackerAsReadyToAttack(CThing* creature, bool ready);         // 0x006EA450
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase { public:
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
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    void UpdateCurrentAction();                 // 0x0093D140
    bool ShouldGoStraightIntoStrafe();          // 0x0093D180
    void AddStrafeMode();                       // 0x0093D7A0
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroup_MeleeCombatNearbyBase {
    bool Archer;                                // +0x1c
protected:
    virtual void Update();
private:
    bool AlertAlliesInVicinity();               // 0x0093DFC0 (Ego_r.pdb ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby2@@AAE_NXZ)
    bool MoveToEnemyCreatureCombatPosition();   // 0x0093E090
    void AttackEnemy();                         // 0x0093DDE0
};

void CAIStateGroup_MeleeCombatNearby2::Update()
{
    UpdateCurrentAction();
    if (!IS_THING_ALIVE(PEnemy)) {
        Terminate();
        return;
    }
    CThing* creature = GetCreature();
    CCombatManager* combat = GetPWorld()->PCombatManager;
    combat->RegisterCombatant(creature, PEnemy.GetPItem(), true);
    CTCPerceiveThing* perceive;
    if (GetCreature()->GetInterface(&perceive, 0x34))
        perceive->Refresh();
    AlertAlliesInVicinity();
    bool strafing = GetCreature()->IsInCombatMode();
    if (ShouldGoStraightIntoStrafe()) {
        AddStrafeMode();
        strafing = false;
    }
    for (;;) {
        if (!IS_THING_ALIVE(PEnemy) || IsStopping() || !IsStillRunnable(true) || !GetCreature()->IsInMeleeAsAttacker())
            break;
        if (!MoveToEnemyCreatureCombatPosition()) {
            Terminate();
            return;
        }
        OnReadyToAttack();
        if (IsStopping())
            continue;
        if (strafing) {
            AddStrafeMode();
            strafing = false;
        }
        AttackEnemy();
        if (!GetCreature()->IsInMeleeAsAttacker())
            continue;
        CThing* attacker = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(attacker, false);
    }
    Terminate();
}