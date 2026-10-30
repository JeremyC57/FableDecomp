// CAIStateGroup_CreatureTakeCover::AttackTargetThing() -- draw the ranged weapon, look at the target, aim (into-aim then aim, with a combat aiming shout between), and fire while the target stays alive and in sight
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the aim-path exit `result != DONE || !IS_THING_ALIVE` written as two
//   exits each with its own Terminate()/return (VC7.1 merges the copies and places the shared block as retail); the parked
//   "known lane gap" label was wrong. Names respelled to Ego_r.pdb (GetWorld, GetCombatManager&, reference action ctors,
//   protected AIAction_*/CanSeeTarget, private IsStillRunnable/AttackTargetThing, IS_THING_ALIVE<T>, GetPItem -> CBaseObject*).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00979F70, 576 bytes. ?AttackTargetThing@CAIStateGroup_CreatureTakeCover@@AAE_NXZ (Ego_r.pdb).
// Shape: LookAtThing and the into-aim face-and-wait pass the target through an inlined base-reference conversion; the aim
//   call copies it through the out-of-line copy ctor 0x0091F990; the dialogue def goes by value into an inline helper built
//   from the raw pointer (gives the memory-form dec and the group spill); creature named before a named GetPWorld().
class CThing;
class CThingCreatureBase;
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& o);   // 0x0091F990
    __forceinline explicit CIntelligentPointer(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    virtual void v01();
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& p);   // 0x006961F0 (IS_THING_ALIVE<CIntelligentPointer<const CThing> >)
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair {
public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
enum EUnsheatheWeaponType { UNSHEATHE_RANGED = 5 };
class CCombatManager;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
class CThingCreatureBase : public CThing {
public:
    CCombatManager& GetCombatManager();         // 0x00661ED0
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CCombatDialogueDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x70 - 8];
    unsigned long RangedCombatAiming;           // +0x70 (PDB +0x74, debug-only base field)
};
class CombatDialogueManager { public:
    unsigned char SpeechGroupInfoMap[0x0c];     // +0x00
    CDefPointer<CCombatDialogueDef> PDialogueDef;   // +0x0c
    CDefPointer<CCombatDialogueDef> GetDialogueDef() const { return PDialogueDef; }
    bool SaySpeech(CThingCreatureBase& talker, unsigned long speechGroup);   // 0x008AFC40
};
__forceinline unsigned long GetRangedAimingGroup(CDefPointer<CCombatDialogueDef> def) { return def->RangedCombatAiming; }
class CCombatManager {
public:
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;      // +0x38
    CDefPointer<CCombatDialogueDef> GetDialogueDef() const { return DialogueManager.PDialogueDef; }
    void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType type);   // 0x006EBDD0
};
class CWorld {
public:
    unsigned char _pad00[0x4c];
    CCombatManager* PThingCombatManager;        // +0x4c
};
class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    unsigned char _members[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool stayOnLastFrame, bool b, bool c, long d, long e);   // 0x008424F0
};
class CCreatureAction_FireMissileWeapon : public CCreatureActionBase {
public:
    unsigned char _members[0xc8 - 0xa8];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase& owner, const CThing* target);   // 0x008D5110
    ~CCreatureAction_FireMissileWeapon();       // 0x0078ADF0
};
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2 };
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    void Terminate();                           // 0x008FCF40
protected:
    CWorld& GetWorld() const;                   // 0x008FCFC0
    __forceinline void SayCombatSpeech(unsigned long group)
    {
        CThingCreatureBase* creature = GetCreature();
        CWorld& world = GetWorld();
        world.PThingCombatManager->DialogueManager.SaySpeech(*creature, group);
    }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames,
                                         float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action,
                                                            CIntelligentPointer<const CThing> target, bool wait);   // 0x0097DCB0
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<const CThing> PTargetThing;   // +0x14
};
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroup_EnemyCreatureBase {
protected:
    bool CanSeeTarget();                        // 0x00970790
};
struct CAIStateGroup_CreatureTakeCover : CAIStateGroup_CreatureUseCoverBase {
private:
    bool IsStillRunnable();                     // 0x00979E90
    bool AttackTargetThing();
};

bool CAIStateGroup_CreatureTakeCover::AttackTargetThing()
{
    CThingCreatureBase* creature = GetCreature();
    creature->GetCombatManager().CreatureUnsheathWeapon(*creature, UNSHEATHE_RANGED);
    if (!IsStillRunnable())
        return false;
    if (AIAction_LookAtThing(AI_ACTION_REPEAT_UNTIL_DONE, CIntelligentPointer<const CThing>(static_cast<const CBaseIntelligentPointer&>(PTargetThing)), 0x21, 0.01f) != AI_ACTION_DONE)
        return false;
    if (PTargetThing.GetPItem() == 0 || (PTargetThing.GetPItem()->m_flags91 & 1))
        return false;
    EAIActionReturn result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(*GetCreature(), "PROJECTILE_WEAPON_INTO_AIM", CSoundPair(), false, true, false, 0, 0),
        CIntelligentPointer<const CThing>(static_cast<const CBaseIntelligentPointer&>(PTargetThing)), false);
    if (result != AI_ACTION_DONE) {
        Terminate();
        return false;
    }
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    unsigned long group = GetRangedAimingGroup(GetWorld().PThingCombatManager->DialogueManager.PDialogueDef.Object);
    SayCombatSpeech(group);
    result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(*GetCreature(), "PROJECTILE_WEAPON_AIM", CSoundPair(), false, true, false, 0, 0),
        PTargetThing, false);
    if (result != AI_ACTION_DONE) {
        Terminate();
        return false;
    }
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    if (!CanSeeTarget())
        return true;
    CThingCreatureBase* shooter = GetCreature();
    result = AIAction_PerformActionAndWait(CCreatureAction_FireMissileWeapon(*shooter, PTargetThing.GetPItem()), false);
    if (result != AI_ACTION_DONE)
        return false;
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    return true;
}