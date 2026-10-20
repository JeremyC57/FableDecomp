// Behaviour test for CTCHeroCentre::OpenTrainingScreen (retail 0x0077D4B0): model copy + stub callees.
#include <cstdio>
#include <cstring>

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
static long g_tutorialCategory = -1;
struct CTCHero { bool pending; bool GiveTutorialForCategory(long category, bool) { g_tutorialCategory = category; bool r = pending; pending = false; return r; } };
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroThing() { return g_hero; } };
struct CPlayerManager {
    CPlayer player;
    CPlayer* GetMainPlayer() const { return const_cast<CPlayer*>(&player); }
    long GetMainPlayerNumber() { return 2; }
};
static int g_events = 0; static long g_lastType = 0; static unsigned long g_lastPayload = 0; static int g_lastPlayer = 0;
struct CMemoryBuffer {
    unsigned char bytes[8]; unsigned long used;
    void Copy(unsigned long length, const void* data) { std::memcpy(bytes, data, length); used = length; }
};
struct CGameEvent : CMemoryBuffer {
    long Type; signed char Player; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { used = 0; }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& e) { ++g_events; g_lastType = e.Type; std::memcpy(&g_lastPayload, e.bytes, 4); g_lastPlayer = e.Player; }
};
static CMainGameComponent g_main;
CMainGameComponent* g_pMainGameComponent = &g_main;
static CPlayerManager g_pm;
struct CTCBase { CPlayerManager* GetPlayerManager() const { return &g_pm; } };
struct CTCHeroCentre : CTCBase {
    bool WaitingForTutorialToFinish;
    void OpenTrainingScreen();
};
void CTCHeroCentre::OpenTrainingScreen()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !(hero->m_flags91 & 1)) {
        CTCHero* heroTC;
        if (hero->GetInterface(&heroTC, 0x29) && heroTC && heroTC->GiveTutorialForCategory(0xf, false)) {
            WaitingForTutorialToFinish = true;
            return;
        }
        if (hero->HasTC(0x16)) {
            CGameEvent event(0x1a);
            event.AddULONG(0xb);
            event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(event);
        }
    }
}
int main()
{
    int fail = 0;
    CTCHero heroTC = { true };
    CInterfaceMap::SEntry entries[1] = { { 0x29, (CTCBase*)&heroTC } };
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[1] = 1UL << 9; hero.m_presentBits[0] = 1UL << 22; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1; hero.m_flags91 = 0;
    g_hero = &hero;
    CTCHeroCentre centre; centre.WaitingForTutorialToFinish = false;
    centre.OpenTrainingScreen();      // tutorial first
    if (!centre.WaitingForTutorialToFinish || g_tutorialCategory != 0xf || g_events != 0) fail = 1;
    centre.OpenTrainingScreen();      // tutorial seen: open the screen
    if (g_events != 1 || g_lastType != 0x1a || g_lastPayload != 0xb || g_lastPlayer != 2) fail = 2;
    hero.m_presentBits[0] = 0;        // cannot train: nothing
    centre.OpenTrainingScreen();
    if (g_events != 1) fail = 3;
    hero.m_flags91 = 1; hero.m_presentBits[0] = 1UL << 22;   // dead hero: nothing
    centre.OpenTrainingScreen();
    if (g_events != 1) fail = 4;
    if (fail) { std::printf("0077d4b0_TEST FAIL\n"); return 1; }
    std::printf("0077d4b0_TEST PASS\n");
    return 0;
}
