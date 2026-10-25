// 00547380 behaviour test: CSwappingStateComponent::Update -- after a state change remember the swap time when the
// current state is a swap state; when settled (target == current) in a swap state whose dwell time has run out, change
// to the next swap state (wrapping), or a random different one when random swapping is on, and restart the clock.
#include <cstdio>

struct Entry { unsigned long first; float second; };

static unsigned long g_baseUpdates = 0;
static unsigned long g_randomCalls = 0;

static unsigned long GFRandomNoRepeat(unsigned long range, unsigned long previous, unsigned long* seed)
{
    ++g_randomCalls;
    *seed = *seed * 0x24a1 + 0x24df;
    unsigned long r = range ? *seed % range : 0;
    if (range > 1 && r == previous) r = (r + 1) % range;
    return r;
}

struct Component {
    float Time;
    unsigned long CurrentState, TargetState;
    Entry* start; Entry* finish;
    float LastSwapTime;
    unsigned long RandomSeed;
    bool changedLast, randomSwap;
    unsigned long changedTo; int changes;
    unsigned long size() const { return (unsigned long)(finish - start); }
    void BaseUpdate(float) { ++g_baseUpdates; }
    void ChangeState(unsigned long s) { changedTo = s; ++changes; }
    void Update(float dt)
    {
        BaseUpdate(dt);
        if (changedLast) {
            for (unsigned long i = 0; i < size(); ++i)
                if (start[i].first == CurrentState) { LastSwapTime = Time; return; }
        } else if (TargetState == CurrentState) {
            Entry* it = start;
            while (it != finish && it->first != TargetState) ++it;
            if (it != finish && Time - LastSwapTime >= it->second) {
                unsigned long next;
                if (randomSwap && start != finish)
                    next = GFRandomNoRepeat(size(), TargetState, &RandomSeed);
                else {
                    unsigned long index = (unsigned long)(it - start);
                    next = index < size() - 1 ? index + 1 : 0;
                }
                ChangeState(start[next].first);
                LastSwapTime = Time;
            }
        }
    }
};

static int Fail(const char* why) { std::printf("00547380_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    Entry states[3] = { { 5, 1.0f }, { 7, 2.0f }, { 9, 0.5f } };
    Component c;
    c.start = states; c.finish = states + 3;
    c.Time = 10.0f; c.LastSwapTime = 0.0f; c.RandomSeed = 1; c.changes = 0; c.changedTo = 0;
    // after a state change into a swap state the clock restarts
    c.changedLast = true; c.randomSwap = false; c.CurrentState = 7; c.TargetState = 7;
    c.Update(0.1f);
    if (c.LastSwapTime != 10.0f || c.changes != 0) return Fail("changed: clock");
    // not a swap state: nothing
    c.CurrentState = 4; c.LastSwapTime = 3.0f; c.Update(0.1f);
    if (c.LastSwapTime != 3.0f) return Fail("changed: other state");
    // settled in state 7 (dwell 2): 1.5 elapsed -> wait
    c.changedLast = false; c.CurrentState = 7; c.TargetState = 7; c.Time = 11.5f; c.LastSwapTime = 10.0f;
    c.Update(0.1f);
    if (c.changes != 0) return Fail("dwell not over");
    // 2.0 elapsed -> next state 9
    c.Time = 12.0f; c.Update(0.1f);
    if (c.changes != 1 || c.changedTo != 9 || c.LastSwapTime != 12.0f) return Fail("next state");
    // the last state wraps to the first
    c.CurrentState = 9; c.TargetState = 9; c.Time = 13.0f; c.Update(0.1f);
    if (c.changes != 2 || c.changedTo != 5) return Fail("wrap");
    // still changing (target != current): nothing
    c.TargetState = 5; c.Time = 100.0f; c.Update(0.1f);
    if (c.changes != 2) return Fail("not settled");
    // not a swap state while settled: nothing
    c.CurrentState = 4; c.TargetState = 4; c.Update(0.1f);
    if (c.changes != 2) return Fail("unknown state");
    // random swapping picks through GFRandomNoRepeat, never the same state index
    c.randomSwap = true; c.CurrentState = 5; c.TargetState = 5; c.LastSwapTime = 0.0f; c.Time = 50.0f;
    c.Update(0.1f);
    if (c.changes != 3 || g_randomCalls != 1 || c.LastSwapTime != 50.0f) return Fail("random");
    if (g_baseUpdates != 8) return Fail("base update");
    std::printf("00547380_TEST PASS\n");
    return 0;
}