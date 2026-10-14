#include <cstdio>
// Model of CAIStateGroup_WanderAroundShops::InitStateGroup @ 0x00946E10: reset the timer, register idle type 10.
struct Group;
struct CTCIdleScheduler { Group* registered; long type; void RegisterIdleStateGroup(Group* g, long t) { registered = g; type = t; } };
struct Creature {
    unsigned long bits[9]; CTCIdleScheduler* sched;
    bool GetInterface(long type, CTCIdleScheduler** out) { if (bits[type / 32] & (1 << (type % 32))) { *out = sched; return true; } return false; }
};
struct Group {
    long NextFrameBuy;
    void InitStateGroup(int&, Creature& c) { NextFrameBuy = 0; CTCIdleScheduler* s; if (c.GetInterface(0xd3, &s)) s->RegisterIdleStateGroup(this, 10); }
};
int main()
{
    CTCIdleScheduler sched = { 0, 0 }; Creature c = {}; c.sched = &sched; int brain = 0;
    Group g; g.NextFrameBuy = 77; g.InitStateGroup(brain, c);
    if (g.NextFrameBuy != 0 || sched.registered != 0) { std::printf("00946e10_TEST FAIL\n"); return 1; }
    c.bits[6] |= 0x80000; g.NextFrameBuy = 77; g.InitStateGroup(brain, c);
    if (g.NextFrameBuy != 0 || sched.registered != &g || sched.type != 10) { std::printf("00946e10_TEST FAIL\n"); return 1; }
    std::printf("00946e10_TEST PASS\n"); return 0;
}