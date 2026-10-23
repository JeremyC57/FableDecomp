// Behaviour test for CTCInventoryClothing::GetCurrentTotalArmorValue @ 0x005B4A95
#include <cstdio>
#include <set>
#include <list>
#include <map>

struct Modifier { int suitPart; float weighting; int armour; };
static std::map<int, Modifier> g_mods;                 // def index -> appearance modifier
static std::map<int, std::map<int, float> > g_armour;  // armour def -> damage factors
static int NoClothingDef(int part) { return 100 + part; }

static int ArmourOf(const Modifier& m)
{
    float factor = 1.0f;
    std::map<int, float>& f = g_armour[m.armour];
    std::map<int, float>::iterator it = f.find(0);
    if (it != f.end())
        factor = it->second;
    return (int)(m.weighting / factor * 500.0f);
}

static int GetCurrentTotalArmorValue(const std::list<int>* worn)
{
    std::set<int> uncovered;
    for (int p = 1; p <= 5; ++p)
        uncovered.insert(p);
    int total = 0;
    if (worn) {
        for (std::list<int>::const_iterator it = worn->begin(); it != worn->end(); ++it) {
            std::map<int, Modifier>::iterator m = g_mods.find(*it);
            if (m != g_mods.end()) {
                uncovered.erase(m->second.suitPart);
                total += ArmourOf(m->second);
            }
        }
    }
    for (std::set<int>::iterator p = uncovered.begin(); p != uncovered.end(); ++p) {
        int index = 0;
        if (*p >= 1 && *p <= 5)
            index = NoClothingDef(*p);
        std::map<int, Modifier>::iterator m = g_mods.find(index);
        if (m != g_mods.end())
            total += ArmourOf(m->second);
    }
    return total;
}

int main()
{
    for (int p = 1; p <= 5; ++p) { Modifier m = { p, 0.125f, 1 }; g_mods[NoClothingDef(p)] = m; }
    g_armour[1][0] = 1.0f;
    Modifier helmet = { 1, 0.5f, 2 };
    g_mods[7] = helmet;
    g_armour[2][0] = 0.5f;
    if (GetCurrentTotalArmorValue(0) != 5 * 62) { std::printf("005b4a95_TEST FAIL\n"); return 1; }
    std::list<int> worn; worn.push_back(7); worn.push_back(55);
    if (GetCurrentTotalArmorValue(&worn) != 4 * 62 + 500) { std::printf("005b4a95_TEST FAIL\n"); return 1; }
    std::printf("005b4a95_TEST PASS\n");
    return 0;
}