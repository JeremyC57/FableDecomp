// CAIStateGroup_CreatureTakeCover::AttackTargetThing() -- draw the ranged weapon, look at the target, aim (into-aim then aim, with a combat aiming shout between), and fire while the target stays alive and in sight
// Grade: DIFFER(571v571) -- only the block placement of the shared Terminate block (known lane gap): retail falls from the last
//   alive test into Terminate and keeps `mov ecx,esi` above the aim-path je; ours places Terminate after the return-true epilogue.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00979F70, 576 bytes. ?AttackTargetThing@CAIStateGroup_CreatureTakeCover@@AAE_NXZ (Ego_r.pdb).
// Shape: LookAtThing and the into-aim face-and-wait pass the target through an inlined base-reference conversion; the aim
//   call copies it through the out-of-line copy ctor 0x0091F990; the dialogue def goes by value into an inline helper built
//   from the raw pointer (gives the memory-form dec and the group spill); creature named before a named GetPWorld().
struct CThing;
struct CThingCreatureBase;
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& o);   // 0x0091F990
    __forceinline explicit CIntelligentPointer(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
    virtual void v01();
};
struct CThingPhysical;
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<const CThing>& p);   // 0x006961F0
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
enum EUnsheatheWeaponType { UNSHEATHE_RANGED = 5 };
struct CCombatManager;
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
struct CThingCreatureBase : CThing {
    CCombatManager* GetCombatManager();         // 0x00661ED0
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CCombatDialogueDef : CDefPointeeBase {
    unsigned char _pad08[0x70 - 8];
    unsigned long RangedCombatAiming;           // +0x70 (PDB +0x74, debug-only base field)
};
struct CombatDialogueManager {
    unsigned char SpeechGroupInfoMap[0x0c];     // +0x00
    CDefPointer<CCombatDialogueDef> PDialogueDef;   // +0x0c
    CDefPointer<CCombatDialogueDef> GetDialogueDef() const { return PDialogueDef; }
    bool SaySpeech(CThingCreatureBase& talker, unsigned long speechGroup);   // 0x008AFC40
};
__forceinline unsigned long GetRangedAimingGroup(CDefPointer<CCombatDialogueDef> def) { return def->RangedCombatAiming; }
struct CCombatManager {
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;      // +0x38
    CDefPointer<CCombatDialogueDef> GetDialogueDef() const { return DialogueManager.PDialogueDef; }
    void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType type);   // 0x006EBDD0
};
struct CWorld {
    unsigned char _pad00[0x4c];
    CCombatManager* PThingCombatManager;        // +0x4c
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _members[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool stayOnLastFrame, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    unsigned char _members[0xc8 - 0xa8];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase* owner, CThing* target);   // 0x008D5110
    ~CCreatureAction_FireMissileWeapon();       // 0x0078ADF0
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2 };
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld() const;                  // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    __forceinline void SayCombatSpeech(unsigned long group)
    {
        CThingCreatureBase* creature = GetCreature();
        CWorld* world = GetPWorld();
        world->PThingCombatManager->DialogueManager.SaySpeech(*creature, group);
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
    bool CanSeeTarget();                        // 0x00970790
};
struct CAIStateGroup_CreatureTakeCover : CAIStateGroup_CreatureUseCoverBase {
    bool IsStillRunnable();                     // 0x00979E90
    bool AttackTargetThing();
};

bool CAIStateGroup_CreatureTakeCover::AttackTargetThing()
{
    CThingCreatureBase* creature = GetCreature();
    creature->GetCombatManager()->CreatureUnsheathWeapon(*creature, UNSHEATHE_RANGED);
    if (!IsStillRunnable())
        return false;
    if (AIAction_LookAtThing(AI_ACTION_REPEAT_UNTIL_DONE, CIntelligentPointer<const CThing>(static_cast<const CBaseIntelligentPointer&>(PTargetThing)), 0x21, 0.01f) != AI_ACTION_DONE)
        return false;
    if (PTargetThing.GetPItem() == 0 || (PTargetThing.GetPItem()->m_flags91 & 1))
        return false;
    EAIActionReturn result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(GetCreature(), "PROJECTILE_WEAPON_INTO_AIM", CSoundPair(), false, true, false, 0, 0),
        CIntelligentPointer<const CThing>(static_cast<const CBaseIntelligentPointer&>(PTargetThing)), false);
    if (result != AI_ACTION_DONE) {
        Terminate();
        return false;
    }
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    unsigned long group = GetRangedAimingGroup(GetPWorld()->PThingCombatManager->DialogueManager.PDialogueDef.Object);
    SayCombatSpeech(group);
    result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(GetCreature(), "PROJECTILE_WEAPON_AIM", CSoundPair(), false, true, false, 0, 0),
        PTargetThing, false);
    if (result != AI_ACTION_DONE || !IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    if (!CanSeeTarget())
        return true;
    CThingCreatureBase* shooter = GetCreature();
    result = AIAction_PerformActionAndWait(CCreatureAction_FireMissileWeapon(shooter, PTargetThing.GetPItem()), false);
    if (result != AI_ACTION_DONE)
        return false;
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    return true;
}
