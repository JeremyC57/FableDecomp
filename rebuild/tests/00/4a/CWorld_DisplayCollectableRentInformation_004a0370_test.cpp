// Behaviour test for 0x004A0370 (CWorld::DisplayCollectableRentInformation): sum collectable and maximum rent over the
// rented buyable houses; with rent to collect and a living hero that has a region display, write the formatted
// collectable-rent text into the display's extra area-name line.
#include <cstdio>
#include <cstring>
#include <vector>
struct House { bool rented; long current; long maximum; };
static std::vector<House> g_houses;
static bool g_heroAlive = true;
static bool g_hasDisplay = true;
static char g_display[64];
static void DisplayCollectableRentInformation()
{
    std::vector<const House*> houses;
    for (size_t k = 0; k < g_houses.size(); ++k) houses.push_back(&g_houses[k]);
    unsigned int count = (unsigned int)houses.size();
    long collectableRent = 0, maxRent = 0;
    for (unsigned int i = 0; i < count; ++i) {
        const House* house = houses[i];
        if (house->rented) {
            collectableRent += house->current;
            maxRent += house->maximum;
        }
    }
    if (collectableRent > 0 && g_heroAlive && g_hasDisplay)
        std::sprintf(g_display, "Rent: %ld of %ld", collectableRent, maxRent);
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    House a = { true, 10, 50 }, b = { false, 99, 99 }, c = { true, 5, 20 };
    g_houses.push_back(a); g_houses.push_back(b); g_houses.push_back(c);
    DisplayCollectableRentInformation();
    CHECK(std::strcmp(g_display, "Rent: 15 of 70") == 0);
    g_display[0] = 0; g_heroAlive = false; DisplayCollectableRentInformation();
    CHECK(g_display[0] == 0);
    g_heroAlive = true; g_houses[0].current = 0; g_houses[2].current = 0;
    DisplayCollectableRentInformation();
    CHECK(g_display[0] == 0);                       // nothing to collect
    if (g_fail) { std::printf("004a0370_TEST FAIL\n"); return 1; }
    std::printf("004a0370_TEST PASS\n");
    return 0;
}