#include <cstdio>
class C3DVector { public: float x, y, z; };
struct Thing { bool building; C3DVector pos; };
struct Creature { int placedBuilding, placedNear; float radius; bool AutoPlaceNearBuilding(Thing*, float r) { ++placedBuilding; radius = r; return true; } bool AutoPlaceNear(const C3DVector&, float r) { ++placedNear; radius = r; return true; } };
struct Group {
    bool workHours; unsigned long seed; unsigned long chance; Thing* shopToPick; Thing* Shop; Creature* creature;
    unsigned long Random(unsigned long range) { seed = seed * 0x24A1U + 0x24DFU; seed = (seed >> 13) | (seed << 19); return seed % range; }
    bool InitCreaturePosition()
    {
        bool ok = workHours && Random(100) < chance && (Shop = shopToPick) != 0;
        if (ok) {
            if (Shop->building) return creature->AutoPlaceNearBuilding(Shop, 5.0f);
            return creature->AutoPlaceNear(Shop->pos, 5.0f);
        }
        return false;
    }
};
int main()
{
    Thing shop = { true, { 1, 2, 3 } }; Creature c = { 0, 0, 0 };
    Group g = { true, 7, 100, &shop, 0, &c };
    if (!g.InitCreaturePosition() || c.placedBuilding != 1 || c.radius != 5.0f || g.Shop != &shop) { std::printf("009473e0_TEST FAIL\n"); return 1; }
    shop.building = false;
    if (!g.InitCreaturePosition() || c.placedNear != 1) { std::printf("009473e0_TEST FAIL\n"); return 1; }
    g.chance = 0;
    if (g.InitCreaturePosition()) { std::printf("009473e0_TEST FAIL\n"); return 1; }
    g.chance = 100; g.shopToPick = 0;
    if (g.InitCreaturePosition()) { std::printf("009473e0_TEST FAIL\n"); return 1; }
    std::printf("009473e0_TEST PASS\n");
    return 0;
}