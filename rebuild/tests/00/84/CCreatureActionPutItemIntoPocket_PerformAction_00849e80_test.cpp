// behaviour test for 0x00849e80 CCreatureAction_PutItemIntoPocket::PerformAction(): with a positive item def index and
// a CTCCarrying, create the object at the creature's position for its player (empty script name) and, when it has a
// CTCCarryable, put it in that carryable's active carry slot (true); otherwise nothing is created / carried
#include <cstdio>
#include <cstring>
struct Vec { float X, Y, Z; };
struct Carryable { long slot; long GetCarrySlotActive() const { return slot; } };
struct Object { long def; Vec pos; long player; char script[8]; const Carryable* carryable;
    bool PeekTC(const Carryable** out) const { if (!carryable) return false; *out = carryable; return true; } };
static Object gObject; static int gCreated = 0; static const Carryable* gNextCarryable = 0;
static Object* Create(long def, const Vec& pos, long player, const char* script)
{
    ++gCreated; gObject.def = def; gObject.pos = pos; gObject.player = player; std::strcpy(gObject.script, script);
    gObject.carryable = gNextCarryable; return &gObject;
}
struct Carrying { Object* put; long slot; bool flag; int count;
    void PutThingInCarrySlot(Object& o, long s, bool f) { put = &o; slot = s; flag = f; ++count; } };
struct Creature { Carrying* carrying; Vec pos; long player;
    bool GetTC(Carrying** out) { if (!carrying) return false; *out = carrying; return true; } };
struct Action {
    Creature* PCreature; long ItemDefIndex;
    void PerformAction()
    {
        if (ItemDefIndex > 0) {
            Carrying* carrying;
            if (PCreature->GetTC(&carrying)) {
                Object* item = Create(ItemDefIndex, PCreature->pos, PCreature->player, "");
                const Carryable* carryable;
                if (item && item->PeekTC(&carryable)) {
                    long slot = carryable->GetCarrySlotActive();
                    carrying->PutThingInCarrySlot(*item, slot, true);
                }
            }
        }
    }
};
int main()
{
    int fails = 0;
    Carryable cy = { 3 };
    Carrying car = { 0, 0, false, 0 };
    Creature c = { &car, { 1.0f, 2.0f, 3.0f }, 2 };
    Action a = { &c, 42 };
    gNextCarryable = &cy;
    a.PerformAction();
    if (gCreated != 1 || gObject.def != 42 || gObject.pos.Z != 3.0f || gObject.player != 2 || gObject.script[0]) fails |= 1;
    if (car.count != 1 || car.put != &gObject || car.slot != 3 || !car.flag) fails |= 2;
    gNextCarryable = 0; a.PerformAction();
    if (gCreated != 2 || car.count != 1) fails |= 4;
    a.ItemDefIndex = 0; a.PerformAction();
    a.ItemDefIndex = 42; c.carrying = 0; a.PerformAction();
    if (gCreated != 2) fails |= 8;
    if (fails) { std::printf("00849e80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00849e80_TEST PASS\n");
    return 0;
}