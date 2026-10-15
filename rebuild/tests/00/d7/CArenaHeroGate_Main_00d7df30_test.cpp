// Behaviour test for CArenaHeroGate::Main (0x00D7DF30): close / wait / open / wait / close, with the
// abort test ending the script inside a wait or between the steps.
#include <cstdio>

struct CScriptThing { long id; };
struct CQ_HeroSoulsArenaScript { bool m_gateMayOpen; };
struct CQ_SunnyvaleMasterData { bool m_gateMayClose; };
struct CArenaHeroGate;
static char g_log[64]; static int g_n = 0;
struct CArenaHeroGate {
    CScriptThing Gate; CQ_HeroSoulsArenaScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    int ticks, abortAt;
    void CloseGate(const CScriptThing*) { g_log[g_n++] = 'C'; }
    void OpenGate(const CScriptThing*) { g_log[g_n++] = 'O'; }
    void Tick()
    {
        ++ticks; g_log[g_n++] = 't';
        if (ticks == 2) ParentClass->m_gateMayOpen = true;
        if (ticks == 4) PMasterData->m_gateMayClose = true;
    }
    bool IsAborted() { return abortAt != 0 && ticks >= abortAt; }
    void Main()
    {
        CloseGate(&Gate);
        while (!ParentClass->m_gateMayOpen) { Tick(); if (IsAborted()) return; }
        if (IsAborted()) return;
        OpenGate(&Gate);
        while (!PMasterData->m_gateMayClose) { Tick(); if (IsAborted()) return; }
        if (IsAborted()) return;
        CloseGate(&Gate);
    }
};

int main()
{
    CQ_HeroSoulsArenaScript p = { false }; CQ_SunnyvaleMasterData m = { false };
    CArenaHeroGate g = { { 1 }, &p, &m, 0, 0 };
    g.Main(); g_log[g_n] = 0;
    if (g_n != 7 || g_log[0] != 'C' || g_log[3] != 'O' || g_log[6] != 'C') { std::printf("00d7df30_TEST FAIL\n"); return 1; }
    p.m_gateMayOpen = false; m.m_gateMayClose = false; g_n = 0;
    CArenaHeroGate h = { { 1 }, &p, &m, 0, 1 };
    h.Main();
    if (g_n != 2 || h.ticks != 1) { std::printf("00d7df30_TEST FAIL\n"); return 1; }
    std::printf("00d7df30_TEST PASS\n");
    return 0;
}