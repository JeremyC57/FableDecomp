// Behaviour test for CAIStateGroup_MinionWander::Update (0x0092C070): a copy of the model with scripted stub actions.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
static int g_released;
struct CCreatureNavigationDef {
    long RefCount;
    bool AvoidDynamicObstacles;
    void DeleteThis() { ++g_released; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerNav { CCreatureNavigationDef* m_p; CDefPointerNav() : m_p(0) { } };
struct CNavDefHolder {
    CCreatureNavigationDef* p;
    CNavDefHolder(CCreatureNavigationDef* d) : p(d) { }
    ~CNavDefHolder() { if (p) p->Release(); }
};
static CCreatureNavigationDef g_def;
struct CDefClassBase { bool GetSubDef(CDefPointerNav* out) { out->m_p = &g_def; ++g_def.RefCount; return true; } };
struct CTCAIScratchpad {
    C3DVector WanderCentrePoint;
    float WanderMaxDistance;
    long ScriptingStateGroup;
    C3DVector GetWanderCentrePoint() const { return WanderCentrePoint; }
};
struct CThing {
    C3DVector pos;
    CTCAIScratchpad* scratchpad;
    CDefClassBase* PhysicsTC;
    const C3DVector& GetPosition() const { return pos; }
    void GetInterface(CTCAIScratchpad** out, long) { *out = scratchpad; }
};
struct CAIBrain { CThing* PCreature; };

static int g_wander[8], g_idle[8], g_nothing[8], g_move[8];
static int g_iw, g_ii, g_in, g_im, g_terminated;
static C3DVector g_moveTo; static float g_moveRadius; static bool g_moveAvoid;

struct CAIStateGroup_MinionWander {
    CAIBrain* PBrain;
    void Terminate() { ++g_terminated; }
    long AIAction_MinionWander(long, void*) { return g_wander[g_iw++]; }
    long AIAction_MinionIdle() { return g_idle[g_ii++]; }
    long AIAction_DoNothing() { return g_nothing[g_in++]; }
    long AIAction_MoveToPosProximity(long, C3DVector pos, float radius, bool* reached, long, bool, bool, bool c, CThing*, bool, float)
    {
        g_moveTo = pos; g_moveRadius = radius; g_moveAvoid = c; *reached = true; return g_move[g_im++];
    }
    void Update();
};

void CAIStateGroup_MinionWander::Update()
{
    CThing* creature = PBrain->PCreature;
    CTCAIScratchpad* scratchpad;
    creature->GetInterface(&scratchpad, 0xaa);
    CDefPointerNav navDef;
    creature->PhysicsTC->GetSubDef(&navDef);
    C3DVector start = PBrain->PCreature->GetPosition();
    CNavDefHolder def(navDef.m_p);
    bool reached;
    do {
        if (AIAction_MinionWander(3, 0) != 2)
            return;
        if (AIAction_MinionIdle() != 2)
            return;
        C3DVector centre = scratchpad->ScriptingStateGroup == 4 ? scratchpad->GetWanderCentrePoint() : start;
        float radius = scratchpad->ScriptingStateGroup == 4 ? scratchpad->WanderMaxDistance : 10.0f;
        float distSq;
        {
            const C3DVector& pos = PBrain->PCreature->GetPosition();
            float dx = pos.X - centre.X;
            float dy = pos.Y - centre.Y;
            distSq = dy * dy + dx * dx;
        }
        float radiusSq = radius * radius;
        if (!(distSq < radiusSq)) {
            reached = false;
            if (AIAction_MoveToPosProximity(1, centre, radius, &reached, 3, true, false, def.p->AvoidDynamicObstacles, 0, true, 0.0f) != 2) {
                Terminate();
                return;
            }
            if (AIAction_MinionIdle() != 2) {
                Terminate();
                return;
            }
        }
    } while (AIAction_DoNothing() == 4);
}

static void Reset() { g_iw = g_ii = g_in = g_im = g_terminated = 0; g_released = 0; g_def.RefCount = 0; }

int main()
{
    int ok = 1;
    g_def.AvoidDynamicObstacles = true;
    CTCAIScratchpad pad; pad.WanderCentrePoint.X = 100; pad.WanderCentrePoint.Y = 0; pad.WanderCentrePoint.Z = 0;
    pad.WanderMaxDistance = 5.0f; pad.ScriptingStateGroup = 0;
    CDefClassBase dcb;
    CThing creature; creature.pos.X = 0; creature.pos.Y = 0; creature.pos.Z = 0; creature.scratchpad = &pad; creature.PhysicsTC = &dcb;
    CAIBrain brain = { &creature };
    CAIStateGroup_MinionWander s; s.PBrain = &brain;

    // 1: close to start: no move; loop twice then stop
    Reset(); g_wander[0] = 2; g_wander[1] = 2; g_idle[0] = 2; g_idle[1] = 2; g_nothing[0] = 4; g_nothing[1] = 0;
    s.Update();
    if (g_iw != 2 || g_im != 0 || g_released != 1 || g_terminated) ok = 0;
    // 2: wander fails at once
    Reset(); g_wander[0] = 3; s.Update();
    if (g_iw != 1 || g_ii != 0 || g_released != 1) ok = 0;
    // 3: state 4, far from the scratchpad centre: move there with the max distance, then idle fails -> Terminate
    Reset(); pad.ScriptingStateGroup = 4; g_wander[0] = 2; g_idle[0] = 2; g_move[0] = 2; g_idle[1] = 1;
    s.Update();
    if (g_im != 1 || g_moveTo.X != 100 || g_moveRadius != 5.0f || !g_moveAvoid || g_terminated != 1 || g_released != 1) ok = 0;
    // 4: state 4 but already inside the circle: no move
    Reset(); creature.pos.X = 98; g_wander[0] = 2; g_idle[0] = 2; g_nothing[0] = 0;
    s.Update();
    if (g_im != 0 || g_released != 1) ok = 0;
    if (!ok) { std::printf("0092c070_TEST FAIL\n"); return 1; }
    std::printf("0092c070_TEST PASS\n");
    return 0;
}