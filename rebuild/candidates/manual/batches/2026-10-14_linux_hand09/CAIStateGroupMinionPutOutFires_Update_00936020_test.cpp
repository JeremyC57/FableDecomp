#include <cstdio>
struct CThing { unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
struct CBaseIntelligentPointer {
    CThing* item; int sets;
    CBaseIntelligentPointer() : item(0), sets(0) { }
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; ++sets; }
};
// the inlined GetInterface: presence bit, then the single-entry map
template<class T> bool GetInterface(CThing* t, T** out, long type)
{
    if (t->bits[type >> 5] & (1UL << (type & 31))) { *out = (t->compType == type) ? (T*)t->comp : 0; return true; }
    return false;
}
static void GiveTC(CThing* t, long type, void* comp) { t->bits[type >> 5] |= 1UL << (type & 31); t->compType = type; t->comp = comp; }
struct CGroup {
    CBaseIntelligentPointer PBuildingOnFire; bool terminated; int wells, fires, waits; const char* script; // script: w=GoToWell result, f=PutOutFire, d=DoNothing(4 or 0)
    CGroup(const char* s) : terminated(false), wells(0), fires(0), waits(0), script(s) { }
    bool GoToWell() { ++wells; return *script++ == '1'; }
    bool PutOutFire() { ++fires; return *script++ == '1'; }
    long AIAction_DoNothing() { ++waits; return *script++ == '4' ? 4 : 0; }
    void Terminate() { terminated = true; }
    void Update()
    {
        if (!(PBuildingOnFire.GetPItem() != 0 && !(PBuildingOnFire.GetPItem()->flags91 & 1))) return;
        do {
            if (!GoToWell()) { Terminate(); return; }
            if (!PutOutFire()) { Terminate(); return; }
        } while (AIAction_DoNothing() == 4);
    }
};
int main()
{
    CThing b; bool ok = true;
    { CGroup g("114110"); g.PBuildingOnFire.SetPItem(&b); g.Update(); ok = ok && !g.terminated && g.wells == 2 && g.fires == 2 && g.waits == 2; }
    { CGroup g("10"); g.PBuildingOnFire.SetPItem(&b); g.Update(); ok = ok && g.terminated && g.wells == 1 && g.fires == 1 && g.waits == 0; }
    { CGroup g("0"); g.PBuildingOnFire.SetPItem(&b); g.Update(); ok = ok && g.terminated && g.wells == 1 && g.fires == 0; }
    { CGroup g("1"); g.Update(); ok = ok && !g.terminated && g.wells == 0; }
    { CThing dead; dead.flags91 = 1; CGroup g("1"); g.PBuildingOnFire.SetPItem(&dead); g.Update(); ok = ok && !g.terminated && g.wells == 0; }
    if (!ok) { std::printf("00936020_TEST FAIL\n"); return 1; }
    std::printf("00936020_TEST PASS\n"); return 0;
}
