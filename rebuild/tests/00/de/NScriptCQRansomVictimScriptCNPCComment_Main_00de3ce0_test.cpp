// Behaviour test for 0x00DE3CE0 NScript::CQ_RansomVictimScript::CNPCComment::Main(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("00de3ce0_TEST FAIL line %d\n", line); exit(1); } }
class CCharString { public: std::string Text; CCharString(const char* t) : Text(t) { } };
class CScriptThing { public:
    std::string Name; bool Alive; float Distance;
    CScriptThing() : Alive(false), Distance(0) { }
    bool IsAlive() const { return Alive; }
    CCharString GetDataString() const { return CCharString("data"); }
};
static CScriptThing g_guard, g_victim;
static int g_terminate_after = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_calls = 0;
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing& b, float d) { return b.Distance < d; }
class CGame { public:
    int Frames; bool Removed;
    void NewScriptFrame() { ++Frames; trace += '.'; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { return n.Text == "Victim" ? g_victim : g_guard; }
    void RemoveThing(const CScriptThing&, bool a, bool b) { check(!a && b, __LINE__); Removed = true; trace += 'X'; }
};
class CScriptDef { public: long NPC_COMMENT_DISTANCE; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;
enum ENPCComment { NPC_COMMENT_VICTIM, NPC_COMMENT_GUARD };
struct CQ_RansomVictimScript {
    int Accept;   // how many comments are accepted (the timer lets one through per call when 1)
    bool MakeNPCComment(const CCharString& name, const CScriptThing& npc, ENPCComment c)
    {
        check(name.Text == "data" && c == NPC_COMMENT_GUARD, __LINE__);
        trace += npc.Name == "guard" ? 'G' : 'V';
        if (Accept > 0) { --Accept; return true; }
        return false;
    }
};
struct CNPCComment {
    CGame& Game; CScriptThing Me; CQ_RansomVictimScript& ParentClass;
    bool VictimsFatherGuardSaidIt, VictimSaidIt;
    CNPCComment(CGame& g, CQ_RansomVictimScript& p) : Game(g), ParentClass(p), VictimsFatherGuardSaidIt(false), VictimSaidIt(false) { }
    bool IsActiveThreadTerminating() const { return ++g_calls > g_terminate_after; }
    void Main();
};
// the row's logic (src/00de3ce0.cpp; the alive predicate registration has no observable effect here)
void CNPCComment::Main()
{
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    long distance = pGlobalScriptDef->NPC_COMMENT_DISTANCE;
    CScriptThing guard = Game.GetThingWithScriptName("VictimsFatherGuard");
    CScriptThing victim = Game.GetThingWithScriptName("Victim");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!VictimsFatherGuardSaidIt && guard.IsAlive() && IsDistanceBetweenThingsUnder(Me, guard, (float)distance)) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass.MakeNPCComment(Me.GetDataString(), guard, NPC_COMMENT_GUARD)) {
                if (IsActiveThreadTerminating())
                    return;
                VictimsFatherGuardSaidIt = true;
            }
        }
        if (!VictimSaidIt && victim.IsAlive() && IsDistanceBetweenThingsUnder(Me, victim, (float)distance)) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass.MakeNPCComment(Me.GetDataString(), victim, NPC_COMMENT_GUARD)) {
                if (IsActiveThreadTerminating())
                    return;
                VictimSaidIt = true;
            }
        }
        if (VictimsFatherGuardSaidIt && VictimSaidIt) {
            if (IsActiveThreadTerminating())
                return;
            Game.RemoveThing(Me, false, true);
            return;
        }
        Game.NewScriptFrame();
    }
}
int main()
{
    g_def.NPC_COMMENT_DISTANCE = 10;
    g_guard.Name = "guard"; g_guard.Alive = true; g_guard.Distance = 5;
    g_victim.Name = "victim"; g_victim.Alive = true; g_victim.Distance = 20;   // too far at first
    CGame game = { 0, false };
    CQ_RansomVictimScript parent; parent.Accept = 1;
    CNPCComment script(game, parent);
    // guard speaks on the first frame; the victim never comes close: runs until terminated, never removed
    g_terminate_after = 12;
    script.Main();
    check(script.VictimsFatherGuardSaidIt && !script.VictimSaidIt && !game.Removed, __LINE__);
    check(trace.substr(0, 2) == ".G", __LINE__);
    // both close and both accepted on the first frame: Me is removed
    trace = ""; g_calls = 0; g_terminate_after = 1000; g_victim.Distance = 1;
    CGame game2 = { 0, false };
    CQ_RansomVictimScript parent2; parent2.Accept = 2;
    CNPCComment script2(game2, parent2);
    script2.Main();
    check(game2.Removed && trace == ".GVX", __LINE__);
    // every offer refused: both retried each frame until the thread terminates; never removed
    trace = ""; g_calls = 0; g_terminate_after = 9;
    CGame game3 = { 0, false };
    CQ_RansomVictimScript parent3; parent3.Accept = 0;
    CNPCComment script3(game3, parent3);
    script3.Main();
    check(!game3.Removed && !script3.VictimsFatherGuardSaidIt && !script3.VictimSaidIt, __LINE__);
    check(trace == ".GV.GV.G", __LINE__);
    printf("00de3ce0_TEST PASS\n");
    return 0;
}