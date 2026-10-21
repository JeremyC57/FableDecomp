// Behaviour test for CGuildSealTrigger::Main @ 0x00EFBE20 (copy of the model with a scripted stub world).
#include <cstdio>
#include <cstring>
static int g_frame, g_doneAt, g_filters, g_on, g_off, g_pauses;
static bool g_hasSeal, g_triggerActive;
struct CScriptThing { int id; void SetActivationTriggerStatus(bool a) { g_triggerActive = a; if (a) ++g_on; else ++g_off; } };
struct CIsThingConscious { CScriptThing m_thing; CIsThingConscious(const CScriptThing& t) : m_thing(t) {} };
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 2.0f && (g_frame % 3) == 0; }
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frame; }
    const CScriptThing& GetHero() const { return hero; }
    bool IsObjectInThingsPossession(const char* obj, const CScriptThing& t) const { return g_hasSeal && t.id == 1 && std::strcmp(obj, "OBJECT_GUILD_SEAL_1") == 0; }
    void Pause(float s) const { if (s == 10.0f && g_triggerActive) ++g_pauses; ++g_frame; }
};
struct CGuildSealTrigger {
    CGameScriptInterface* Game; CScriptThing Me;
    void RegisterFilter(CIsThingConscious& f) { if (f.m_thing.id == Me.id) ++g_filters; }
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main()
    {
        { CIsThingConscious filter(Me); RegisterFilter(filter); }
        Game->NewScriptFrame();
        if (IsDone()) return;
        while (!IsDone()) {
            while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
            if (IsDone()) return;
            if (Game->IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", Game->GetHero())) {
                if (IsDone()) return;
                Me.SetActivationTriggerStatus(true);
                Game->Pause(10.0f);
                Me.SetActivationTriggerStatus(false);
            }
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    int bad = 0;
    CGameScriptInterface game; game.hero.id = 1; CGuildSealTrigger s; s.Game = &game; s.Me.id = 2;
    g_frame = 0; g_doneAt = 30; g_filters = g_on = g_off = g_pauses = 0; g_hasSeal = true; g_triggerActive = false;
    s.Main(); if (g_filters != 1 || g_on == 0 || g_on != g_off || g_pauses != g_on || g_triggerActive) bad++;
    g_frame = 0; g_doneAt = 30; g_filters = g_on = g_off = g_pauses = 0; g_hasSeal = false;
    s.Main(); if (g_filters != 1 || g_on != 0) bad++;
    g_frame = 0; g_doneAt = 1; g_filters = 0; s.Main(); if (g_filters != 1 || g_frame != 1) bad++;
    if (bad) { std::printf("00efbe20_TEST FAIL\n"); return 1; }
    std::printf("00efbe20_TEST PASS\n");
    return 0;
}
