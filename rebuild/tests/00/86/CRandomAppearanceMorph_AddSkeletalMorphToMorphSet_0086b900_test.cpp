// Behaviour test for 0x0086B900: AddSkeletalMorphToMorphSet appends {name of the morph's def string, strength}; strength is
// a random value in [0, 1) drawn from the seed when the morph allows variation, else 1.0; the seed advances only then.
#include <cstdio>
#include <string>
#include <vector>

static const char* g_Table[3] = { "MorphA", "MorphB", "MorphC" };
struct CDefStringTable { std::string GetString(int tablePos) const { return g_Table[tablePos]; } };
static CDefStringTable g_DefStringTable;

static float GFFloatRandom(float range, unsigned int* seed)
{
    *seed = *seed * 0x24A1u + 0x24DFu;
    return range * (float)(*seed % 1000u) / 1000.0f;
}

struct CDefString { int TablePos; };
struct CSkeletalMorph { int GroupID; CDefString Name; bool AllowVariation; };
struct CMorphSetSkeletalMorph { std::string Name; float Strength; };
struct CMorphSet { std::vector<CMorphSetSkeletalMorph> SkeletalMorphs; };

static void AddSkeletalMorphToMorphSet(unsigned int& seed, const CSkeletalMorph& morph, CMorphSet& morphSet)
{
    CMorphSetSkeletalMorph entry;
    {
        std::string name = g_DefStringTable.GetString(morph.Name.TablePos);
        entry.Name = name;
    }
    if (morph.AllowVariation)
        entry.Strength = GFFloatRandom(1.0f, &seed);
    else
        entry.Strength = 1.0f;
    morphSet.SkeletalMorphs.push_back(entry);
}

int main()
{
    unsigned int seed = 7;
    CMorphSet set;
    CSkeletalMorph fixed = { 1, { 2 }, false };
    CSkeletalMorph varied = { 1, { 0 }, true };
    int fails = 0;
    AddSkeletalMorphToMorphSet(seed, fixed, set);
    if (seed != 7 || set.SkeletalMorphs.size() != 1 || set.SkeletalMorphs[0].Name != "MorphC" || set.SkeletalMorphs[0].Strength != 1.0f) ++fails;
    AddSkeletalMorphToMorphSet(seed, varied, set);
    if (seed == 7 || set.SkeletalMorphs.size() != 2 || set.SkeletalMorphs[1].Name != "MorphA") ++fails;
    if (!(set.SkeletalMorphs[1].Strength >= 0.0f && set.SkeletalMorphs[1].Strength < 1.0f)) ++fails;
    if (fails) { std::printf("0086b900_TEST FAIL\n"); return 1; }
    std::printf("0086b900_TEST PASS\n");
    return 0;
}