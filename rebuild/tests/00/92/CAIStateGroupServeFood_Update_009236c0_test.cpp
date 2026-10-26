// Behaviour test for CAIStateGroup_ServeFood::Update (0x009236C0): model copy, scripted stubs.
#include <cstdio>

class CThing { public: bool dead; bool killed; int carrySlot; };
class CTCTavernTable { public:
    bool Occupied;
    CThing* food;
    int notified;
    CThing* notifiedFood;
    CThing* GetFood(long) { return food; }
    void NotifyFoodServed(long, CThing* f) { ++notified; notifiedFood = f; }
};
class CTCCarrying { public:
    CThing* carried;
    int removed;
    void RemoveThingFromPrimarySlot(long) { ++removed; carried = 0; }
    void PutThingInCarrySlot(CThing* t, long slot, bool) { carried = t; t->carrySlot = (int)slot; }
};
static bool IsAlive(const CThing* t) { return t && !t->dead; }
static int g_walkOk, g_actionResult, g_actions, g_replaced, g_terminated;
static CThing g_madeFood;
struct CAIStateGroup_ServeFood {
    CTCTavernTable* table;
    CTCCarrying* carrying;
    long Mode;
    bool WalkTo() { return g_walkOk != 0; }
    CThing* MakeFood() { g_madeFood.dead = false; g_madeFood.killed = false; return &g_madeFood; }
    long PerformActionAndWait() { ++g_actions; return g_actionResult; }
    void OnReplace() { ++g_replaced; }
    void Terminate() { ++g_terminated; }
    void Update();
};

void CAIStateGroup_ServeFood::Update()
{
    table->Occupied = true;
    switch (Mode) {
    case 1:
        if (WalkTo()) {
            CThing* food = MakeFood();
            carrying->PutThingInCarrySlot(food, 3, true);
            if (PerformActionAndWait() != 2)
                break;
            table->NotifyFoodServed(0, food);
        }
        break;
    case 2:
        if (WalkTo()) {
            CThing* food = table->GetFood(0);
            if (food && !food->dead) {
                if (PerformActionAndWait() != 2)
                    break;
                carrying->RemoveThingFromPrimarySlot(0);
                if (IsAlive(food))
                    food->killed = true;
            }
            table->NotifyFoodServed(0, 0);
        }
        break;
    }
    OnReplace();
    Terminate();
}

int main()
{
    int ok = 1;
    CThing plate = { false, false, 0 };
    CTCTavernTable table = { false, &plate, 0, 0 };
    CTCCarrying carrying = { 0, 0 };
    CAIStateGroup_ServeFood s = { &table, &carrying, 0 };
    // mode 0: just occupy, replace and terminate
    s.Update();
    if (!table.Occupied || g_replaced != 1 || g_terminated != 1 || g_actions) ok = 0;
    // mode 1, walk fails
    s.Mode = 1;
    s.Update();
    if (g_actions || table.notified) ok = 0;
    // mode 1, served
    g_walkOk = 1; g_actionResult = 2;
    s.Update();
    if (carrying.carried != &g_madeFood || g_madeFood.carrySlot != 3 || table.notified != 1 || table.notifiedFood != &g_madeFood) ok = 0;
    // mode 1, action failed: no notification
    g_actionResult = 3;
    s.Update();
    if (table.notified != 1) ok = 0;
    // mode 2, live food, action ok: removed, killed, notified
    s.Mode = 2; g_actionResult = 2;
    s.Update();
    if (carrying.removed != 1 || !plate.killed || table.notified != 2 || table.notifiedFood != 0) ok = 0;
    // mode 2, action failed: nothing else
    plate.killed = false; g_actionResult = 1;
    s.Update();
    if (carrying.removed != 1 || plate.killed || table.notified != 2) ok = 0;
    // mode 2, dead food: notified without the action
    plate.dead = true; g_actions = 0;
    s.Update();
    if (g_actions || table.notified != 3 || plate.killed) ok = 0;
    if (g_replaced != 7 || g_terminated != 7) ok = 0;
    if (!ok) { std::printf("009236c0_TEST FAIL\n"); return 1; }
    std::printf("009236c0_TEST PASS\n");
    return 0;
}