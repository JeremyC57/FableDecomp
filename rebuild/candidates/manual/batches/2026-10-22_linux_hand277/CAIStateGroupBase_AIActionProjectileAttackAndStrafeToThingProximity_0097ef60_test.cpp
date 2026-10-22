// Behaviour test for CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToThingProximity @ 0x0097EF60: strafe mode, repeat loop, missile action and def release.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CDefCounted {
    long m_refCount; int deleted;
    CDefCounted() : m_refCount(1), deleted(0) {}
    virtual ~CDefCounted() {}
    void DeleteThis() { ++deleted; }
};
struct CAICreatureWillPowerIndicatorDef : CDefCounted { bool UseStrafe; };
struct CTCCreatureModeManager {
    unsigned long modes; int adds, removes;
    bool HasMode(unsigned long m) { return (modes >> m) & 1; }
    bool AddMode(unsigned long m) { modes |= 1UL << m; ++adds; return true; }
    void RemoveMode(unsigned long m) { modes &= ~(1UL << m); ++removes; }
};
struct CThing { bool dead; };
struct CThingCreatureBase;
struct CCombatTypeDef {
    CAICreatureWillPowerIndicatorDef* indicator;
    bool GetSubDef(CAICreatureWillPowerIndicatorDef** out) const
    {
        if (!indicator) return false;
        if (*out != indicator) { *out = indicator; ++indicator->m_refCount; }
        return true;
    }
};
struct CCreatureAction_FireMissileWeapon { CThingCreatureBase* creature; const CThing* target;
    CCreatureAction_FireMissileWeapon(CThingCreatureBase* c, const CThing* t) : creature(c), target(t) {} };
struct CThingCreatureBase : CThing {
    CCombatTypeDef combat; CTCCreatureModeManager modeManager; int actions; const CThing* lastTarget;
    CCombatTypeDef* GetCombatTypeDef() { return &combat; }
    void GetInterfaceT(CTCCreatureModeManager** out, long type) { if (type == 0x31) *out = &modeManager; }
    bool SetCurrentAction(CCreatureAction_FireMissileWeapon* a) { ++actions; lastTarget = a->target; return true; }
};
struct CIntelligentPointerConstThing {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static bool IS_THING_ALIVE(const CIntelligentPointerConstThing& p) { return p.GetPItem() != 0 && !p.GetPItem()->dead; }
struct CAIBrain { CThingCreatureBase* Creature; };
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT };
static int g_moves = 0; static int g_runningLeft = 0; static bool g_lastNotStrafing = false;
static EAIActionReturn NextMove(bool notStrafing)
{
    ++g_moves; g_lastNotStrafing = notStrafing;
    if (g_runningLeft > 0) { --g_runningLeft; return AI_ACTION_RUNNING; }
    return AI_ACTION_DONE;
}
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity, bool& speed, EMovementType anim, bool strafe, bool b2, bool extra, bool b4, float f)
    { (void)repeat; (void)thing; (void)proximity; (void)speed; (void)anim; (void)b2; (void)extra; (void)b4; (void)f; return NextMove(strafe); }
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing moveTo, CIntelligentPointerConstThing target, float proximity, bool& speed, EMovementType anim, bool extra);
};
EAIActionReturn CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing moveTo, CIntelligentPointerConstThing target, float proximity, bool& speed, EMovementType anim, bool extra)
{
    bool useStrafe = false;
    CAICreatureWillPowerIndicatorDef* indicator = 0;
    if (GetCreature()->GetCombatTypeDef()->GetSubDef(&indicator))
        useStrafe = indicator->UseStrafe;
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterfaceT(&modes, 0x31);
    if (useStrafe && !modes->HasMode(0x19))
        modes->AddMode(0x19);
    bool notStrafing = true;
    if (modes->HasMode(0x19))
        notStrafing = false;
    EAIActionReturn result;
    do
        result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_POLL, moveTo, proximity, speed, anim, notStrafing, false, extra, true, 0.0f);
    while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    if (useStrafe) {
        if (result != AI_ACTION_RUNNING)
            modes->RemoveMode(0x19);
        if (IS_THING_ALIVE(target)) {
            CThingCreatureBase* creature = GetCreature();
            CCreatureAction_FireMissileWeapon action(creature, target.GetPItem());
            creature->SetCurrentAction(&action);
        }
    }
    if (indicator)
        if (--indicator->m_refCount == 0)
            indicator->DeleteThis();
    return result;
}
int main()
{
    int bad = 0;
    CThing enemy; enemy.dead = false;
    CThingCreatureBase me; memset(&me, 0, sizeof(me));
    CAICreatureWillPowerIndicatorDef ind; ind.UseStrafe = true; ind.m_refCount = 0;
    me.combat.indicator = &ind;
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroupBase sg; sg.PBrain = &brain;
    CIntelligentPointerConstThing target; target.m_pItem = &enemy;
    bool speed = false;
    g_runningLeft = 2;
    EAIActionReturn r = sg.AIAction_ProjectileAttackAndStrafeToThingProximity(AI_ACTION_REPEAT_UNTIL_DONE, target, target, 2.0f, speed, MOVEMENT_DEFAULT, false);
    if (r != AI_ACTION_DONE || g_moves != 3 || g_lastNotStrafing) ++bad;
    if (me.modeManager.adds != 1 || me.modeManager.removes != 1 || me.modeManager.modes != 0) ++bad;
    if (me.actions != 1 || me.lastTarget != &enemy || ind.deleted != 1) ++bad;
    /* no indicator: no strafe, poll once, no action */
    me.combat.indicator = 0; g_moves = 0; g_runningLeft = 3; me.actions = 0;
    r = sg.AIAction_ProjectileAttackAndStrafeToThingProximity(AI_ACTION_REPEAT_ONCE, target, target, 2.0f, speed, MOVEMENT_DEFAULT, false);
    if (r != AI_ACTION_RUNNING || g_moves != 1 || !g_lastNotStrafing || me.actions != 0) ++bad;
    /* strafe but target dead, still running: mode kept */
    me.combat.indicator = &ind; ind.m_refCount = 1; ind.deleted = 0; enemy.dead = true; g_moves = 0; g_runningLeft = 1;
    r = sg.AIAction_ProjectileAttackAndStrafeToThingProximity(AI_ACTION_REPEAT_ONCE, target, target, 2.0f, speed, MOVEMENT_DEFAULT, false);
    if (r != AI_ACTION_RUNNING || me.modeManager.modes == 0 || me.actions != 0 || ind.deleted != 0 || ind.m_refCount != 1) ++bad;
    if (bad == 0) printf("0097ef60_TEST PASS\n"); else printf("0097ef60_TEST FAIL %d\n", bad);
    return 0;
}
