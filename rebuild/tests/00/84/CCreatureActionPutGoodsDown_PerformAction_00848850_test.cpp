// behaviour test for 0x00848850 CCreatureAction_PutGoodsDown::PerformAction(): the goods in the primary carry slot are
// removed (true) and repositioned to their own position raised by the global Z offset; mode 6 is always removed
#include <cstdio>
struct Vec { float X, Y, Z; };
static float g_PutGoodsDownZOffset = 0.5f;
struct Physics { Vec Position; int sets; void SetPosition(const Vec& v) { Position = v; ++sets; } };
struct Thing { Physics* PhysicsTC; };
struct Carrying {
    Thing* primary; int removed; bool removeArg;
    Thing* GetThingInPrimarySlot() const { return primary; }
    Thing* RemoveThingFromPrimarySlot(bool b) { ++removed; removeArg = b; Thing* t = primary; primary = 0; return t; }
};
struct Modes { unsigned long lastRemoved; int count; void RemoveMode(unsigned long m) { lastRemoved = m; ++count; } };
struct Creature { Carrying* carrying; Modes* modes; };
static void PerformAction(Creature& c)
{
    Carrying* carrying = c.carrying;
    Thing* goods = carrying->GetThingInPrimarySlot();
    if (goods) {
        carrying->RemoveThingFromPrimarySlot(true);
        Physics* physics = goods->PhysicsTC;
        const Vec& position = physics->Position;
        Vec newPosition = { position.X, position.Y, position.Z + g_PutGoodsDownZOffset };
        physics->SetPosition(newPosition);
    }
    c.modes->RemoveMode(6);
}
int main()
{
    int fails = 0;
    Physics ph = { { 1.0f, 2.0f, 3.0f }, 0 };
    Thing goods = { &ph };
    Carrying car = { &goods, 0, false };
    Modes m = { 0, 0 };
    Creature c = { &car, &m };
    PerformAction(c);
    if (car.removed != 1 || !car.removeArg || car.primary) fails |= 1;
    if (ph.sets != 1 || ph.Position.X != 1.0f || ph.Position.Y != 2.0f || ph.Position.Z != 3.5f) fails |= 2;
    if (m.count != 1 || m.lastRemoved != 6) fails |= 4;
    PerformAction(c);
    if (car.removed != 1 || ph.sets != 1 || m.count != 2) fails |= 8;
    if (fails) { std::printf("00848850_TEST FAIL %d\n", fails); return 1; }
    std::printf("00848850_TEST PASS\n");
    return 0;
}