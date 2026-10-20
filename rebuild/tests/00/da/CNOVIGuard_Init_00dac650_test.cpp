// Behaviour test for 0x00DAC650 CNOVI_Guard::Init: the guard is made undamageable/unkillable/not combo-counted, wander
// centred on his own position with 0..6 range, scripting state group 4 and weapons kept out, in that order; the
// by-value thing copies add-ref the shared imp.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CImp { long Count; C3DVector Pos; };
struct CScriptThing {
    CImp* PImp;
    CScriptThing(CImp* p) : PImp(p) { ++PImp->Count; }
    CScriptThing(const CScriptThing& o) : PImp(o.PImp) { ++PImp->Count; }
    ~CScriptThing() { --PImp->Count; }
    C3DVector GetHomePos() const { return PImp->Pos; }
};
static int g_log[16]; static int g_n; static long g_maxCount; static C3DVector g_centre; static float g_min, g_max; static long g_group;
static void Log(int id, const CScriptThing& t) { g_log[g_n++] = id; if (t.PImp->Count > g_maxCount) g_maxCount = t.PImp->Count; }
struct CGameScriptInterface {
    void EntitySetAsDamageable(const CScriptThing& t, bool d) const { Log(d ? 100 : 516, t); }
    void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { Log(k || b ? 100 : 517, t); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& t, bool a) const { Log(a ? 100 : 526, t); }
    void SetWanderCentrePoint(CScriptThing t, C3DVector c) { Log(763, t); g_centre = c; }
    void SetWanderMinDistance(CScriptThing t, float d) { Log(766, t); g_min = d; }
    void SetWanderMaxDistance(CScriptThing t, float d) { Log(769, t); g_max = d; }
    void SetScriptingStateGroup(CScriptThing t, long g) { Log(784, t); g_group = g; }
    void EntitySheatheWeapons(const CScriptThing& t, bool s) const { Log(s ? 100 : 506, t); }
};
struct CNOVI_Guard {
    CGameScriptInterface* GSI; CScriptThing Me;
    CNOVI_Guard(CGameScriptInterface* g, CImp* imp) : GSI(g), Me(imp) {}
    void Init()
    {
        GSI->EntitySetAsDamageable(Me, false);
        GSI->EntitySetAsKillable(Me, false, false);
        GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
        GSI->SetWanderCentrePoint(Me, Me.GetHomePos());
        GSI->SetWanderMinDistance(Me, 0.0f);
        GSI->SetWanderMaxDistance(Me, 6.0f);
        GSI->SetScriptingStateGroup(Me, 4);
        GSI->EntitySheatheWeapons(Me, false);
    }
};
int main()
{
    CImp imp = { 0, { 1.0f, 2.0f, 3.0f } };
    CGameScriptInterface gsi;
    CNOVI_Guard guard(&gsi, &imp);
    guard.Init();
    static const int expect[8] = { 516, 517, 526, 763, 766, 769, 784, 506 };
    if (g_n != 8) { std::printf("00dac650_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (g_log[i] != expect[i]) { std::printf("00dac650_TEST FAIL\n"); return 1; }
    if (g_centre.X != 1.0f || g_centre.Y != 2.0f || g_centre.Z != 3.0f || g_min != 0.0f || g_max != 6.0f || g_group != 4) { std::printf("00dac650_TEST FAIL\n"); return 1; }
    if (imp.Count != 1 || g_maxCount != 2) { std::printf("00dac650_TEST FAIL\n"); return 1; }
    std::printf("00dac650_TEST PASS\n");
    return 0;
}