// behaviour test for 0x007EDCD0 CTCCarriedActionUseFish::vfunc_32
#include <cstdio>
struct Creature { unsigned char flags91; long lastKind; void* lastArg; bool SetCurrentAction(long kind, void* arg) { lastKind = kind; lastArg = arg; return true; } };
struct Fish {
    Creature* ParentThing; bool targeted;
    bool IsWallMountTargeted(Creature*) { return targeted; }
    void vfunc_32(Creature* user)
    {
        if (IsWallMountTargeted(user)) {
            if (user && !(user->flags91 & 1)) { Creature* holder = ParentThing; user->SetCurrentAction(1, holder); }
        } else { Creature* holder = ParentThing; holder->SetCurrentAction(100, 0); }
    }
};
int main()
{
    Creature holder = { 0, 0, 0 }, user = { 0, 0, 0 };
    Fish f; f.ParentThing = &holder; f.targeted = false;
    f.vfunc_32(&user);
    bool ok = holder.lastKind == 100 && user.lastKind == 0;
    f.targeted = true; user.flags91 = 1;
    f.vfunc_32(&user);
    ok = ok && user.lastKind == 0;
    user.flags91 = 0;
    f.vfunc_32(&user);
    ok = ok && user.lastKind == 1 && user.lastArg == &holder;
    f.vfunc_32(0);
    if (!ok) { std::printf("007edcd0_TEST FAIL\n"); return 1; }
    std::printf("007edcd0_TEST PASS\n");
    return 0;
}
