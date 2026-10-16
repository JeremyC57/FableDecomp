// Behaviour test for 00da4c90: CBalverineFightingGuard::OnInterrupted -- start a speech on Me, add the guard-attack line with no listener.
#include <cstdio>
#include <cstring>
struct TThing { const void* data; TThing() : data(0) {} };
struct TGame {
    long lastSpeech; const char* lastLine; const TThing* speaker; const void* listenerData; int calls;
    TGame() : lastSpeech(0), lastLine(0), speaker(0), listenerData((void*)1), calls(0) {}
    long StartSpeech(const TThing& s, long, long) { speaker = &s; return 42; }
    void AddSpeechLine(long speech, const char* line, long, const TThing& s, const TThing& listener)
    { lastSpeech = speech; lastLine = line; if (&s != speaker) speaker = 0; listenerData = listener.data; ++calls; }
};
struct TGuard {
    TGame* Game; TThing Me;
    void OnInterrupted() { long speech = Game->StartSpeech(Me, 0, 0); Game->AddSpeechLine(speech, "TEXT_QST_045_GUARD_ATTACK", 0, Me, TThing()); }
};
int main()
{
    TGame g; TGuard guard; guard.Game = &g; guard.OnInterrupted();
    if (g.calls != 1 || g.lastSpeech != 42 || g.speaker != &guard.Me || g.listenerData != 0 ||
        std::strcmp(g.lastLine, "TEXT_QST_045_GUARD_ATTACK") != 0) { std::printf("00da4c90_TEST FAIL\n"); return 1; }
    std::printf("00da4c90_TEST PASS\n");
    return 0;
}