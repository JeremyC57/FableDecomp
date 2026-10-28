// CAIStateGroup_MeleeCombatNearby::Update() -- melee combat loop: finish the current action, process melee attack messages, unblock movement, register with the combat manager and look at a live enemy, then move to the combat position and attack until the enemy dies, the group stops or is no longer runnable; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096EE90, length 349 bytes (vtable 0x0128D05C slot 12; both exits are
//          `jmp Terminate` tail calls).
// Leaf: ?Update@CAIStateGroup_MeleeCombatNearby@@MAEXXZ
// Callees: CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction @ 0x0093D140 / ShouldGoStraightIntoStrafe @ 0x0093D180
//          / AddStrafeMode @ 0x0093D7A0, CAIStateGroupBase::IsStopping @ 0x008FD010, AlertAlliesInVicinity
//          @ 0x0096EB40 (private bool member of this class, result unused), IsAbleToMove / RemoveMovementBlockingModes @ 0x0085F3E0 / 0x0085F600
//          (__fastcall on the creature), CBaseIntelligentPointer ctor/GetPItem/SetPItem, CAIStateGroupBase::GetPWorld
//          @ 0x008FCFC0, CCombatManager::RegisterCombatant @ 0x006EDC50 (world +0x4c), AIAction_LookAtThing @ 0x0097D6F0
//          (CIntelligentPointer<const CThing> by value, vtable 0x01237F1C), CThingCreatureBase::IsInCombatMode
//          @ 0x006621A0 / IsInMeleeAsAttacker @ 0x00662410, MoveToEnemyCreatureCombatPosition @ 0x0096EC10, AttackEnemy
//          @ 0x0096EA50, virtual IsStillRunnable(bool) (slot 14, 0x0093D800) / OnReadyToAttack (slot 15, 0x0093D720),
//          CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the strafe flag is a bool in bl reset after each AddStrafeMode; the combat manager is fetched through a named
//          creature then world (GetPWorld after the creature load).
// Layout: engine/CAIStateGroup_MeleeCombatNearby.h is flattened (vptr spelled as a data member) and this function makes
//          virtual calls through slots 14/15, so the class is modelled locally (PBrain +0x04, PEnemy +0x14).
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
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
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CCombatManager { public:
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                          // +0x18
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

struct CAIStateGroup_MeleeCombatNearby : CAIStateGroup_MeleeCombatNearbyBase {
    bool Archer;                                // +0x1c
protected:
    virtual void Update();
private:
    bool AlertAlliesInVicinity();               // 0x0096EB40 (Ego_r.pdb ?AlertAlliesInVicinity@CAIStateGroup_MeleeCombatNearby@@AAE_NXZ)
    bool MoveToEnemyCreatureCombatPosition();   // 0x0096EC10
    void AttackEnemy();                         // 0x0096EA50
};

void CAIStateGroup_MeleeCombatNearby::Update()
{
    UpdateCurrentAction();
    if (IsStopping())
        goto stop;
    {
        AlertAlliesInVicinity();
        if (!IsAbleToMove(GetCreature()))
            RemoveMovementBlockingModes(GetCreature());
        if (IS_THING_ALIVE(PEnemy)) {
            CThing* creature = GetCreature();
            CCombatManager* combat = GetPWorld()->PCombatManager;
            combat->RegisterCombatant(creature, PEnemy.GetPItem(), true);
            if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(PEnemy), 0x21, 0.0027777f) == 1)
                goto done;
        }
        bool strafing = GetCreature()->IsInCombatMode();
        if (ShouldGoStraightIntoStrafe()) {
            AddStrafeMode();
            strafing = false;
        }
        do {
            if (MoveToEnemyCreatureCombatPosition()) {
                OnReadyToAttack();
                if (!IsStopping()) {
                    if (strafing) {
                        AddStrafeMode();
                        strafing = false;
                    }
                    AttackEnemy();
                }
                if (!IS_THING_ALIVE(PEnemy) || IsStopping() || !IsStillRunnable(true))
                    break;
            } else {
                Terminate();
                return;
            }
        } while (GetCreature()->IsInMeleeAsAttacker());
    }
    goto done;
stop:
    Terminate();
    return;
done:
    Terminate();
}