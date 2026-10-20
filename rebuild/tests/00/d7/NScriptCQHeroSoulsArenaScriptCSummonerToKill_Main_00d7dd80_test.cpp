// Behaviour test for CQ_HeroSoulsArenaScript::CSummonerToKill::Main (0x00D7DD80): control flow over the parent flags.
#include <cstdio>

struct CScriptThing { long id; };
struct CQ_HeroSoulsArenaScript { bool PlayedArenaCS; bool FirstWaveAttack; };

struct CGame {
    int frames, started, startPriority, raiseWaveAt, doneAt, filters, scriptedAlive;
    CQ_HeroSoulsArenaScript* parent;
    void NewScriptFrame() { ++frames; if (frames == raiseWaveAt) parent->FirstWaveAttack = true; }
    bool StartScriptingEntity(const CScriptThing&, int priority) { ++started; startPriority = priority; return true; }
};
static CGame g;
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
struct CScripted { CScripted() { ++g.scriptedAlive; } ~CScripted() { --g.scriptedAlive; } };

struct CSummonerToKill {
    CGame* Game;
    CScriptThing Me;
    CQ_HeroSoulsArenaScript* ParentClass;
    void RegisterFilter(CIsThingAlive&) { ++Game->filters; }
    bool IsDone() { return Game->frames >= Game->doneAt; }
    void Main()
    {
        {
            CIsThingAlive filter(Me);
            RegisterFilter(filter);
        }
        Game->NewScriptFrame();
        if (IsDone())
            return;
        if (!ParentClass->PlayedArenaCS) {
            if (IsDone())
                return;
            CScripted scripted;
            Game->StartScriptingEntity(Me, 4);
            while (!ParentClass->FirstWaveAttack) {
                Game->NewScriptFrame();
                if (IsDone())
                    return;
            }
            if (IsDone())
                return;
        }
        do {
            Game->NewScriptFrame();
        } while (!IsDone());
    }
};

static int Run(bool played, int waveAt, int doneAt, int* frames, int* started)
{
    CQ_HeroSoulsArenaScript parent; parent.PlayedArenaCS = played; parent.FirstWaveAttack = false;
    g.frames = 0; g.started = 0; g.startPriority = 0; g.raiseWaveAt = waveAt; g.doneAt = doneAt; g.filters = 0;
    g.scriptedAlive = 0; g.parent = &parent;
    CSummonerToKill s; s.Game = &g; s.Me.id = 1; s.ParentClass = &parent;
    s.Main();
    *frames = g.frames; *started = g.started;
    return g.filters == 1 && g.scriptedAlive == 0;
}

int main()
{
    int ok = 1, frames, started;
    if (!Run(true, 0, 5, &frames, &started) || frames != 5 || started != 0) ok = 0;          // cut-scene played: plain frame loop
    if (!Run(false, 3, 6, &frames, &started) || frames != 6 || started != 1 || g.startPriority != 4) ok = 0;   // wait for the wave, then loop
    if (!Run(false, 99, 4, &frames, &started) || frames != 4 || started != 1) ok = 0;        // done while waiting
    if (!Run(false, 0, 1, &frames, &started) || frames != 1 || started != 0) ok = 0;         // done after the first frame
    if (!ok) { std::printf("00d7dd80_TEST FAIL\n"); return 1; }
    std::printf("00d7dd80_TEST PASS\n");
    return 0;
}