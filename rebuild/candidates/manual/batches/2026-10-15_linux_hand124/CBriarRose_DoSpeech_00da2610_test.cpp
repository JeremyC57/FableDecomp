#include <cstdio>
struct CCharString { const char* text; };
struct CScriptThing { int id; };
static int g_seq[8]; static int g_n = 0; static const CCharString* g_line = 0; static long g_listener = 0;
struct CGameScriptInterface {
    virtual ~CGameScriptInterface() { }
    virtual long GetHero() { g_seq[g_n++] = 70; return 55; }
    virtual long StartSpeech(const CScriptThing& s, long a, long b) { g_seq[g_n++] = 364; return 9 + s.id + a + b; }
    virtual void SetSpeechListener(long speech, long listener) { g_seq[g_n++] = 365; g_listener = listener + speech; }
    virtual void AddSpeechLine(long speech, const CCharString& line, long a, const CScriptThing&, long listener) { g_seq[g_n++] = 366; g_line = &line; g_listener += listener + speech + a; }
};
struct CTimerManager { long m_remaining, m_set; long GetTimer(long) { return m_remaining; } void SetTimer(long, long t) { m_set = t; } };
CTimerManager g_tm; CTimerManager* g_pScriptTimerManager = &g_tm;
struct CTimer { long TimerIndex; };
struct CBriarRose {
    CGameScriptInterface* Game; CScriptThing Me; CTimer SpeechTimer; bool m_done;
    bool IsDone() { return m_done; }
    void DoSpeech(const CCharString& line)
    {
        if (g_pScriptTimerManager->GetTimer(SpeechTimer.TimerIndex) > 0) return;
        if (IsDone()) return;
        long speech = Game->StartSpeech(Me, 0, 0);
        Game->SetSpeechListener(speech, Game->GetHero());
        Game->AddSpeechLine(speech, line, 0, Me, Game->GetHero());
        g_pScriptTimerManager->SetTimer(SpeechTimer.TimerIndex, 3);
    }
};
int main()
{
    CGameScriptInterface g; CBriarRose b; b.Game = &g; b.Me.id = 1; b.SpeechTimer.TimerIndex = 4; b.m_done = false;
    CCharString line; line.text = "hi";
    g_tm.m_remaining = 2; g_tm.m_set = -1; b.DoSpeech(line);
    if (g_n != 0 || g_tm.m_set != -1) { std::printf("00da2610_TEST FAIL\n"); return 1; }
    g_tm.m_remaining = 0; b.m_done = true; b.DoSpeech(line);
    if (g_n != 0) { std::printf("00da2610_TEST FAIL\n"); return 1; }
    b.m_done = false; b.DoSpeech(line);
    if (g_n != 5 || g_seq[0] != 364 || g_seq[1] != 70 || g_seq[2] != 365 || g_seq[3] != 70 || g_seq[4] != 366 ||
        g_line != &line || g_listener != (55 + 10) + 55 + 10 || g_tm.m_set != 3)
    { std::printf("00da2610_TEST FAIL\n"); return 1; }
    std::printf("00da2610_TEST PASS\n");
    return 0;
}
