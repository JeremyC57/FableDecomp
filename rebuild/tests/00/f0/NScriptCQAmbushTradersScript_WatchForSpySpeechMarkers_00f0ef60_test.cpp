#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
// behaviour test: CQ_AmbushTradersScript::WatchForSpySpeechMarkers -- each time EnableSpeechSpy is raised the spy says its
// next line (sentry-seen / proximity / near the marker), the first five in order then random ones (proximity and normal
// random lines never repeat the last index), restarting the speech timer; the watch ends when the spy dies
static int g_frames = 0, g_spyDiesAt = 1000, g_randSeq = 0;
static std::vector<std::string> g_lines;
static long g_timer = -1;
static bool g_near = true;
static bool* g_enable = 0;
static int test_rand() { static const int seq[] = { 3, 3, 1, 4, 4, 2 }; return seq[g_randSeq++ % 6]; }
struct CScriptThing { bool spy; bool IsAlive() const { return !spy || g_frames < g_spyDiesAt; } };
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; if (g_frames % 2 == 1) *g_enable = true; }   // the timer thread raises the flag
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const char*) const { CScriptThing m; m.spy = false; return m; }
    long AddNewConversation(const CScriptThing&, bool, bool) { return 7; }
    void AddPersonToConversation(long, const CScriptThing&) {}
    void AddLineToConversation(long c, const std::string& text, bool, const CScriptThing&, const CScriptThing&) { if (c == 7) g_lines.push_back(text); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) { return g_near; }
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game;
    bool SpySeenBySentry, SpyHeroProximity, SpyContactMade, EnableSpeechSpy;
    long SpeechSpyCount, SpeechSpyProximityCount, SpeechSpySentryCount, lastRandomSpySpeechIdx;
    std::string SpySpeechNormal[5], SpySpeechRandom[5], SpySpeechProximityNormal[5], SpySpeechProximityRandom[5];
    std::string SpySpeechSentryNormal[5], SpySpeechSentryRandom[5];
    bool IsActiveThreadTerminating() { return false; }
    void WatchForSpySpeechMarkers(CScriptThing spy);
};
void CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy)
{
    std::string speech;
    for (;;) {
        while (!EnableSpeechSpy && spy.IsAlive()) Game->NewScriptFrame();
        if (!spy.IsAlive()) break;
        CScriptThing marker = Game->GetNearestWithScriptName(spy, "M_AT_EntitySpeech");
        if (SpySeenBySentry && !SpyContactMade && !SpyHeroProximity) {
            g_timer = 0xa58;
            if (SpeechSpySentryCount < 5) { speech = SpySpeechSentryNormal[SpeechSpySentryCount];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); SpeechSpySentryCount++; } }
            else { speech = SpySpeechSentryRandom[test_rand() % 5];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); } }
        } else if (SpyHeroProximity && !SpyContactMade) {
            g_timer = 0xa4c;
            if (SpeechSpyProximityCount < 5) { speech = SpySpeechProximityNormal[SpeechSpyProximityCount];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); SpeechSpyProximityCount++; } }
            else { long index; for (;;) { index = test_rand() % 5; if (index != lastRandomSpySpeechIdx) break; Game->NewScriptFrame(); }
                lastRandomSpySpeechIdx = index; speech = SpySpeechProximityRandom[index];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); } }
        } else if (IsDistanceBetweenThingsUnder(spy, marker, 5.0f)) {
            if (SpeechSpyCount < 5) { speech = SpySpeechNormal[SpeechSpyCount];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); SpeechSpyCount++; } }
            else { long index; for (;;) { index = test_rand() % 5; if (index != lastRandomSpySpeechIdx) break; Game->NewScriptFrame(); }
                lastRandomSpySpeechIdx = index; speech = SpySpeechRandom[index];
                if (spy.IsAlive()) { long c = Game->AddNewConversation(spy, false, false); Game->AddPersonToConversation(c, Game->GetHero());
                    Game->AddLineToConversation(c, speech, false, spy, Game->GetHero()); } }
            g_timer = 0xa50;
        } else g_timer = 0xa54;
        EnableSpeechSpy = false;
        Game->NewScriptFrame();
    }
}
int main()
{
    CGameScriptInterface game; game.hero.spy = false;
    CQ_AmbushTradersScript s; s.Game = &game; g_enable = &s.EnableSpeechSpy;
    s.SpySeenBySentry = false; s.SpyHeroProximity = false; s.SpyContactMade = false; s.EnableSpeechSpy = true;
    s.SpeechSpyCount = 0; s.SpeechSpyProximityCount = 0; s.SpeechSpySentryCount = 0; s.lastRandomSpySpeechIdx = 3;
    for (int i = 0; i < 5; ++i) { char b[8]; std::sprintf(b, "N%d", i); s.SpySpeechNormal[i] = b; std::sprintf(b, "R%d", i); s.SpySpeechRandom[i] = b; }
    g_spyDiesAt = 7;
    CScriptThing spy; spy.spy = true;
    s.WatchForSpySpeechMarkers(spy);
    // lines at frames 0,1,3,5 -> N0..N3; the spy is dead at frame 7
    bool ok = g_lines.size() == 4 && g_lines[0] == "N0" && g_lines[3] == "N3" && s.SpeechSpyCount == 4 && g_timer == 0xa50;
    g_lines.clear(); g_frames = 0; g_spyDiesAt = 1000; s.SpeechSpyCount = 5; s.EnableSpeechSpy = true;
    g_spyDiesAt = 4;
    s.WatchForSpySpeechMarkers(spy);
    // the random pick skips 3 (the last index): rand 3,3 repeat, then 1 -> R1
    ok = ok && g_lines.size() == 2 && g_lines[0] == "R1" && g_lines[1] == "R4" && s.lastRandomSpySpeechIdx == 4;
    if (!ok) { std::printf("00f0ef60_TEST FAIL\n"); return 1; }
    std::printf("00f0ef60_TEST PASS\n");
    return 0;
}