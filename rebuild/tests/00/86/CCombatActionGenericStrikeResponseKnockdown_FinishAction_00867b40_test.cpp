// Behaviour test for CCombatAction_GenericStrikeResponseKnockdown::FinishAction (0x00867B40): the flip decision and the choice of
// the creature's next action, through a copy of the row's logic over a small recording model.
#include <cstdio>

enum ENextAction { NEXT_NONE, NEXT_DIE, NEXT_KILL, NEXT_BE_STUNNED, NEXT_RECOVER };
static const float DELTA_NOTIONAL_ZERO = 0.0001f;

struct SModel {
    // inputs
    bool  FlipFacing;
    long  AnimationIndex;
    bool  StatsHaveRotation;
    bool  Mode29Active;
    float Health;
    bool  LeaveDeadCreature;
    bool  KnockdownToStun;
    // recorded
    int   BaseFinishCalls, StatsQueries, StatsReleases, ModeQueries, ModeRemovals, Order;
    int   BaseFinishOrder, ModeRemovalOrder, NextOrder;
    ENextAction Next;
    bool  NextFlip;
    bool  KillArgument;
};

static void FinishAction(SModel& m)
{
    m.BaseFinishCalls++; m.BaseFinishOrder = ++m.Order;          // CCombatActionBase::FinishAction()
    long animation_index = m.AnimationIndex;                     // GetAnimationIndex()
    bool flip_facing = m.FlipFacing;
    if (flip_facing) {
        if (animation_index) {
            m.StatsQueries++;                                    // PeekAnimation(index)->GetAnimationStats()
            if (m.StatsHaveRotation)
                flip_facing = false;
            m.StatsReleases++;                                   // ~CCountedPointer
        }
        if (flip_facing && (m.ModeQueries++, m.Mode29Active))    // IsModeActive(29)
            flip_facing = false;
    }
    m.ModeRemovals++; m.ModeRemovalOrder = ++m.Order;            // RemoveMode(29)
    m.NextOrder = ++m.Order;
    if (m.Health < DELTA_NOTIONAL_ZERO) {
        if (m.LeaveDeadCreature) { m.Next = NEXT_DIE; m.NextFlip = flip_facing; }
        else { m.Next = NEXT_KILL; m.KillArgument = true; }
    } else if (m.KnockdownToStun) { m.Next = NEXT_BE_STUNNED; m.NextFlip = flip_facing; }
    else { m.Next = NEXT_RECOVER; m.NextFlip = flip_facing; }
}

struct SCase {
    bool flip; long index; bool rotation; bool mode; float health; bool leave_dead; bool stun;
    ENextAction next; bool next_flip; int stats_queries; int mode_queries;
};

static const SCase g_cases[] = {
    // flip   idx  rot    mode   health   leave  stun    next             flip   stats modeq
    { false, 12,  true,  true,  50.0f,   false, false,  NEXT_RECOVER,    false, 0,    0 },   // no flip: nothing is asked
    { true,  12,  false, false, 50.0f,   false, false,  NEXT_RECOVER,    true,  1,    1 },   // flip survives both vetoes
    { true,  12,  true,  false, 50.0f,   false, false,  NEXT_RECOVER,    false, 1,    0 },   // rotation in the animation clears it, mode not asked
    { true,  12,  false, true,  50.0f,   false, false,  NEXT_RECOVER,    false, 1,    1 },   // mode 29 active clears it
    { true,  0,   true,  false, 50.0f,   false, false,  NEXT_RECOVER,    true,  0,    1 },   // no animation: the stats are never fetched
    { true,  0,   true,  true,  50.0f,   false, true,   NEXT_BE_STUNNED, false, 0,    1 },   // stun follow-up, flip cleared by the mode
    { true,  7,   false, false, 50.0f,   true,  true,   NEXT_BE_STUNNED, true,  1,    1 },   // alive: LeaveDeadCreature is irrelevant
    { true,  7,   false, false, 0.0f,    true,  true,   NEXT_DIE,        true,  1,    1 },   // dead and the def leaves the body: Die(flip)
    { true,  7,   true,  false, 0.0f,    true,  false,  NEXT_DIE,        false, 1,    0 },
    { true,  7,   false, false, 0.0f,    false, true,   NEXT_KILL,       false, 1,    1 },   // dead, no body left: Kill(true)
    { false, 0,   false, false, 0.00005f,false, false,  NEXT_KILL,       false, 0,    0 },   // below the notional zero counts as dead
    { false, 0,   false, false, 0.0001f, true,  false,  NEXT_RECOVER,    false, 0,    0 },   // exactly the threshold is alive (strict <)
    { true,  3,   false, false, -5.0f,   true,  false,  NEXT_DIE,        true,  1,    1 },
};

int main()
{
    bool ok = true;
    for (unsigned i = 0; i < sizeof(g_cases) / sizeof(g_cases[0]); ++i) {
        const SCase& c = g_cases[i];
        SModel m = { c.flip, c.index, c.rotation, c.mode, c.health, c.leave_dead, c.stun };
        FinishAction(m);
        bool good = m.BaseFinishCalls == 1 && m.BaseFinishOrder == 1          // the base runs first, once
                 && m.ModeRemovals == 1 && m.ModeRemovalOrder == 2           // mode 29 is always dropped, before the next action
                 && m.NextOrder == 3 && m.Next == c.next
                 && m.StatsQueries == c.stats_queries && m.StatsReleases == c.stats_queries
                 && m.ModeQueries == c.mode_queries;
        if (c.next == NEXT_KILL) good = good && m.KillArgument;
        else good = good && m.NextFlip == c.next_flip;
        if (!good) { std::printf("case %u wrong\n", i); ok = false; }
    }
    if (!ok) { std::printf("00867b40_TEST FAIL\n"); return 1; }
    std::printf("00867b40_TEST PASS\n");
    return 0;
}