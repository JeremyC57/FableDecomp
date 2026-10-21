// Behaviour test for CTCWife::FindMaritalHome (0x006CFAA0): the village comes from the wife's own village membership
// when it has one, else from the first thing found carrying a village component; null when neither gives a village.
// The first live building whose buyable house is owned by the player, not rented, not in use and has several dress
// levels is returned; dead buildings, non-houses and houses failing any test are skipped.
#include <cstdio>
#include <list>

struct House { bool owned, rented, inUse, multiDress; };
struct Building { bool dead; House* house; };
struct Village { std::list<Building*> buildings; };
struct Thing { Village* memberOf; Village* village; };

static Thing* g_searchResult = 0;

static Building* FindMaritalHome(const Thing* parent)
{
    Village* village = 0;
    if (parent->memberOf != 0)
        village = parent->memberOf;
    else {
        Thing* thing = g_searchResult;
        if (thing != 0 && thing->village != 0)
            village = thing->village;
    }
    if (village != 0) {
        for (std::list<Building*>::iterator it = village->buildings.begin(); it != village->buildings.end(); ++it) {
            Building* b = *it;
            if (b != 0 && !b->dead && b->house != 0) {
                const House* h = b->house;
                if (h->owned && !h->rented && !h->inUse && h->multiDress)
                    return b;
            }
        }
    }
    return 0;
}

int main()
{
    int ok = 1;
    House rented = { true, true, false, true }, notOwned = { false, false, false, true }, used = { true, false, true, true };
    House plain = { true, false, false, false }, good = { true, false, false, true };
    Building b0 = { false, 0 }, b1 = { false, &rented }, b2 = { true, &good }, b3 = { false, &notOwned };
    Building b4 = { false, &used }, b5 = { false, &plain }, b6 = { false, &good }, b7 = { false, &good };
    Village v;
    v.buildings.push_back(0); v.buildings.push_back(&b0); v.buildings.push_back(&b1); v.buildings.push_back(&b2);
    v.buildings.push_back(&b3); v.buildings.push_back(&b4); v.buildings.push_back(&b5); v.buildings.push_back(&b6);
    v.buildings.push_back(&b7);
    Thing wife = { &v, 0 };
    ok &= FindMaritalHome(&wife) == &b6;                     // own village, first good house
    Thing loner = { 0, 0 };
    ok &= FindMaritalHome(&loner) == 0;                      // no member village, nothing found
    Thing other = { 0, &v };
    g_searchResult = &other;
    ok &= FindMaritalHome(&loner) == &b6;                    // village from the search
    Village empty; Thing e = { &empty, 0 };
    ok &= FindMaritalHome(&e) == 0;                          // empty village
    std::printf(ok ? "006cfaa0_TEST PASS\n" : "006cfaa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
