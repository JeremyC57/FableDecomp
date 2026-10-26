// Behaviour test for CAIStateGroupBase::AIAction_FleeToThingProximity @ 0x009804A0: proximity loop, started-moving latch, look+move or perform.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
class CThing { public: C3DVector pos; bool dead; };
static int g_directedOk = 0; static C3DVector g_dest;
class CTCCreatureNavigation { public:
    bool GetNearbyDirectedPosition(const C3DVector& from, float radius, const C3DVector& dir, float spread, C3DVector* out)
    { (void)from; (void)radius; (void)dir; (void)spread; if (!g_directedOk) return false; *out = g_dest; return true; }
};
static int g_busy = 0;
class CThingCreatureBase : public CThing { public:
    CTCCreatureNavigation nav;
    const C3DVector& GetPosition() const { return pos; }
    bool IsPerformingAnyAction() const { return g_busy != 0; }
    void GetInterfaceR(CTCCreatureNavigation** out, long type, bool& has) { has = type == 7; if (has) *out = &nav; }
};
struct CIntelligentPointerConstCreature {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    const CThingCreatureBase* GetPCreature() const { return (const CThingCreatureBase*)m_pItem; }
};
struct CIntelligentPointerConstThing {
    const CThing* m_pItem;
    CIntelligentPointerConstThing(const CThing* p) : m_pItem(p) { }
    CIntelligentPointerConstThing(const CIntelligentPointerConstCreature& o) : m_pItem(o.GetPItem()) { }
};
static bool IS_THING_ALIVE(const CIntelligentPointerConstCreature& p) { return p.GetPItem() != 0 && !p.GetPItem()->dead; }
static bool IS_THING_ALIVE(const CThing* p) { return p != 0 && !p->dead; }
static const CThing* g_face = 0;
static const CThing* GetPBestTargetToFaceForCreature(CThingCreatureBase* c) { (void)c; return g_face; }
static const float EPSILON = 0.01f;
static int g_filterLeft = 100;
struct CAIFilterBase { bool Check() const { return g_filterLeft-- > 0; } };
class CCreatureActionBase { public: int performed; };
class CAIBrain { public: CThingCreatureBase* Creature; };
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_RUN = 5 };
static int g_prox[8]; static bool g_proxMoving[8]; static int g_proxN = 0;
static int g_moves = 0; static int g_moveResult = 2; static bool g_lastNotLooking = false;
static int g_looks = 0; static int g_lookResult = 4;
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& moving, EMovementType anim, bool b1,
                                       bool b2, const CThing* target, bool b3, float f)
    { (void)repeat; (void)pos; (void)moving; (void)anim; (void)b2; (void)target; (void)b3; (void)f; ++g_moves; g_lastNotLooking = b1; return (EAIActionReturn)g_moveResult; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity,
                                                 bool& moving, EMovementType anim, bool b1, bool b2, bool b3, bool b4, float f)
    { (void)repeat; (void)thing; (void)proximity; (void)anim; (void)b1; (void)b2; (void)b3; (void)b4; (void)f;
      moving = g_proxMoving[g_proxN]; return (EAIActionReturn)g_prox[g_proxN++]; }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, long frames, float speed)
    { (void)repeat; (void)thing; (void)frames; (void)speed; ++g_looks; return (EAIActionReturn)g_lookResult; }
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action) { ++action->performed; return AI_ACTION_DONE; }
    EAIActionReturn AIAction_FleeToThingProximity(CIntelligentPointerConstCreature threat, float proximity,
                                                  CCreatureActionBase* action, CAIFilterBase* filter);
};
EAIActionReturn CAIStateGroupBase::AIAction_FleeToThingProximity(CIntelligentPointerConstCreature threat, float proximity,
                                                                 CCreatureActionBase* action, CAIFilterBase* filter)
{
    bool moving = false;
    bool startedMoving = false;
    EAIActionReturn result;
    do {
        const CThing* faceTarget = GetPBestTargetToFaceForCreature(GetCreature());
        if (!IS_THING_ALIVE(threat) || !filter->Check())
            return AI_ACTION_ABORTED;
        result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_POLL, threat, proximity, moving, MOVEMENT_RUN, true, false,
                                               true, true, 0.0f);
        if (!IS_THING_ALIVE(threat) || result == AI_ACTION_ABORTED)
            return AI_ACTION_ABORTED;
        if (startedMoving || moving) {
            if (moving)
                startedMoving = true;
        } else if (!GetCreature()->IsPerformingAnyAction()) {
            CThingCreatureBase* creature = GetCreature();
            CTCCreatureNavigation* nav;
            bool found;
            creature->GetInterfaceR(&nav, 7, found);
            C3DVector toward = threat.GetPCreature()->GetPosition() - creature->GetPosition();
            C3DVector dest;
            if (nav->GetNearbyDirectedPosition(GetCreature()->GetPosition(), 5.0f, toward, 0.25277778f, &dest)) {
                CThingCreatureBase* me = GetCreature();
                float tolerance = EPSILON;
                const C3DVector& pos = me->GetPosition();
                float dx = pos.X - dest.X;
                float dy = pos.Y - dest.Y;
                float distanceSquared = dy * dy + dx * dx;
                if (!(distanceSquared < tolerance)) {
                    bool notLooking;
                    if (IS_THING_ALIVE(faceTarget)) {
                        EAIActionReturn lookResult = AIAction_LookAtThing(AI_ACTION_REPEAT_POLL,
                                                                          CIntelligentPointerConstThing(faceTarget), 0x27,
                                                                          0.0027777f);
                        if (lookResult != AI_ACTION_RUNNING && lookResult != AI_ACTION_DONE)
                            return lookResult;
                        notLooking = false;
                    } else
                        notLooking = true;
                    result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, dest, found, MOVEMENT_RUN, notLooking, true, 0, true,
                                                0.0f);
                    continue;
                }
            }
            AIAction_PerformAction(action);
        }
    } while (result == AI_ACTION_RUNNING);
    return result;
}

