// Behaviour test for CV_MurderTwistFinalScript::PlayBeingRumbledCutscene 0x00EAF1E0: a model of the cutscene's speech sequence
// (guard 0 -> sheriff, sheriff -> guard 0 twice, guard 0 -> sheriff, guard 1 -> guard 0, sheriff -> guard 0, guard 1 -> hero),
// each line spoken only while its speaker is alive, the pause held for the whole scene, and the three handles released at the end
// (an early terminate releases nothing and still unpauses).
#include <cstdio>
#include <cstring>
struct CLog { const char* lines[16]; int n; int pauses; bool terminate; };
struct CHandle {
    int id; bool alive; bool valid; CLog* log;
    void Speak(int to, const char* text) { (void)to; log->lines[log->n++] = text; }
    bool IsValid() const { return valid; }
    void Release() { valid = false; }
};
struct CPause { CLog* log; CPause(CLog* l) : log(l) { ++log->pauses; } ~CPause() { --log->pauses; } };
struct CScript {
    CLog log; CHandle sheriff, guard0, guard1;
    void Play()
    {
        CPause pause(&log);
        if (guard0.alive) { guard0.Speak(1, "TEXT_QST_B07_RUMBLED_10"); if (log.terminate) return; }
        if (sheriff.alive) { sheriff.Speak(2, "TEXT_QST_B07_RUMBLED_20"); if (log.terminate) return; }
        if (sheriff.alive) { sheriff.Speak(2, "TEXT_QST_B07_RUMBLED_30"); if (log.terminate) return; }
        if (guard0.alive) { guard0.Speak(1, "TEXT_QST_B07_RUMBLED_40"); if (log.terminate) return; }
        if (guard1.alive) { guard1.Speak(2, "TEXT_QST_B07_RUMBLED_50"); if (log.terminate) return; }
        if (sheriff.alive) { sheriff.Speak(2, "TEXT_QST_B07_RUMBLED_60"); if (log.terminate) return; }
        if (guard1.alive) { guard1.Speak(0, "TEXT_QST_B07_RUMBLED_70"); if (log.terminate) return; }
        if (guard0.IsValid()) guard0.Release();
        if (guard1.IsValid()) guard1.Release();
        if (sheriff.IsValid()) sheriff.Release();
    }
};
static void Init(CScript& s, bool g0, bool sh, bool g1)
{
    std::memset(&s, 0, sizeof(s));
    s.sheriff.alive = sh; s.guard0.alive = g0; s.guard1.alive = g1;
    s.sheriff.valid = s.guard0.valid = s.guard1.valid = true;
    s.sheriff.log = s.guard0.log = s.guard1.log = &s.log;
}
int main()
{
    int ok = 1;
    static CScript a; Init(a, true, true, true); a.Play();
    ok &= a.log.n == 7 && std::strcmp(a.log.lines[6], "TEXT_QST_B07_RUMBLED_70") == 0 && a.log.pauses == 0;
    ok &= !a.guard0.valid && !a.guard1.valid && !a.sheriff.valid;
    static CScript b; Init(b, false, true, false); b.Play();
    ok &= b.log.n == 3 && std::strcmp(b.log.lines[0], "TEXT_QST_B07_RUMBLED_20") == 0 && std::strcmp(b.log.lines[2], "TEXT_QST_B07_RUMBLED_60") == 0;
    static CScript c; Init(c, true, true, true); c.log.terminate = true; c.Play();
    ok &= c.log.n == 1 && c.log.pauses == 0 && c.guard0.valid && c.sheriff.valid;
    if (!ok) { std::printf("00eaf1e0_TEST FAIL\n"); return 1; }
    std::printf("00eaf1e0_TEST PASS\n");
    return 0;
}