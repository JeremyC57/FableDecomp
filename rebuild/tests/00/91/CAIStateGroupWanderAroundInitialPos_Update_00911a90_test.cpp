// Behaviour test for 0x00911A90 CAIStateGroup_WanderAroundInitialPos::Update(): a model of the row. The wander query runs on
// the creature's initial position with the scratchpad's WanderMaxDistance; a found position is walked to once; a live creature
// then has its targets cleared and its perception reset; the state group always terminates.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Log { int moves, clears, resets, terminates; Vec movedTo; float radiusSeen; Vec centreSeen; };
static Log g;
struct Navigation {
    bool succeed; Vec answer;
    bool GetNearbyWanderPosition(const Vec& centre, float radius, Vec& result, bool) const
    {
        g.centreSeen = centre; g.radiusSeen = radius;
        if (succeed) result = answer;
        return succeed;
    }
};
struct Scratchpad { float WanderMaxDistance; };
struct Creature { bool dead; Vec InitialPos; Navigation* nav; Scratchpad* pad; };
struct StateGroup {
    Creature* creature;
    void MoveToPos(const Vec& v, bool& reached) { ++g.moves; g.movedTo = v; reached = false; }
    void Update()
    {
        Vec target;
        bool found;
        {
            Creature* c = creature;
            found = c->nav->GetNearbyWanderPosition(c->InitialPos, c->pad->WanderMaxDistance, target, true);
        }
        if (found) {
            bool reached = false;
            MoveToPos(target, reached);
        }
        if (!creature->dead) { ++g.clears; ++g.resets; }
        ++g.terminates;
    }
};
int main()
{
    int ok = 1;
    Navigation nav = { true, { 1.0f, 2.0f, 3.0f } };
    Scratchpad pad = { 7.5f };
    Creature c = { false, { 10.0f, 20.0f, 0.0f }, &nav, &pad };
    StateGroup s = { &c };
    s.Update();
    ok &= g.moves == 1 && g.movedTo.X == 1.0f && g.movedTo.Z == 3.0f && g.clears == 1 && g.resets == 1 && g.terminates == 1;
    ok &= g.radiusSeen == 7.5f && g.centreSeen.X == 10.0f && g.centreSeen.Y == 20.0f;
    nav.succeed = false; c.dead = true;
    s.Update();
    ok &= g.moves == 1 && g.clears == 1 && g.terminates == 2;
    std::printf(ok ? "00911a90_TEST PASS\n" : "00911a90_TEST FAIL\n");
    return ok ? 0 : 1;
}