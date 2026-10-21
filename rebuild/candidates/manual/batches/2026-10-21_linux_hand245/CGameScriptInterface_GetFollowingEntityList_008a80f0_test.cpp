// Behaviour test for 008a80f0 CGameScriptInterface::GetFollowingEntityList: append followers not yet in the list; true when one was added.
#include <cstdio>
struct Thing { int id; bool dead; int nFollowers; Thing* followers[4]; bool hasFollowTC; };
static bool GetFollowing(Thing* leader, int* list, int& n)
{
    bool added = false;
    if (leader && !leader->dead && leader->hasFollowTC) {
        for (int i = 0; i < leader->nFollowers; ++i) {
            int id = leader->followers[i]->id;
            bool found = false;
            for (int j = 0; j < n; ++j) if (list[j] == id) found = true;
            if (!found) { list[n++] = id; added = true; }
        }
    }
    return added;
}
int main()
{
    Thing a = { 1, false, 0, { 0 }, false }, b = { 2, false, 0, { 0 }, false };
    Thing lead = { 9, false, 2, { &a, &b }, true };
    int list[8] = { 2 }; int n = 1;
    bool r1 = GetFollowing(&lead, list, n);
    bool r2 = GetFollowing(&lead, list, n);
    lead.dead = true; bool r3 = GetFollowing(&lead, list, n);
    if (!r1 || r2 || r3 || n != 2 || list[1] != 1) { std::printf("008a80f0_TEST FAIL\n"); return 1; }
    std::printf("008a80f0_TEST PASS\n");
    return 0;
}
