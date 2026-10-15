// Behaviour test for CV_AmbushScamScript::ResetRandomSpeechTime (0x00E1FEB0).
#include <cstdio>

static int g_rand = 0;
static int fake_rand() { return g_rand; }

struct CGameScriptInterface {
    long m_timer, m_time;
    virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { }
    virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void v014() { } virtual void v015() { }
    virtual void v016() { } virtual void v017() { } virtual void v018() { } virtual void v019() { } virtual void v020() { } virtual void v021() { } virtual void v022() { } virtual void v023() { }
    virtual void v024() { } virtual void v025() { } virtual void v026() { } virtual void v027() { } virtual void v028() { } virtual void v029() { } virtual void v030() { } virtual void v031() { }
    virtual void v032() { } virtual void v033() { } virtual void v034() { } virtual void v035() { } virtual void v036() { } virtual void v037() { } virtual void v038() { } virtual void v039() { }
    virtual void v040() { } virtual void v041() { } virtual void v042() { } virtual void v043() { } virtual void v044() { } virtual void v045() { } virtual void v046() { } virtual void v047() { }
    virtual void v048() { } virtual void v049() { } virtual void v050() { } virtual void v051() { } virtual void v052() { } virtual void v053() { } virtual void v054() { } virtual void v055() { }
    virtual void v056() { } virtual void v057() { } virtual void v058() { } virtual void v059() { } virtual void v060() { } virtual void v061() { } virtual void v062() { } virtual void v063() { }
    virtual void v064() { } virtual void v065() { } virtual void v066() { } virtual void v067() { } virtual void v068() { } virtual void v069() { } virtual void v070() { } virtual void v071() { }
    virtual void v072() { } virtual void v073() { } virtual void v074() { } virtual void v075() { } virtual void v076() { } virtual void v077() { } virtual void v078() { } virtual void v079() { }
    virtual void v080() { } virtual void v081() { } virtual void v082() { } virtual void v083() { } virtual void v084() { } virtual void v085() { } virtual void v086() { } virtual void v087() { }
    virtual void v088() { }
    virtual void SetTimer(long timer, long time) { m_timer = timer; m_time = time; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

struct CScriptDef { long RandomSpeechTimeMax; long RandomSpeechTimeRange; };
CScriptDef* g_pScriptDef = 0;

struct CTimer { long Index; };

struct CV_AmbushScamScript {
    CTimer RandomSpeechTimer;
    void ResetRandomSpeechTime();
};

void CV_AmbushScamScript::ResetRandomSpeechTime()
{
    CScriptDef* def = g_pScriptDef;
    long time = g_pScriptDef->RandomSpeechTimeMax - fake_rand() % def->RandomSpeechTimeRange;
    g_pGameScriptInterface->SetTimer(RandomSpeechTimer.Index, time);
}

int main()
{
    CGameScriptInterface gsi; gsi.m_timer = gsi.m_time = 0;
    CScriptDef def; def.RandomSpeechTimeMax = 600; def.RandomSpeechTimeRange = 100;
    g_pGameScriptInterface = &gsi; g_pScriptDef = &def;
    CV_AmbushScamScript s; s.RandomSpeechTimer.Index = 7;
    g_rand = 1234;
    s.ResetRandomSpeechTime();
    if (gsi.m_timer != 7 || gsi.m_time != 566) { std::printf("00e1feb0_TEST FAIL\n"); return 1; }
    g_rand = 99;
    s.ResetRandomSpeechTime();
    if (gsi.m_time != 501) { std::printf("00e1feb0_TEST FAIL\n"); return 1; }
    std::printf("00e1feb0_TEST PASS\n");
    return 0;
}