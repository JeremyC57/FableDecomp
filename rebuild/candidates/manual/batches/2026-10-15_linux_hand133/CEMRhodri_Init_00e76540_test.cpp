// Behaviour test for CEM_Rhodri::Init @ 0x00E76540
#include <cstdio>
struct CBaseClass { virtual void v00() {} };
struct CCountedPointerWords {
    void* Data; long* Info;
    CCountedPointerWords() : Data(0), Info(0) {}
    CCountedPointerWords(const CCountedPointerWords& other) : Data(other.Data), Info(other.Info) { if (Info) ++*Info; }
};
struct CScriptThing : CBaseClass { CCountedPointerWords PImp; };
static int g_log[8]; static int g_n = 0; static long g_countSeen = 0;
struct CGameScriptInterface {
    virtual void SetThingFlags(const CScriptThing&, long a, long b, long c) { g_log[g_n++] = 360 + a * 1000 + b + c; }
    virtual void SetThingAsTarget(CScriptThing thing, long) { g_countSeen = *thing.PImp.Info; g_log[g_n++] = 844; }
    virtual void Slot745(const CScriptThing&, long) { g_log[g_n++] = 745; }
    virtual void Slot517(const CScriptThing&, long, long) { g_log[g_n++] = 517; }
    virtual void Slot526(const CScriptThing&, long) { g_log[g_n++] = 526; }
    virtual void Slot516(const CScriptThing&, long) { g_log[g_n++] = 516; }
};
class CEM_Rhodri {
public:
    CGameScriptInterface* Game; CScriptThing Me; void* ParentClass; void* PMasterData;
    bool ExplainedPlight, m_flag1d, m_flag1e, m_flag1f;
    void Init()
    {
        ExplainedPlight = false; m_flag1d = false; m_flag1e = false;
        Game->SetThingFlags(Me, 1, 0, 0);
        Game->SetThingAsTarget(Me, 0);
        Game->Slot745(Me, 0);
        Game->Slot517(Me, 0, 0);
        Game->Slot526(Me, 0);
        Game->Slot516(Me, 0);
        m_flag1f = false;
    }
};
int main()
{
    CGameScriptInterface game;
    long count = 1;
    CEM_Rhodri r; r.Game = &game; r.Me.PImp.Info = &count;
    r.ExplainedPlight = r.m_flag1d = r.m_flag1e = r.m_flag1f = true;
    r.Init();
    if (r.ExplainedPlight || r.m_flag1d || r.m_flag1e || r.m_flag1f || g_n != 6 || g_log[0] != 1360 || g_log[1] != 844 ||
        g_log[2] != 745 || g_log[3] != 517 || g_log[4] != 526 || g_log[5] != 516 || g_countSeen != 2) {
        std::printf("00e76540_TEST FAIL\n"); return 1;
    }
    std::printf("00e76540_TEST PASS\n");
    return 0;
}
