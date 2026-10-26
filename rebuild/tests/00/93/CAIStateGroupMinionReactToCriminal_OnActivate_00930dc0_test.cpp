#include <cstdio>
class CThing { public: unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
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
struct CTCModes { unsigned long lastMode; bool AddMode(unsigned long m) { lastMode = m; return true; } };
struct CGroup { CThing* creature; long FrameToNextSpeak; bool OverrodeLocking, HeroInsideHouse, MeInsideHouse, GuardInsideHouse;
    void OnActivate() { CTCModes* p; GetInterface(creature, &p, 0x31); p->AddMode(15); FrameToNextSpeak = 0; OverrodeLocking = false; HeroInsideHouse = false; MeInsideHouse = false; } };
int main()
{
    CThing c; CTCModes m; m.lastMode = 0; GiveTC(&c, 0x31, &m);
    CGroup g; g.creature = &c; g.FrameToNextSpeak = 99; g.OverrodeLocking = g.HeroInsideHouse = g.MeInsideHouse = g.GuardInsideHouse = true;
    g.OnActivate();
    bool ok = m.lastMode == 15 && g.FrameToNextSpeak == 0 && !g.OverrodeLocking && !g.HeroInsideHouse && !g.MeInsideHouse && g.GuardInsideHouse;
    if (!ok) { std::printf("00930dc0_TEST FAIL\n"); return 1; }
    std::printf("00930dc0_TEST PASS\n"); return 0;
}