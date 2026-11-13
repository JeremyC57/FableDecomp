// Behaviour test for 0x0073B130 CTCCreatureModeManager::RegisterModes: a copy of the registry model (vector of {key, alloc} pairs,
// Insert = push_back + Dirty, Update = sort by key when Dirty), registering a sample of the retail table, then checks the sorted order,
// the stable presence of the three MODE_STAND allocators and the Dirty flag.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
class CThing;
struct CModeBase { int id; };
typedef CModeBase* (__fastcall* AllocFn)(CThing&);
static CModeBase g_modes[8];
static CModeBase* __fastcall AllocNull(CThing&) { return &g_modes[0]; }
static CModeBase* __fastcall AllocStandFront(CThing&) { return &g_modes[1]; }
static CModeBase* __fastcall AllocStandLeft(CThing&) { return &g_modes[2]; }
static CModeBase* __fastcall AllocStandRight(CThing&) { return &g_modes[3]; }
static CModeBase* __fastcall AllocWeapon(CThing&) { return &g_modes[4]; }
static CModeBase* __fastcall AllocEpic(CThing&) { return &g_modes[5]; }
static CModeBase* __fastcall AllocRoll(CThing&) { return &g_modes[6]; }
struct CRegisteredModeInfo { AllocFn AllocFunc; CRegisteredModeInfo(AllocFn f) : AllocFunc(f) {} };
struct Less { bool operator()(const std::pair<unsigned long, CRegisteredModeInfo>& a, const std::pair<unsigned long, CRegisteredModeInfo>& b) const { return a.first < b.first; } };
struct Map {
    std::vector<std::pair<unsigned long, CRegisteredModeInfo> > v; Less Compare; bool Dirty;
    Map() : Dirty(false) {}
    void Insert(const unsigned long& k, const CRegisteredModeInfo& i) { v.push_back(std::make_pair(k, i)); Dirty = true; }
    void Update() { if (Dirty) { std::sort(v.begin(), v.end(), Compare); Dirty = false; } }
};
static Map RegisteredModes;
static void RegisterModes()
{
    RegisteredModes.Insert(0, CRegisteredModeInfo(AllocNull));
    RegisteredModes.Insert(4, CRegisteredModeInfo(AllocWeapon));
    RegisteredModes.Insert(1, CRegisteredModeInfo(AllocStandFront));
    RegisteredModes.Insert(1, CRegisteredModeInfo(AllocStandLeft));
    RegisteredModes.Insert(1, CRegisteredModeInfo(AllocStandRight));
    RegisteredModes.Insert(95, CRegisteredModeInfo(AllocEpic));
    RegisteredModes.Insert(70, CRegisteredModeInfo(AllocRoll));
    RegisteredModes.Update();
}
int main()
{
    RegisterModes();
    const std::vector<std::pair<unsigned long, CRegisteredModeInfo> >& v = RegisteredModes.v;
    bool ok = v.size() == 7 && !RegisteredModes.Dirty;
    for (size_t i = 1; ok && i < v.size(); ++i) if (v[i - 1].first > v[i].first) ok = false;
    int stand = 0; CThing* t = 0;
    for (size_t i = 0; ok && i < v.size(); ++i) {
        if (v[i].first == 1) { CModeBase* m = v[i].second.AllocFunc(*t); if (m->id != 0 || (m != &g_modes[1] && m != &g_modes[2] && m != &g_modes[3])) ok = false; ++stand; }
    }
    if (ok && (stand != 3 || v[0].first != 0 || v[0].second.AllocFunc != AllocNull || v[6].first != 95 || v[5].second.AllocFunc != AllocRoll)) ok = false;
    if (!ok) { std::printf("0073b130_TEST FAIL\n"); return 1; }
    std::printf("0073b130_TEST PASS\n");
    return 0;
}