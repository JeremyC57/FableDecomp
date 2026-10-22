// Behaviour test for CAIStateGroup_MinionKidCalledToMother::Update (0x0090EEC0): model copy, scripted stubs.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char m_flags91;
    C3DVector pos;
    const C3DVector& GetPosition() const { return pos; }
};
struct CPtr {
    CThing* item;
    CThing* GetPItem() const { return item; }
};
struct CThingAICreature : CThing {
    CPtr PMother;
    float turn;
    float GetTurnAngleDegrees() { return turn; }
};
struct CIntelligentPointerConstThing { const CThing* item; CIntelligentPointerConstThing(CThing* t) : item(t) { } };

static CThing* g_event;
static CThing* g_hero;
static int g_eventLife;          // number of GetPItem calls on the held message before it vanishes (-1: never)
static int g_look[64], g_li, g_move[64], g_mi, g_nothing[64], g_ni, g_terminated;
static const CThing* g_lastTarget;
static float g_lastSpeed, g_lastDistance;
static long g_lastFrames, g_lastMovement;

struct CHeld {
    CThing* item;
    CHeld(CThing* t) : item(t) { }
    CThing* GetPItem() const
    {
        if (g_eventLife == 0) return 0;
        if (g_eventLife > 0) --g_eventLife;
        return item;
    }
};
struct CAIBrain { CThingAICreature* PCreature; };
struct CAIStateGroup_MinionKidCalledToMother {
    CAIBrain* PBrain;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CThing* FindCallHomeEvent() { return g_event; }
    CThing* GetHero() { return g_hero; }
    void Terminate() { ++g_terminated; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed)
    {
        g_lastTarget = thing.item; g_lastFrames = frames; g_lastSpeed = speed;
        return repeat == 1 ? g_look[g_li++] : -1;
    }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed)
    {
        g_lastTarget = target.item; g_lastDistance = distance; g_lastMovement = movement;
        if (repeat != 2 || *reached || !a || b || !c || !d || speed != 0.0f) return -1;
        return g_move[g_mi++];
    }
    long AIAction_DoNothing() { return g_nothing[g_ni++]; }
    void Update();
};

void CAIStateGroup_MinionKidCalledToMother::Update()
{
    CHeld callHome(FindCallHomeEvent());
    if (callHome.GetPItem() == 0)
        return;
    long result = 3;
    do {
        if (callHome.GetPItem() == 0) {
            Terminate();
            return;
        }
        CThing* hero = GetHero();
        if (hero != 0 && !(hero->m_flags91 & 1)) {
            CThing* mother = GetCreature()->PMother.GetPItem();
            if (mother != 0 && !(mother->m_flags91 & 1)) {
                CThingAICreature* creature = GetCreature();
                float distanceSquared;
                {
                    const C3DVector& motherPos = creature->PMother.GetPItem()->GetPosition();
                    const C3DVector& pos = creature->GetPosition();
                    float dx = pos.X - motherPos.X;
                    float dy = pos.Y - motherPos.Y;
                    distanceSquared = dy * dy + dx * dx;
                }
                if (distanceSquared < 1.44f) {
                    if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(hero), 9,
                            GetCreature()->GetTurnAngleDegrees() / 360.0f) != 2)
                        return;
                } else {
                    bool reached = false;
                    do {
                        if (callHome.GetPItem() == 0 || (mother = GetCreature()->PMother.GetPItem()) == 0
                                || (mother->m_flags91 & 1)) {
                            Terminate();
                            break;
                        }
                        CThing* target = GetCreature()->PMother.GetPItem();
                        result = AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(target), 1.2f, &reached, 5,
                            true, false, true, true, 0.0f);
                    } while (result == 4);
                    if (result != 2)
                        return;
                }
            }
        }
    } while (AIAction_DoNothing() == 4);
}

static void Reset()
{
    g_li = g_mi = g_ni = g_terminated = 0; g_eventLife = -1; g_lastTarget = 0;
    for (int i = 0; i < 64; ++i) { g_look[i] = 2; g_move[i] = 2; g_nothing[i] = 4; }
}

int main()
{
    int ok = 1;
    CThing event = { 0 }, hero = { 0 }, mother = { 0 };
    mother.pos.X = 0.0f; mother.pos.Y = 0.0f; mother.pos.Z = 0.0f;
    CThingAICreature kid; kid.m_flags91 = 0; kid.turn = 90.0f; kid.PMother.item = &mother;
    kid.pos.X = 1.0f; kid.pos.Y = 0.5f; kid.pos.Z = 9.0f;           // 1.25 < 1.44: close
    CAIBrain brain = { &kid };
    CAIStateGroup_MinionKidCalledToMother s; s.PBrain = &brain;
    g_hero = &hero;
    // no message: nothing happens
    Reset(); g_event = 0; s.Update(); if (g_terminated || g_li || g_ni) ok = 0;
    g_event = &event;
    // close to mother: look at the hero at a quarter turn per frame; stop when DoNothing is not 4
    Reset(); g_nothing[2] = 1; s.Update();
    if (g_terminated || g_li != 3 || g_ni != 3 || g_lastTarget != &hero || g_lastFrames != 9 || g_lastSpeed != 0.25f) ok = 0;
    // look fails: return at once
    Reset(); g_look[0] = 3; s.Update(); if (g_terminated || g_ni) ok = 0;
    // message vanishes after the first check: terminate
    Reset(); g_eventLife = 1; s.Update(); if (g_terminated != 1 || g_li) ok = 0;
    // dead hero: just idles
    Reset(); hero.m_flags91 = 1; g_nothing[1] = 0; s.Update(); if (g_li || g_mi || g_ni != 2 || g_terminated) ok = 0;
    hero.m_flags91 = 0;
    // far away: walk to the mother, retrying while 4
    kid.pos.X = 3.0f;
    Reset(); g_move[0] = 4; g_move[1] = 4; g_move[2] = 2; g_nothing[0] = 0; s.Update();
    if (g_mi != 3 || g_ni != 1 || g_lastTarget != &mother || g_lastDistance != 1.2f || g_lastMovement != 5 || g_terminated) ok = 0;
    // move fails: return
    Reset(); g_move[0] = 3; s.Update(); if (g_mi != 1 || g_ni || g_terminated) ok = 0;
    // mother dies during the walk: terminate, then result 4 != 2 returns
    Reset(); g_move[0] = 4; mother.m_flags91 = 0;
    {
        // dies before the second attempt: emulate by a message that vanishes on the inner re-check
        g_eventLife = 3; s.Update();
        if (g_terminated != 1 || g_mi != 1 || g_ni) ok = 0;
    }
    if (!ok) { std::printf("0090eec0_TEST FAIL\n"); return 1; }
    std::printf("0090eec0_TEST PASS\n");
    return 0;
}
