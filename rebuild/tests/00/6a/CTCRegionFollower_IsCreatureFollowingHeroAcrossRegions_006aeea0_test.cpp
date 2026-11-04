// Behaviour test for CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions (0x006AEEA0): a model of the row -- a child follows only
// when its creature def allows it; a wife married to the player but not permitted does not follow; a summoned creature and the
// GHOSTLY_SWORD always follow; everything else asks the parent's TCI_FOLLOWED component.
#include <stdio.h>
struct Creature { bool child; bool defAllowsChild; bool hasDef; bool isWife; bool married; bool permitted;
                  bool summonable; bool summoned; const char* defName; bool validFollower; };
static bool SameNoCase(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        char x = (*a >= 'a' && *a <= 'z') ? (char)(*a - 32) : *a;
        char y = (*b >= 'a' && *b <= 'z') ? (char)(*b - 32) : *b;
        if (x != y) return false;
        if (x == 0) return true;
    }
}
static bool IsFollowing(const Creature& c)
{
    if (c.child) return c.hasDef && c.defAllowsChild;
    if (c.isWife && c.married && !c.permitted) return false;
    if (c.summonable && c.summoned) return true;
    if (SameNoCase(c.defName, "GHOSTLY_SWORD")) return true;
    return c.validFollower;
}
int main()
{
    Creature child = { true, true, true, false, false, false, false, false, "KID", false };
    Creature childNo = { true, false, true, false, false, false, false, false, "KID", true };
    Creature wife = { false, false, false, true, true, false, false, false, "WIFE", true };
    Creature wifeOk = { false, false, false, true, true, true, false, false, "WIFE", true };
    Creature summoned = { false, false, false, false, false, false, true, true, "X", false };
    Creature sword = { false, false, false, false, false, false, false, false, "ghostly_sword", false };
    Creature other = { false, false, false, false, false, false, false, false, "X", false };
    bool ok = IsFollowing(child) && !IsFollowing(childNo) && !IsFollowing(wife) && IsFollowing(wifeOk)
        && IsFollowing(summoned) && IsFollowing(sword) && !IsFollowing(other);
    printf(ok ? "006aeea0_TEST PASS\n" : "006aeea0_TEST FAIL\n");
    return ok ? 0 : 1;
}