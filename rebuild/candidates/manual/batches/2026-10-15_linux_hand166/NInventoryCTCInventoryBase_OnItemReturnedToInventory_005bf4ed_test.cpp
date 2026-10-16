// Behaviour test for 0x005BF4ED NInventory::CTCInventoryBase::OnItemReturnedToInventory
#include <cstdio>
struct CTCTrophy { bool MinigameInProgress; int ended; void EndMinigame() { ++ended; MinigameInProgress = false; } };
struct CThing {
    CTCTrophy* trophy; bool limbo; bool killed;
    bool GetInterface(CTCTrophy** out, long) { if (!trophy) return false; *out = trophy; return true; }
    void SetInLimbo(bool b) { limbo = b; }
    void Kill(bool) { killed = true; }
};
static int g_updates = 0;
static void OnItemReturnedToInventory(CThing& item)
{
    ++g_updates;
    CTCTrophy* trophy;
    if (item.GetInterface(&trophy, 0x39) && trophy->MinigameInProgress) {
        item.SetInLimbo(true);
        trophy->EndMinigame();
    } else
        item.Kill(false);
}
int main()
{
    CTCTrophy tr = { true, 0 };
    CThing a = { &tr, false, false }, b = { 0, false, false };
    OnItemReturnedToInventory(a); OnItemReturnedToInventory(b);
    bool ok = a.limbo && !a.killed && tr.ended == 1 && b.killed && g_updates == 2;
    OnItemReturnedToInventory(a); ok = ok && a.killed;
    if (!ok) { std::printf("005bf4ed_TEST FAIL\n"); return 1; }
    std::printf("005bf4ed_TEST PASS\n");
    return 0;
}
