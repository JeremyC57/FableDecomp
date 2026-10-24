// behaviour test for 0x00AF4360 NParticleEngine::CParticleSystem::GetComponentOfType(EPSCType, unsigned long index): the index-th component of that type from the sorted component map, else 0
#include <cstdio>
struct SEntry { long first; int* Component; };
struct SSystem {
    SEntry Entries[8]; int Count;
    SEntry* LowerBound(long key) { SEntry* f = Entries; long n = Count; while (n > 0) { long h = n / 2; SEntry* m = f + h; if (m->first < key) { f = m + 1; n -= h + 1; } else n = h; } return f; }
    SEntry* UpperBound(long key) { SEntry* f = Entries; long n = Count; while (n > 0) { long h = n / 2; SEntry* m = f + h; if (!(key < m->first)) { f = m + 1; n -= h + 1; } else n = h; } return f; }
    int* GetComponentOfType(long type, unsigned int index)
    {
        SEntry* it = LowerBound(type);
        SEntry* last = UpperBound(type);
        for (unsigned int i = 0; i < index; ++i) {
            if (it == last)
                return 0;
            ++it;
        }
        if (it != last)
            return it->Component;
        return 0;
    }
};
int main()
{

    int c[6];
    SSystem s;
    long types[6] = { 1, 3, 3, 3, 5, 8 };
    for (int i = 0; i < 6; ++i) { s.Entries[i].first = types[i]; s.Entries[i].Component = &c[i]; }
    s.Count = 6;
    if (s.GetComponentOfType(3, 0) != &c[1] || s.GetComponentOfType(3, 2) != &c[3]) { std::printf("00af4360_TEST FAIL\n"); return 1; }
    if (s.GetComponentOfType(3, 3) != 0) { std::printf("00af4360_TEST FAIL\n"); return 1; }              // runs out of the range
    if (s.GetComponentOfType(8, 0) != &c[5] || s.GetComponentOfType(8, 1) != 0) { std::printf("00af4360_TEST FAIL\n"); return 1; }
    if (s.GetComponentOfType(4, 0) != 0 || s.GetComponentOfType(0, 0) != 0 || s.GetComponentOfType(9, 0) != 0) { std::printf("00af4360_TEST FAIL\n"); return 1; }
    std::printf("00af4360_TEST PASS\n");
    return 0;
}