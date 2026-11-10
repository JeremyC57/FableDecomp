// Behaviour model of CGameScriptInterface::EntitySetAsSittingOnFloor (0x008A8C60).
#include <cstdio>
#include <cstring>
enum { MODE_STAND = 1, MODE_SIT_FLOOR = 10 };
struct Creature { bool alive; bool isCreature; int mode; int finished; char action[32]; char sitGroup[32]; };
static void SetAction(Creature& c, const char* name) { std::strcpy(c.action, name); }
static void EntitySetAsSittingOnFloor(Creature& c, bool sit, const char* animGroup)
{
    if (!c.alive || !c.isCreature) return;
    if (sit) {
        if (c.mode == MODE_SIT_FLOOR) return;
        ++c.finished;
        std::strcpy(c.sitGroup, animGroup);           // the new floor-sitting mode filters on the chosen animation's group
        SetAction(c, "SitDown");
    }
    else {
        if (c.mode != MODE_SIT_FLOOR) return;
        ++c.finished;
        SetAction(c, "ST_STAND_UP_FROM_FLOOR");
    }
}
static int fail(int n) { std::printf("008a8c60_TEST FAIL %d\n", n); return 1; }
int main()
{
    Creature c = { true, true, MODE_STAND, 0, "", "" };
    EntitySetAsSittingOnFloor(c, false, "G");
    if (c.finished != 0 || c.action[0]) return fail(1);
    EntitySetAsSittingOnFloor(c, true, "SIT_GROUP");
    if (c.finished != 1 || std::strcmp(c.action, "SitDown") || std::strcmp(c.sitGroup, "SIT_GROUP")) return fail(2);
    c.mode = MODE_SIT_FLOOR;
    EntitySetAsSittingOnFloor(c, true, "X");
    if (c.finished != 1) return fail(3);
    EntitySetAsSittingOnFloor(c, false, "X");
    if (c.finished != 2 || std::strcmp(c.action, "ST_STAND_UP_FROM_FLOOR")) return fail(4);
    Creature d = { true, false, MODE_STAND, 0, "", "" };
    EntitySetAsSittingOnFloor(d, true, "X");
    if (d.finished) return fail(5);
    Creature e = { false, true, MODE_STAND, 0, "", "" };
    EntitySetAsSittingOnFloor(e, true, "X");
    if (e.finished) return fail(6);
    std::printf("008a8c60_TEST PASS\n");
    return 0;
}