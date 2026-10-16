// behaviour test for 0x007EDDF0 CTCCarriedActionUseWallMount::vfunc_28
#include <cstdio>
struct Comp { int notified; void Notify() { ++notified; } };
struct Entry { long type; Comp* component; };
struct Creature {
    unsigned int bits[9]; Entry* first; Entry* last; unsigned char flags91; void* lastArg; int actions;
    unsigned int HasInterface(long t) const { return bits[t >> 5] & (1u << (t & 31)); }
    Comp* GetInterface(long type) {
        Entry* it = first; while (it != last && it->type < type) ++it;
        if (it == last || type < it->type) it = last;
        return it->component;
    }
    bool SetCurrentAction(void* arg) { lastArg = arg; ++actions; return true; }
};
struct WallMount {
    Creature* ParentThing;
    void vfunc_28(Creature* user)
    {
        if (user && !(user->flags91 & 1)) { Creature* holder = ParentThing; user->SetCurrentAction(holder); }
        else if (ParentThing->HasInterface(0x1a)) ParentThing->GetInterface(0x1a)->Notify();
    }
};
static Creature mk() { Creature c; for (int i = 0; i < 9; ++i) c.bits[i] = 0; c.first = c.last = 0; c.flags91 = 0; c.lastArg = 0; c.actions = 0; return c; }
int main()
{
    Creature holder = mk(), user = mk();
    Comp comp; comp.notified = 0;
    Entry e[2]; e[0].type = 0x1a; e[0].component = &comp; e[1].component = 0; holder.first = e; holder.last = e + 1;
    WallMount w; w.ParentThing = &holder;
    w.vfunc_28(&user);
    bool ok = user.actions == 1 && user.lastArg == &holder && comp.notified == 0;
    user.flags91 = 1;
    w.vfunc_28(&user);
    ok = ok && user.actions == 1 && comp.notified == 0;
    holder.bits[0] = 0x4000000;
    w.vfunc_28(0);
    ok = ok && comp.notified == 1;
    if (!ok) { std::printf("007eddf0_TEST FAIL\n"); return 1; }
    std::printf("007eddf0_TEST PASS\n");
    return 0;
}
