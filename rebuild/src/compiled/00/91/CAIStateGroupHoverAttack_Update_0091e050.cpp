// CAIStateGroup_HoverAttack::Update() -- while the target lives, is to be attacked and the group stays runnable: hover to the combat manager's preferred position facing the target until there or within the combat minimum distance in XY, then perform the next combat action; terminate on any failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091E050, length 534 bytes (+ int3 padding), virtual slot 12.
// Callees: CThingCreatureBase::GetCombatManager @ 0x00661ED0 / IsRegisteredCombatant @ 0x00662410, CAIStateGroupBase::
// GetPWorld @ 0x008FCFC0, CCombatManager::SetAttackerAsReadyToAttack @ 0x006EA450 / GetPreferredPos @ 0x006EA770 /
// GetNextActionForCreature @ 0x006EB600, IsToAttackTarget @ 0x00864D00 (`__fastcall`), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, AIAction_CombatPerformActionAndWait @ 0x0097DA40, Terminate @ 0x008FCF40;
// virtual slots 15 IsRunnableLeaf (+0x3c), 19 GetCombatMinDistance (+0x4c), 22 AIAction_MoveToPosAndFacePos (+0x58).
// Shape: an outer while over the attack conditions, an inner for(;;) hover loop whose in-range flag is reset per outer pass.
// Layout: Ego_r.pdb CAIStateGroup_HoverMovementBase (0x1c bytes): target pointer +0x14; CWorld +0x4c combat manager.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

class CCombatManager;
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing { public:
    CCombatManager* GetCombatManager();         // 0x00661ED0
    bool IsRegisteredCombatant();               // 0x00662410
};
bool __fastcall IsToAttackTarget(CThingCreatureBase* creature, CThing* target);   // 0x00864D00

class CCombatActionBase;
class CCombatManager { public:
    void SetAttackerAsReadyToAttack(CThingCreatureBase* attacker, bool ready);    // 0x006EA450
    C3DVector GetPreferredPos(CThingCreatureBase* creature);                       // 0x006EA770
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase* creature);    // 0x006EB600
};
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1); }

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Update();                      // slot 12
    virtual void v13(); virtual void v14();
    virtual bool IsRunnableLeaf();              // slot 15 (+0x3c)
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual float GetCombatMinDistance();       // slot 19 (+0x4c)
    virtual void v20(); virtual void v21();
    virtual long AIAction_MoveToPosAndFacePos(long repeat, const C3DVector& pos, const C3DVector& facePos, bool& arrived,
                                              long movement);   // slot 22 (+0x58)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
};
struct CAIStateGroup_HoverAttack : CAIStateGroup_HoverMovementBase {
    virtual void Update();
};

void CAIStateGroup_HoverAttack::Update()
{
    CCombatManager* combat = GetCreature()->GetCombatManager();
    if (GetCreature()->IsRegisteredCombatant())
    {
        CThingCreatureBase* creature = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(creature, true);
    }
    while (IsAlive(PTarget) && IsToAttackTarget(GetCreature(), PTarget.GetPItem()) && IsRunnableLeaf()) {
        bool inRange = false;
        for (;;) {
            if (!IsAlive(PTarget) || !GetCreature()->IsRegisteredCombatant()
                || !IsToAttackTarget(GetCreature(), PTarget.GetPItem())) {
                Terminate();
                return;
            }
            bool arrived;
            C3DVector preferred = combat->GetPreferredPos(GetCreature());
            long result = AIAction_MoveToPosAndFacePos(2, preferred, PTarget.GetPItem()->GetPosition(), arrived, 8);
            if (IsAlive(PTarget)) {
                const CThing* me = GetCreature();
                const CThing* target = PTarget.GetPItem();
                float minDistance = GetCombatMinDistance();
                C3DVector offset = target->GetPosition() - me->GetPosition();
                inRange = offset.X * offset.X + offset.Y * offset.Y < minDistance * minDistance;
            }
            if (result == 4) {
                if (inRange)
                    break;
                continue;
            }
            if (result == 2 || inRange)
                break;
            Terminate();
            return;
        }
        if (!IsAlive(PTarget) || !GetCreature()->IsRegisteredCombatant()) {
            Terminate();
            return;
        }
        CCombatActionBase* action = combat->GetNextActionForCreature(GetCreature());
        if (action == 0) {
            Terminate();
            return;
        }
        if (AIAction_CombatPerformActionAndWait(action, false) != 2) {
            Terminate();
            return;
        }
    }
    Terminate();
}