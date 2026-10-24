// Behaviour test for 0x0071CF70 AddMorph: each morph of the requested type is appended with the given weight; nothing when the
// lookup fails.
#include <cstdio>

struct SEntry { int MorphName; short Priority; bool Supresses; };
struct SWeighted { SEntry first; float second; };
struct SMorphs {
    SEntry entries[4]; int types[4]; int count;
    bool GetAllMorphsOfType(int type, SEntry* out, int& n) const
    {
        n = 0;
        for (int i = 0; i < count; ++i) if (types[i] == type) out[n++] = entries[i];
        return n != 0;
    }
};
static void AddMorph(const SMorphs& morphs, int type, float weight, SWeighted* out, int& outCount)
{
    SEntry found[4]; int n = 0;
    if (morphs.GetAllMorphsOfType(type, found, n))
        for (int i = 0; i < n; ++i) { out[outCount].first = found[i]; out[outCount].second = weight; ++outCount; }
}
int main()
{
    SMorphs m; m.count = 3;
    for (int i = 0; i < 3; ++i) { m.entries[i].MorphName = 10 + i; m.entries[i].Priority = (short)i; m.entries[i].Supresses = false; }
    m.types[0] = 1; m.types[1] = 2; m.types[2] = 1;
    SWeighted out[8]; int n = 0;
    AddMorph(m, 1, 0.75f, out, n);
    bool ok = n == 2 && out[0].first.MorphName == 10 && out[1].first.MorphName == 12 && out[0].second == 0.75f && out[1].second == 0.75f;
    AddMorph(m, 7, 1.0f, out, n);
    ok = ok && n == 2;
    if (!ok) { std::printf("0071cf70_TEST FAIL\n"); return 1; }
    std::printf("0071cf70_TEST PASS\n");
    return 0;
}