int main()
{
    int bad = 0;
    CThingCreatureBase threat; threat.pos = C3DVector(10, 0, 0); threat.dead = false;
    CThingCreatureBase me; me.pos = C3DVector(0, 0, 0); me.dead = false;
    CThing face; face.dead = false;
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroupBase sg; sg.PBrain = &brain;
    CIntelligentPointerConstCreature t; t.m_pItem = &threat;
    CCreatureActionBase action; action.performed = 0;
    CAIFilterBase filter;
    /* filter fails at once */
    g_filterLeft = 0;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_ABORTED) ++bad;
    /* moving then not moving: latch keeps the creature from acting; then stopping ends */
    g_filterLeft = 100; g_proxN = 0; g_prox[0] = 4; g_proxMoving[0] = true; g_prox[1] = 4; g_proxMoving[1] = false; g_prox[2] = 1;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_STOPPING || action.performed != 0 || g_proxN != 3) ++bad;
    /* not moving, no directed position: perform action, loop continues on the proximity result */
    g_proxN = 0; g_prox[0] = 4; g_proxMoving[0] = false; g_prox[1] = 2; g_proxMoving[1] = false; g_directedOk = 0;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_DONE || action.performed != 2) ++bad;
    /* directed far away, face target alive, look fails with 3 -> returned */
    action.performed = 0; g_proxN = 0; g_prox[0] = 4; g_proxMoving[0] = false; g_directedOk = 1; g_dest = C3DVector(5, 5, 0);
    g_face = &face; g_lookResult = 3; g_looks = 0;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_ABORTED || g_looks != 1 || g_moves != 0) ++bad;
    /* look running -> move (looking), move result done ends */
    g_proxN = 0; g_lookResult = 4; g_moveResult = 2;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_DONE || g_moves != 1 || g_lastNotLooking) ++bad;
    /* no face target -> move not looking */
    g_proxN = 0; g_face = 0; g_moves = 0;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_DONE || g_moves != 1 || !g_lastNotLooking) ++bad;
    /* destination is where we stand: perform */
    g_proxN = 0; g_prox[0] = 4; g_prox[1] = 1; g_dest = C3DVector(0, 0, 0); action.performed = 0; g_moves = 0;
    if (sg.AIAction_FleeToThingProximity(t, 3.0f, &action, &filter) != AI_ACTION_STOPPING || action.performed != 2 || g_moves != 0) ++bad;
    if (bad == 0) printf("009804a0_TEST PASS\n"); else printf("009804a0_TEST FAIL %d\n", bad);
    return 0;
}