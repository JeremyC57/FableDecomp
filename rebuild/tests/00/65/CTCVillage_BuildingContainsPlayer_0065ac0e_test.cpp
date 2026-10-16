// Behaviour test for CTCVillage::BuildingContainsPlayer (0x0065AC0E): a live hero whose location component names the
// building.
#include <cstdio>
struct Hero { bool alive; bool hasLocation; void* building; };
static bool Contains(const Hero* h, void* building)
{
    if (h && h->alive && h->hasLocation) { if (h->building == building) return true; }
    return false;
}
int main()
{
    int house, shop;
    Hero a = { true, true, &house }, b = { false, true, &house }, c = { true, false, &house };
    if (!Contains(&a, &house) || Contains(&a, &shop) || Contains(&b, &house) || Contains(&c, &house) || Contains(0, &house)) {
        std::printf("0065ac0e_TEST FAIL\n"); return 1;
    }
    std::printf("0065ac0e_TEST PASS\n");
    return 0;
}