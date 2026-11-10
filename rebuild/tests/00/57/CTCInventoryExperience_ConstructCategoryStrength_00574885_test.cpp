// Behaviour test for CTCInventoryExperience::ConstructCategoryStrength (0x00574885): a model of the console category build.
// Three branch entries are appended in order strength, skill, will; strength and skill count the buyable stats of their three
// stats, will sums the counts of its three spell sub-lists (7, 5 and 6 abilities) and the magic-power stat. Each branch fades
// the two sliders that are not its own (strength: 2,3; skill: 1,3; will: 1,2) and shares one on-selected highlight action.
#include <cstdio>
#include <string>
#include <vector>
struct SBranch { std::string name; long items; int fadeA, fadeB; int highlight; };
static long CountBuyable(const bool* buyable, int first, int count)
{
    long n = 0;
    for (int i = first; i < first + count; ++i)
        if (buyable[i]) ++n;
    return n;
}
static void Construct(const bool* stats, const bool* abilities, std::vector<SBranch>& out)
{
    const int highlight = 1;
    SBranch b;
    b.name = "STRENGTH"; b.items = CountBuyable(stats, 0, 3); b.fadeA = 2; b.fadeB = 3; b.highlight = highlight; out.push_back(b);
    b.name = "SKILL"; b.items = CountBuyable(stats, 3, 3); b.fadeA = 1; b.fadeB = 3; out.push_back(b);
    long will = CountBuyable(abilities, 0, 7);
    will += CountBuyable(abilities, 7, 5);
    will += CountBuyable(abilities, 12, 6);
    will += CountBuyable(stats, 6, 1);
    b.name = "WILL"; b.items = will; b.fadeA = 1; b.fadeB = 2; out.push_back(b);
}
int main()
{
    int fails = 0;
    bool stats[7] = { true, false, true, true, true, true, false };
    bool abilities[18];
    for (int i = 0; i < 18; ++i) abilities[i] = (i % 3) == 0;   // 0,3,6 | 9 | 12,15 -> 3 + 1 + 2
    std::vector<SBranch> out;
    Construct(stats, abilities, out);
    if (out.size() != 3) ++fails;
    else {
        if (out[0].name != "STRENGTH" || out[0].items != 2 || out[0].fadeA != 2 || out[0].fadeB != 3) ++fails;
        if (out[1].name != "SKILL" || out[1].items != 3 || out[1].fadeA != 1 || out[1].fadeB != 3) ++fails;
        if (out[2].name != "WILL" || out[2].items != 6 || out[2].fadeA != 1 || out[2].fadeB != 2) ++fails;
        if (out[0].highlight != out[2].highlight) ++fails;
    }
    stats[6] = true;
    out.clear();
    Construct(stats, abilities, out);
    if (out.size() != 3 || out[2].items != 7) ++fails;
    std::printf(fails ? "00574885_TEST FAIL\n" : "00574885_TEST PASS\n");
    return fails;
}