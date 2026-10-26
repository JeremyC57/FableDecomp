// CAIStateGroup_EnemyCreatureBase::Cleanup() -- deregisters a registered combatant from the world's combat manager, runs the slot-17 cleanup hook and forgets the enemy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091D850, length 112 bytes.
// Callees: the creature's combat-registration predicate @ 0x00662410 (thiscall, bool; the manifest's
// C3DAnimationBlendState::AddCombinationBlend label is propagated), the state group's world accessor @ 0x008FCFC0,
// CCombatManager::DeregisterCombatant(CThingCreatureBase*) @ 0x006ED9C0 (the world's manager at +0x4c), `this`'s
// virtual slot 17 (+0x44), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/90/50/0x00A01C10.
// Shape: the creature is read into a named local (edi) BEFORE the world call, and the world is a named local
// too (a chained `GetPWorld()->CombatManager->Deregister(creature)` pushes the creature before the call); then `PEnemy = CIntelligentPointer<CThingCreatureBase>(0)` -- a frame temporary
// through the inline converting constructor (vtable 0x0123E7D8, SetPItem(0)), the inline operator= and the base
// destructor.
// Model: local (engine/CAIStateGroup_EnemyCreatureBase.h flattens the base).
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CThingAICreature : public CThingCreatureBase { public:
};

class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase* creature);     // 0x006ED9C0
};

class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* CombatManager;                  // +0x4c
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                     // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual void OnCleanup();                       // slot 17 (+0x44)
    CAIBrain* PBrain;                               // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                            // 0x008FCFC0
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PEnemy;             // +0x14
    virtual void Cleanup();
};

void CAIStateGroup_EnemyCreatureBase::Cleanup()
{
    if (PBrain->Creature->IsInMeleeAsAttacker()) {
        CThingAICreature* creature = PBrain->Creature;
        CWorld* world = GetPWorld();
        world->CombatManager->DeregisterCombatant((CThingCreatureBase*)creature);
    }
    OnCleanup();
    PEnemy = CIntelligentPointer<CThingCreatureBase>(0);
}