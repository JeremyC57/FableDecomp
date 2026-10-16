// behaviour test for 0x007ECE60 CTCCarriedActionUseBase::vfunc_32 (sheathe-to-inventory action, kind 100)
#include <cstdio>
struct Action;
struct Creature { Action* got; long kind; void* owner; bool SetCurrentAction(Action* a); };
struct Block { long refs; };
static int g_dtors = 0;
struct Action {
    Creature* owner; Action* next; long kind; Block* count;
    Action(Creature* o, Action* n, long k) : owner(o), next(n), kind(k), count(0) {}
    ~Action() { if (count) { --count->refs; } count = 0; ++g_dtors; }
};
bool Creature::SetCurrentAction(Action* a) { got = a; kind = a->kind; owner = a->owner; return true; }
struct UseBase {
    void vfunc_32(Creature* user)
    {
        Action a(user, 0, 100);
        user->SetCurrentAction(&a);
    }
};
int main()
{
    Creature c; c.got = 0; c.kind = 0; c.owner = 0;
    UseBase u; u.vfunc_32(&c);
    bool ok = c.got != 0 && c.kind == 100 && c.owner == &c && g_dtors == 1;
    if (!ok) { std::printf("007ece60_TEST FAIL\n"); return 1; }
    std::printf("007ece60_TEST PASS\n");
    return 0;
}