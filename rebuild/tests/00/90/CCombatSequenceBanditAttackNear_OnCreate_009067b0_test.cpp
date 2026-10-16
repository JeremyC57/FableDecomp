#include <cstdio>
// Behaviour test for CCombatSequence_BanditAttackNear::OnCreate @ 0x009067B0: two actions queued in order
// (interruptable near attack code 150, leap back code 50), the base OnCreate last, the counted local released.
static int g_alive = 0;
struct Action { long code; int owner; int target; Action(int o, int t, long c) : code(c), owner(o), target(t) { ++g_alive; } ~Action() { --g_alive; } };
struct Block { long refs; Action* obj; };
struct Ptr {
    Action* obj; Block* block;
    Ptr() : obj(0), block(0) { }
    void Reset(Action* a) { Release(); obj = a; block = a ? new Block : 0; if (block) { block->refs = 1; block->obj = a; } }
    Ptr& operator=(Action* a) { Reset(a); return *this; }
    void Release() { if (block) { if (--block->refs == 0) { delete block->obj; delete block; } } obj = 0; block = 0; }
    ~Ptr() { Release(); }
};
struct Seq {
    int owner, target; long codes[4]; int n; bool baseCreated; int liveAtBase;
    void AddAction(const Ptr& p) { codes[n++] = p.obj->code; if (p.obj->owner != owner || p.obj->target != target) codes[n - 1] = -1; }
    void BaseOnCreate() { baseCreated = true; liveAtBase = g_alive; }
    void OnCreate()
    {
        Ptr action;
        action = new Action(owner, target, 150);
        AddAction(action);
        action = new Action(owner, target, 50);
        AddAction(action);
        BaseOnCreate();
    }
};
int main()
{
    Seq s; s.owner = 3; s.target = 9; s.n = 0; s.baseCreated = false; s.liveAtBase = -1;
    s.OnCreate();
    if (s.n != 2 || s.codes[0] != 150 || s.codes[1] != 50 || !s.baseCreated || s.liveAtBase != 1 || g_alive != 0) {
        std::printf("009067b0_TEST FAIL\n"); return 1;
    }
    std::printf("009067b0_TEST PASS\n");
    return 0;
}