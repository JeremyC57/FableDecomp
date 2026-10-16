#include <cstdio>
struct CScriptThing { int id; };
struct CGSI {
    int log[8]; int n; int pushable; int info; int deeds;
    void Damageable(const CScriptThing&, bool) { log[n++] = 516; }
    void Killable(const CScriptThing&, bool, bool) { log[n++] = 517; }
    void Combo(const CScriptThing&, bool) { log[n++] = 526; }
    void Info(const CScriptThing&, bool a, bool b, bool c) { log[n++] = 360; info = a * 4 + b * 2 + c; }
    void Pushable(CScriptThing, bool p) { log[n++] = 844; pushable = p; }
    void Movement(const CScriptThing&, bool) { log[n++] = 529; }
    void Deeds(const CScriptThing&, bool e) { log[n++] = 595; deeds = e; }
};
struct CNOVI_AffairWife {
    CGSI* GSI; CScriptThing Me; bool GoingForHusband; bool ForceFirstTimeSpeak; bool SaidRunningLine;
    void Init() { GoingForHusband = false; ForceFirstTimeSpeak = true; GSI->Damageable(Me, false); GSI->Killable(Me, false, false); GSI->Combo(Me, false); GSI->Info(Me, false, true, false); GSI->Movement(Me, false); GSI->Pushable(Me, false); SaidRunningLine = false; GSI->Deeds(Me, false); }
};
int main()
{
    CGSI g; g.n = 0; g.pushable = 1; g.info = 0; g.deeds = 1;
    CNOVI_AffairWife w; w.GSI = &g; w.Me.id = 1; w.GoingForHusband = true; w.ForceFirstTimeSpeak = false; w.SaidRunningLine = true;
    w.Init();
    static const int want[7] = { 516, 517, 526, 360, 529, 844, 595 };
    for (int i = 0; i < 7; ++i) if (g.log[i] != want[i]) { std::printf("00db2a70_TEST FAIL\n"); return 1; }
    if (g.n != 7 || w.GoingForHusband || !w.ForceFirstTimeSpeak || w.SaidRunningLine || g.pushable || g.deeds || g.info != 2) { std::printf("00db2a70_TEST FAIL\n"); return 1; }
    std::printf("00db2a70_TEST PASS\n");
    return 0;
}
