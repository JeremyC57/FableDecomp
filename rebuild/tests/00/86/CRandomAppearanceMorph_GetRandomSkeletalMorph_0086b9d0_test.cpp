// Behaviour test for CRandomAppearanceMorph::GetRandomSkeletalMorph (0x0086B9D0): the distinct group keys of the sorted skeletal-morph
// map (GetKeyList); group 0 adds every morph that allows variation and each other one when GFRandom(100) >= 50; any other group, when
// GFRandom(100) > 50, adds the morph at index GFRandom(count of the group) inside the group. GFRandom: seed = ROR13(seed * 0x24A1 +
// 0x24DF), result seed % range (0 for a zero range). Expected lists computed independently (python reference) and hard-coded; seeds
// 22 / 97 hit a draw of exactly 50 in group 0 / in a later group (kills >= / > mutants); seed 1 kills the AllowVariation test.
#include <cstdio>
#include <cstring>
#include <vector>

static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = ROR13(*seed);
    if (range) return *seed % range;
    return 0;
}
struct Morph { long group; char name; bool allow; };
static const Morph M[] = { {0,'a',true}, {0,'b',false}, {0,'c',false}, {0,'d',false}, {3,'e',false}, {3,'f',true}, {3,'g',false}, {7,'h',false} };
static const int N = 8;

static void GetKeyList(std::vector<long>& key_list)
{
    bool no_key = true; long last_key = 0;
    for (int i = 0; i < N; ++i)
        if (no_key || M[i].group != last_key) { key_list.push_back(M[i].group); last_key = M[i].group; no_key = false; }
}
static int LowerBound(long g) { int i = 0; while (i < N && M[i].group < g) ++i; return i; }
static int UpperBound(long g) { int i = 0; while (i < N && !(g < M[i].group)) ++i; return i; }

static void GetRandomSkeletalMorph(unsigned long& seed, char* out)
{
    int n = 0;
    std::vector<long> keys;
    GetKeyList(keys);
    for (std::vector<long>::iterator it_group = keys.begin(); it_group != keys.end(); ++it_group) {
        long group = *it_group;
        if (group == 0) {
            int upper_bound = UpperBound(group);
            for (int it = LowerBound(group); it != upper_bound; ++it)
                if (M[it].allow || GFRandom(100, &seed) >= 50) out[n++] = M[it].name;
        } else if (GFRandom(100, &seed) > 50) {
            long no_morphs = UpperBound(group) - LowerBound(group);
            int it = LowerBound(group) + (int)GFRandom(no_morphs, &seed);
            out[n++] = M[it].name;
        }
    }
    out[n] = 0;
}

static int check(unsigned long seed0, const char* expected, unsigned long seed_after)
{
    char out[16]; unsigned long seed = seed0;
    GetRandomSkeletalMorph(seed, out);
    if (std::strcmp(out, expected) != 0 || seed != seed_after) { std::printf("0086b9d0_TEST FAIL seed %lu got %s\n", seed0, out); return 1; }
    return 0;
}

int main()
{
    if (check(1, "acdeh", 1125611668UL)) return 1;
    if (check(2, "abgh", 123697772UL)) return 1;
    if (check(6, "acdf", 1015894946UL)) return 1;
    if (check(12, "a", 370302208UL)) return 1;
    if (check(22, "acd", 3170988635UL)) return 1;
    if (check(97, "abch", 3773080269UL)) return 1;
    std::printf("0086b9d0_TEST PASS\n");
    return 0;
}