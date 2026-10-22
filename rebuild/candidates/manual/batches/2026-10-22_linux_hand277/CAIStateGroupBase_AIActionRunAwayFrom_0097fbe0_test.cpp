// Behaviour test for CAIStateGroupBase::AIAction_RunAwayFrom @ 0x0097FBE0: abort/done exits, directed run, wander retry, idle.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CThing {
    C3DVector pos; bool dead; unsigned char m_flags6c;
    const C3DVector& GetPosition() const { return pos; }
};
static int g_awareLeft = 0; static int g_directedOk = 0; static int g_wanderOk = 0;
static int g_moveResults[8]; static int g_moveN = 0; static int g_lastRepeat = -1; static C3DVector g_lastDir;
static int g_idle = 0; static int g_idleResult = 2;
struct CTCCreatureNavigation {
    bool GetNearbyDirectedPosition(const C3DVector& from, float radius, const C3DVector& dir, float spread, C3DVector* out)
    { (void)from; (void)radius; (void)spread; g_lastDir = dir; if (!g_directedOk) return false; *out = C3DVector(9, 9, 9); return true; }
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool flag)
    { (void)from; (void)radius; (void)flag; if (!g_wanderOk) return false; *out = C3DVector(7, 7, 7); return true; }
};
struct CThingCreatureBase : CThing {
    CTCCreatureNavigation nav;
    static float GetPersonalSpaceRadius() { return 2.0f; }
    void GetInterfaceR(CTCCreatureNavigation** out, long type, bool& has) { has = type == 7; if (has) *out = &nav; }
};
struct CIntelligentPointerConstThing { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CIntelligentPointerConstCreature { CThing* m_pItem; CIntelligentPointerConstCreature(CThing* p) : m_pItem(p) { } CThing* GetPItem() const { return m_pItem; } };
template <class P> static bool IS_THING_ALIVE(const P& p) { return p.GetPItem() != 0 && !p.GetPItem()->dead; }
static bool IsAwareOfThingImplementation(CThingCreatureBase* c, CThing* t, bool f) { (void)c; (void)t; (void)f; if (g_awareLeft > 0) { --g_awareLeft; return true; } return false; }
struct CAIBrain { CThingCreatureBase* Creature; };
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_RUN = 5 };
struct CCreatureActionBase;
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& moving, EMovementType anim, bool b1,
                                       bool b2, const CThing* target, bool b3, float f)
    { (void)pos; (void)anim; (void)b1; (void)b2; (void)target; (void)b3; (void)f; moving = true; g_lastRepeat = repeat; return (EAIActionReturn)g_moveResults[g_moveN++]; }
    EAIActionReturn AIAction_DoNothing() { ++g_idle; return (EAIActionReturn)g_idleResult; }
    EAIActionReturn AIAction_RunAwayFrom(CIntelligentPointerConstThing threat, CCreatureActionBase* action);
};
EAIActionReturn CAIStateGroupBase::AIAction_RunAwayFrom(CIntelligentPointerConstThing threat, CCreatureActionBase* action)
{
    (void)action;
    if (!IS_THING_ALIVE(threat) || !(threat.GetPItem()->m_flags6c & 8))
        return AI_ACTION_ABORTED;
    CIntelligentPointerConstCreature enemy(threat.GetPItem());
    for (;;) {
        if (!IS_THING_ALIVE(enemy) || !IsAwareOfThingImplementation(GetCreature(), enemy.GetPItem(), false))
            return AI_ACTION_DONE;
        C3DVector away = GetCreature()->GetPosition() - enemy.GetPItem()->GetPosition();
        CThingCreatureBase* creature = GetCreature();
        CTCCreatureNavigation* nav;
        C3DVector dest;
        EAIActionReturn result;
        bool moving;
        creature->GetInterfaceR(&nav, 7, moving);
        if (nav->GetNearbyDirectedPosition(creature->GetPosition(), 5.0f, away, 0.25277778f, &dest)) {
            result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, dest, moving, MOVEMENT_RUN, true, true, 0, true, 0.0f);
        } else {
            const CThing* threatThing = enemy.GetPItem();
            CThingCreatureBase* me = GetCreature();
            float radius = CThingCreatureBase::GetPersonalSpaceRadius();
            const C3DVector& from = threatThing->GetPosition();
            const C3DVector& to = me->GetPosition();
            float dx = to.X - from.X;
            float dy = to.Y - from.Y;
            float distanceSquared = dy * dy + dx * dx;
            float radiusSquared = radius * radius;
            if (distanceSquared < radiusSquared
                && nav->GetNearbyWanderPosition(enemy.GetPItem()->GetPosition(), 5.0f, &dest, false)) {
                result = AIAction_MoveToPos(AI_ACTION_REPEAT_UNTIL_DONE, dest, moving, MOVEMENT_RUN, true, true, 0, true, 0.0f);
                if (result == AI_ACTION_DONE)
                    continue;
                return result;
            }
            result = AIAction_DoNothing();
        }
        if (result == AI_ACTION_RUNNING)
            continue;
        return result;
    }
}
int main()
{
    int bad = 0;
    CThing threatThing; threatThing.pos = C3DVector(1, 1, 0); threatThing.dead = false; threatThing.m_flags6c = 8;
    CThingCreatureBase me; me.pos = C3DVector(2, 3, 0); me.dead = false; me.m_flags6c = 0;
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroupBase sg; sg.PBrain = &brain;
    CIntelligentPointerConstThing t; t.m_pItem = &threatThing;
    threatThing.m_flags6c = 0;
    if (sg.AIAction_RunAwayFrom(t, 0) != AI_ACTION_ABORTED) ++bad;
    threatThing.m_flags6c = 8;
    g_awareLeft = 0;
    if (sg.AIAction_RunAwayFrom(t, 0) != AI_ACTION_DONE) ++bad;
    /* directed: running twice then stopping */
    g_awareLeft = 10; g_directedOk = 1; g_moveN = 0; g_moveResults[0] = 4; g_moveResults[1] = 4; g_moveResults[2] = 1;
    if (sg.AIAction_RunAwayFrom(t, 0) != AI_ACTION_STOPPING || g_moveN != 3 || g_lastRepeat != 2) ++bad;
    if (g_lastDir.X != 1 || g_lastDir.Y != 2) ++bad;
    /* inside personal space (dist^2 = 5 > 4: outside) -> idle */
    g_directedOk = 0; g_wanderOk = 1; g_moveN = 0; g_idle = 0; g_idleResult = 3; g_awareLeft = 10;
    if (sg.AIAction_RunAwayFrom(t, 0) != AI_ACTION_ABORTED || g_idle != 1 || g_moveN != 0) ++bad;
    /* inside: wander until-done; DONE loops, then RUNNING returned */
    me.pos = C3DVector(1.5f, 1.5f, 0); g_moveResults[0] = 2; g_moveResults[1] = 4; g_awareLeft = 10;
    if (sg.AIAction_RunAwayFrom(t, 0) != AI_ACTION_RUNNING || g_moveN != 2 || g_lastRepeat != 1) ++bad;
    if (bad == 0) printf("0097fbe0_TEST PASS\n"); else printf("0097fbe0_TEST FAIL %d\n", bad);
    return 0;
}
