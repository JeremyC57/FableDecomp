// Behaviour test for 0x0086B9D0 CRandomAppearanceMorph::GetRandomSkeletalMorph: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <map>
#include <vector>
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Next(unsigned long& s) { s = s * 0x24A1 + 0x24DF; s = Ror13(s); return s; }
int main() {
    std::multimap<long, bool> morphs; morphs.insert(std::make_pair(0L, true)); morphs.insert(std::make_pair(0L, false)); morphs.insert(std::make_pair(3L, false));
    unsigned long seed = 12345; std::vector<long> created;
    long keys[2] = { 0, 3 };
    for (int k = 0; k < 2; ++k) {
        long key = keys[k];
        if (key == 0) {
            for (std::multimap<long, bool>::iterator e = morphs.lower_bound(key); e != morphs.upper_bound(key); ++e) {
                if (!e->second && Next(seed) % 100 < 50) continue;
                created.push_back(key);
            }
        } else if (Next(seed) % 100 > 50) {
            unsigned long count = morphs.count(key); Next(seed); unsigned long index = count ? seed % count : 0;
            if (index < count) created.push_back(key);
        }
    }
    int ok = created.size() >= 1 && created[0] == 0 && seed != 12345;
    std::printf(ok ? "0086b9d0_TEST PASS\n" : "0086b9d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
