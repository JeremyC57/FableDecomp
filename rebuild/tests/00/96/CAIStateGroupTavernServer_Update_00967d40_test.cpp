// Behaviour test for 0x00967D40 CAIStateGroup_TavernServer::Update: the row's body over scripted stubs (each member records
// its call in a trace; results come from scripts), checking the order of the work loop and where it terminates or returns.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
static void check(bool b) { if (!b) { std::puts("00967d40_TEST FAIL"); std::exit(1); } }
enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_FINISHED = 2 };
enum EMovementType { AI_MOVEMENT_WALKING = 3 };
static std::string g_trace;
static const char* g_script; static int g_pos;
static bool next(char tag) { g_trace += tag; char c = g_script[g_pos]; check(c != 0); ++g_pos; return c == '1'; }
class CThing { public: bool dead; bool IsDead() const { return dead; } };
class CTCVillageMember { public: void* GetPVillage() const { g_trace += 'V'; return 0; } };
class CTCTavernJug { public: long Fullness; };
class CTCTavernTable { public: long freed; void FreeServingPoint(long i) { g_trace += 'F'; freed = i; } };
class CJugThing : public CThing { public: CTCTavernJug jug; bool has; bool GetTC(CTCTavernJug** p) { if (!has) return false; *p = &jug; return true; } };
class CTableThing : public CThing { public: CTCTavernTable table; bool GetTC(CTCTavernTable** p) { *p = &table; return true; } };
template<class T> class CIntelligentPointer { public: T* p; T* Get() const { return p; } T* operator->() const { return p; } };
template<class T> bool operator!=(const CIntelligentPointer<T>& a, int) { return a.Get() != 0; }
template<class T> bool IS_THING_ALIVE(const T& t) { if (t != 0) { if (!t->IsDead()) return true; } return false; }
class CCreature { public: CTCVillageMember member; bool GetTC(CTCVillageMember** p) { *p = &member; return true; } };
struct CAIStateGroup_TavernServer {
    CCreature creature; CIntelligentPointer<CJugThing> PJug; CIntelligentPointer<CTableThing> PTable; CIntelligentPointer<CThing> PPub;
    long ServingActionPointIndex; bool terminated; int pours;
    CCreature& GetCreature() { return creature; }
    bool GetServingPoint() { bool r = next('S'); if (r) ServingActionPointIndex = 5; return r; }
    bool IsCarryingJug() { return next('C'); }
    bool GetJug() { return next('J'); }
    bool WalkToPoint() { return next('W'); }
    bool PickUpJug() { return next('P'); }
    bool FillTankard() { bool r = next('T'); if (r && --pours == 0) PJug.p->jug.Fullness = 0; return r; }
    bool GetOldJugPoint() { return next('O'); }
    bool PutDownJug() { return next('D'); }
    bool IsStopping() { return next('?'); }
    EAIActionReturn AIAction_DoNothing() { g_trace += 'N'; return AI_ACTION_FINISHED; }
    EAIActionReturn AIAction_MinionWander(EMovementType m, CThing* b) { check(m == AI_MOVEMENT_WALKING && b == PPub.Get()); g_trace += 'R'; return AI_ACTION_FINISHED; }
    void Terminate() { g_trace += 'X'; terminated = true; }
    EAIActionReturn WanderWithinTavern() { return AIAction_MinionWander(AI_MOVEMENT_WALKING, PPub.Get()); }
    void FreeServingPoint() {
        if (ServingActionPointIndex != -1) {
            CTCTavernTable* table; PTable->GetTC(&table);
            table->FreeServingPoint(ServingActionPointIndex); ServingActionPointIndex = -1;
        }
    }
    bool WalkToBar() { return WalkToPoint(); }
    bool WalkToServingPoint() { return WalkToPoint(); }
    void Update();
};
void CAIStateGroup_TavernServer::Update()
{
    CTCVillageMember* village_member;
    GetCreature().GetTC(&village_member);
    village_member->GetPVillage();
    for (;;) {
        if (ServingActionPointIndex == -1 && !GetServingPoint()) break;
        if (!IsCarryingJug()) {
            if (!GetJug()) break;
            if (!WalkToBar()) break;
            if (!PickUpJug()) break;
        }
        CTCTavernJug* jug;
        if (!IS_THING_ALIVE(PJug) || !PJug->GetTC(&jug)) break;
        while (jug->Fullness > 0) {
            if (ServingActionPointIndex == -1 && !GetServingPoint()) break;
            if (IsStopping()) return;
            AIAction_DoNothing();
            if (!WalkToServingPoint()) FreeServingPoint();
            else if (!FillTankard()) FreeServingPoint();
        }
        while (!GetOldJugPoint()) { WanderWithinTavern(); if (IsStopping()) return; }
        while (!WalkToBar()) { WanderWithinTavern(); if (IsStopping()) return; }
        PutDownJug();
    }
    Terminate();
}
static std::string run(const char* script, long index, bool jugAlive, bool hasJug, long fullness, int pours)
{
    static CJugThing jug; static CTableThing table; static CThing pub;
    jug.dead = !jugAlive; jug.has = hasJug; jug.jug.Fullness = fullness; table.table.freed = 0;
    CAIStateGroup_TavernServer s; s.PJug.p = &jug; s.PTable.p = &table; s.PPub.p = &pub;
    s.ServingActionPointIndex = index; s.terminated = false; s.pours = pours;
    g_trace = ""; g_script = script; g_pos = 0;
    s.Update();
    check(g_script[g_pos] == 0);
    return g_trace;
}
int main()
{
    // no serving point: terminate at once
    check(run("0", -1, true, true, 1, 1) == "VSX");
    // carrying; point known; pour twice (walk fails once -> point freed and re-found), jug empties; old point, bar, put down; then no point
    check(run("1" "0" "0" "1" "0" "1" "1" "0" "1" "1" "1" "1" "1" "0" "0", 7, true, true, 1, 2) ==
          "V" "C" "?NWF" "S?NWT" "?NWT" "O" "W" "D" "C" "J" "X");
    // fetch a jug: get, walk to bar, pick up; dead jug -> terminate
    check(run("0" "1" "1" "1", 3, false, true, 1, 1) == "VCJWPX");
    // jug without the jug component -> terminate
    check(run("1", 3, true, false, 1, 1) == "VCX");
    // stopping while waiting for the old jug point: return without Terminate
    check(run("1" "0" "1", 3, true, true, 0, 1) == "VCOR?");
    // stopping inside the pour loop
    check(run("1" "1", 3, true, true, 2, 1) == "VC?");
    std::puts("00967d40_TEST PASS");
    return 0;
}