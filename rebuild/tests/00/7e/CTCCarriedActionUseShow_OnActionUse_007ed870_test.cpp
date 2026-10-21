// Behaviour test for CTCCarriedActionUseShow::OnActionUse (0x007ED870): show to a target and apply the bonus def.
#include <cstdio>
struct Item { bool persisted; bool hasBonusDef; int defRefs; };
struct User { bool hasTargeting, targetOk, hasStats; int action; int bonusApplied; };
static void Use(User* u, Item* item, int* failed)
{
    item->persisted = false;
    if (u->hasTargeting && u->targetOk) {
        u->action = 1;
        if (item->hasBonusDef) {
            ++item->defRefs;
            if (u->hasStats) ++u->bonusApplied;
            --item->defRefs;
        }
        return;
    }
    ++*failed;
}
int main()
{
    Item it = { true, true, 1 }; int failed = 0;
    User a = { true, true, true, 0, 0 }; Use(&a, &it, &failed);
    User b = { true, false, true, 0, 0 }; Use(&b, &it, &failed);
    User c = { true, true, false, 0, 0 }; Use(&c, &it, &failed);
    if (it.persisted || a.action != 1 || a.bonusApplied != 1 || b.action != 0 || failed != 1 || c.bonusApplied != 0 || it.defRefs != 1)
    { std::printf("007ed870_TEST FAIL\n"); return 1; }
    std::printf("007ed870_TEST PASS\n");
    return 0;
}