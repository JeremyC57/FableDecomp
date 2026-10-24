// behaviour test for 0x00AF28C0 NParticleEngine::CPSCManager::AllocatePSC(const CCharString&): look the name up in the
// sorted registered-PSC map and call its allocator; 0 for an unknown name
#include <cstdio>
#include <cstring>
struct CPSCBase { int Kind; };
static CPSCBase gWind = { 1 }, gSpin = { 2 }, gAlpha = { 3 };
static CPSCBase* AllocWind() { return &gWind; }
static CPSCBase* AllocSpin() { return &gSpin; }
static CPSCBase* AllocAlpha() { return &gAlpha; }
struct SInfo { CPSCBase* (*AllocFunc)(); int ComponentType; };
struct SEntry { const char* first; SInfo second; };
static bool Less(const char* a, const char* b) { return std::strcmp(a, b) < 0; }
struct SManager {
    SEntry* First; SEntry* Last;
    SEntry* LowerBound(const char* key)
    {
        SEntry* f = First; long n = (long)(Last - First);
        while (n > 0) { long h = n / 2; SEntry* m = f + h; if (Less(m->first, key)) { f = m + 1; n -= h + 1; } else n = h; }
        return f;
    }
    SEntry* Find(const char* key) { SEntry* it = LowerBound(key); if (it == Last || Less(key, it->first)) return Last; return it; }
    CPSCBase* AllocatePSC(const char* name)
    {
        SEntry* it = Find(name);
        if (it != Last)
            return it->second.AllocFunc();
        return 0;
    }
};
int main()
{
    SEntry e[3];
    e[0].first = "Alpha"; e[0].second.AllocFunc = &AllocAlpha;
    e[1].first = "Spin"; e[1].second.AllocFunc = &AllocSpin;
    e[2].first = "Wind"; e[2].second.AllocFunc = &AllocWind;
    SManager m; m.First = e; m.Last = e + 3;
    if (m.AllocatePSC("Spin") != &gSpin || m.AllocatePSC("Wind") != &gWind || m.AllocatePSC("Alpha") != &gAlpha) { std::printf("00af28c0_TEST FAIL\n"); return 1; }
    if (m.AllocatePSC("Size") != 0 || m.AllocatePSC("Zoom") != 0 || m.AllocatePSC("A") != 0) { std::printf("00af28c0_TEST FAIL\n"); return 1; }
    m.Last = e;                                             // empty map
    if (m.AllocatePSC("Spin") != 0) { std::printf("00af28c0_TEST FAIL\n"); return 1; }
    std::printf("00af28c0_TEST PASS\n");
    return 0;
}