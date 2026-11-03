// Behaviour test for PutSheatheObjectInCarrySlot (0x005C36C2, anonymous namespace of the CTCInventoryWeapons TU): model.
#include <cstdio>
struct Thing { bool alive; long sheatheObject; bool carryable; float offsetAmount; float onTop; float passiveScale; float renderSize; float scale; long passiveSlot; bool limbo; bool killed; };
struct Carrier { Thing* slots[4];
    bool IsCarrying(const Thing* t) const { for (int i = 0; i < 4; ++i) if (slots[i] == t) return true; return false; }
};
static Thing g_created = { true, -1, true, 0, 0.5f, 0.75f, 2.0f, 1.0f, 2, true, false };
static void PutSheatheObjectInCarrySlot(Carrier& c, Thing& weapon, float& offset)
{
    long slot = weapon.sheatheObject;
    if (c.IsCarrying(&weapon) && slot <= 0) return;
    Thing* s = &weapon;
    if (slot > 0) s = &g_created; else if (slot == 0) s = 0;
    if (s && s->alive && s->carryable) {
        s->offsetAmount = offset;
        offset += s->onTop;
        s->scale = s->passiveScale * s->renderSize;
        Thing* old = c.slots[s->passiveSlot];
        if (old && old->alive) old->killed = true;
        s->limbo = false;
        c.slots[s->passiveSlot] = s;
    }
}
int main()
{
    Thing old = { true, 0, false, 0, 0, 0, 0, 0, 0, false, false };
    Thing weapon = { true, 5, true, 0, 0, 0, 0, 1.0f, 1, false, false };
    Carrier c = { { 0, 0, &old, 0 } };
    float offset = 0.25f;
    PutSheatheObjectInCarrySlot(c, weapon, offset);
    if (c.slots[2] != &g_created || !old.killed || g_created.limbo || g_created.offsetAmount != 0.25f || offset != 0.75f || g_created.scale != 1.5f) {
        std::printf("005c36c2_TEST FAIL\n"); return 1;
    }
    std::printf("005c36c2_TEST PASS\n");
    return 0;
}
