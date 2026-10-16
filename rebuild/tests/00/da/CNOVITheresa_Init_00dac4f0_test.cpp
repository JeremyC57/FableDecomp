#include <cstdio>
struct CScriptThing { int id; };
struct CGSI {
    int log[8]; int n; int pushable; int info;
    void Damageable(const CScriptThing&, bool) { log[n++] = 516; }
    void Killable(const CScriptThing&, bool, bool) { log[n++] = 517; }
    void Combo(const CScriptThing&, bool) { log[n++] = 526; }
    void Info(const CScriptThing&, bool a, bool b, bool c) { log[n++] = 360; info = a * 4 + b * 2 + c; }
    void Pushable(CScriptThing, bool p) { log[n++] = 844; pushable = p; }
    void Movement(const CScriptThing&, bool) { log[n++] = 529; }
};
struct CNOVI_Theresa {
    CGSI* GSI; CScriptThing Me; bool spoken_already; bool DoneIntro;
    void Init() { DoneIntro = false; spoken_already = false; GSI->Damageable(Me, false); GSI->Killable(Me, false, false); GSI->Combo(Me, false); GSI->Info(Me, false, true, false); GSI->Pushable(Me, false); GSI->Movement(Me, false); }
};
int main()
{
    CGSI g; g.n = 0; g.pushable = 1; g.info = 0;
    CNOVI_Theresa t; t.GSI = &g; t.Me.id = 1; t.spoken_already = true; t.DoneIntro = true;
    t.Init();
    static const int want[6] = { 516, 517, 526, 360, 844, 529 };
    for (int i = 0; i < 6; ++i) if (g.log[i] != want[i]) { std::printf("00dac4f0_TEST FAIL\n"); return 1; }
    if (g.n != 6 || t.spoken_already || t.DoneIntro || g.pushable || g.info != 2) { std::printf("00dac4f0_TEST FAIL\n"); return 1; }
    std::printf("00dac4f0_TEST PASS\n");
    return 0;
}