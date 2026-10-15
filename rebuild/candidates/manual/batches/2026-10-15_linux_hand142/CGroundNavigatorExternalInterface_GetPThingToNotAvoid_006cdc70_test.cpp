#include <cstdio>
// Host model of CGroundNavigatorExternalInterface::GetPThingToNotAvoid.
struct Thing;
struct Record { void* thing; };
struct Nav { Record* record; Thing* notAvoid; };
struct Thing { Nav* nav; };
static void* GetPThingToNotAvoid(Thing* creature)
{
    Nav* navigation = creature->nav;                 // retail requires the creature to have one
    Thing* other = navigation->notAvoid;
    Nav* otherNavigation = 0;
    if (other) otherNavigation = other->nav;
    if (!otherNavigation) return 0;
    return otherNavigation->record->thing;
}
int main()
{
    int fail = 0;
    int target;
    Record rec = { &target };
    Nav otherNav = { &rec, 0 };
    Thing other = { &otherNav };
    Thing bare = { 0 };
    Nav nav = { 0, 0 };
    Thing creature = { &nav };
    if (GetPThingToNotAvoid(&creature) != 0) fail = 1;
    nav.notAvoid = &bare;
    if (GetPThingToNotAvoid(&creature) != 0) fail = 1;
    nav.notAvoid = &other;
    if (GetPThingToNotAvoid(&creature) != &target) fail = 1;
    if (fail) { std::printf("006cdc70_TEST FAIL\n"); return 1; }
    std::printf("006cdc70_TEST PASS\n");
    return 0;
}
