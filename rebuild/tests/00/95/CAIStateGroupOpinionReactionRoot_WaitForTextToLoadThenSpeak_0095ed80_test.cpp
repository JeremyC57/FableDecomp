#include <cstdio>
class CTCTalk { public: const char* script; int plays; unsigned long text; long layer; bool allow, a, b;
    long PlayAISpeech(unsigned long t, bool al, long l, bool x, bool y) { ++plays; text = t; allow = al; layer = l; a = x; b = y; return *script++ - '0'; } };
struct CGroup { CTCTalk* talk; bool AllowSpeech; const char* waits; int waitCalls; bool terminated;
    long AIAction_DoNothing() { ++waitCalls; return *waits++ - '0'; }
    void Terminate() { terminated = true; }
    long Wait(unsigned long text, long layer) {
        long wait;
        do { long r = talk->PlayAISpeech(text, AllowSpeech, layer, false, true); if (r != 2) return r == 0 ? 2 : 3; wait = AIAction_DoNothing(); } while (wait == 4);
        Terminate(); return wait; } };
int main()
{
    CTCTalk t; CGroup g; g.talk = &t; g.AllowSpeech = true; bool ok = true;
    t.script = "220"; t.plays = 0; g.waits = "44"; g.waitCalls = 0; g.terminated = false;
    ok = ok && g.Wait(55, 3) == 2 && t.plays == 3 && g.waitCalls == 2 && !g.terminated && t.text == 55 && t.layer == 3 && t.allow && !t.a && t.b;
    t.script = "1"; t.plays = 0; g.waitCalls = 0; ok = ok && g.Wait(1, 1) == 3 && t.plays == 1 && g.waitCalls == 0;
    t.script = "22"; t.plays = 0; g.waits = "45"; g.waitCalls = 0; ok = ok && g.Wait(1, 1) == 5 && g.terminated && t.plays == 2 && g.waitCalls == 2;
    if (!ok) { std::printf("0095ed80_TEST FAIL\n"); return 1; }
    std::printf("0095ed80_TEST PASS\n"); return 0;
}