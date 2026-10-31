// Behaviour test for CAIStateGroup_ServeFood::IsRunnable (0x00923070): the decision logic over a model tavern.
#include <cstdio>
struct Table {
    bool alive, limbo, hasTable, hasGame, beingServed, servingOk, eatingOk;
    int occupant;           // creature id seated, 0 none
    int freed;
};
struct Model {
    int sex, type;
    bool hasBuilding, buildingDead, hasOwner, isTime;
    Table tables[3]; int count;
    int creature;
    int mode; int table; long seat, actionPoint;
    bool IsRunnable()
    {
        if (sex != 2) return false;
        if (type != 2) return false;
        if (!hasBuilding || buildingDead || !hasOwner) return false;
        int i;
        if (isTime) {
            for (i = 0; i < count; ++i) {
                Table& t = tables[i];
                if (t.alive && !t.limbo && t.hasTable && !t.beingServed && !t.hasGame) {
                    mode = 1;
                    if (!t.beingServed && t.servingOk) { table = i; return true; }
                }
            }
            for (i = 0; i < count; ++i) {
                Table& t = tables[i];
                if (t.alive && !t.limbo && t.hasTable && !t.hasGame) {
                    if (t.occupant == creature) return false;
                    if (t.eatingOk) { t.freed = 1; seat = -1; actionPoint = -1; return false; }
                }
            }
            for (i = 0; i < count; ++i) {
                Table& t = tables[i];
                if (t.alive && !t.limbo && t.hasTable && !t.beingServed && !t.hasGame) {
                    mode = 1;
                    if (!t.beingServed && t.servingOk) { table = i; return true; }
                }
            }
            return false;
        }
        for (i = 0; i < count; ++i) {
            Table& t = tables[i];
            if (t.alive && !t.limbo && t.hasTable && !t.hasGame) {
                mode = 2;
                if (!t.beingServed && t.servingOk) { table = i; return true; }
            }
        }
        return false;
    }
};
static void Reset(Model& m)
{
    m.sex = 2; m.type = 2; m.hasBuilding = true; m.buildingDead = false; m.hasOwner = true; m.isTime = true;
    m.count = 2; m.creature = 7; m.mode = 0; m.table = -1; m.seat = 3; m.actionPoint = 4;
    for (int i = 0; i < 3; ++i) {
        Table& t = m.tables[i];
        t.alive = true; t.limbo = false; t.hasTable = true; t.hasGame = false; t.beingServed = false;
        t.servingOk = false; t.eatingOk = false; t.occupant = 0; t.freed = 0;
    }
}
int main()
{
    Model m;
    Reset(m); m.sex = 1;
    if (m.IsRunnable()) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.buildingDead = true;
    if (m.IsRunnable()) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.tables[1].servingOk = true;
    if (!m.IsRunnable() || m.table != 1 || m.mode != 1) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.tables[0].occupant = 7;
    if (m.IsRunnable() || m.mode != 1) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.tables[1].eatingOk = true;
    if (m.IsRunnable() || m.tables[1].freed != 1 || m.seat != -1 || m.actionPoint != -1) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.isTime = false; m.tables[0].beingServed = true; m.tables[0].servingOk = true; m.tables[1].servingOk = true;
    if (!m.IsRunnable() || m.table != 1 || m.mode != 2) { std::printf("00923070_TEST FAIL\n"); return 1; }
    Reset(m); m.isTime = false; m.tables[0].hasGame = true; m.tables[1].limbo = true;
    if (m.IsRunnable() || m.mode != 0) { std::printf("00923070_TEST FAIL\n"); return 1; }
    std::printf("00923070_TEST PASS\n");
    return 0;
}