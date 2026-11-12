// Behaviour test for 0x00940960 CAIStateGroup_VillageTaskHorn::Update(): the row's body copied over test doubles (task pointer,
// village, horn, move / use actions); checks which calls each path makes and that Terminate runs exactly once.
#include <cstdio>
#include <cstring>
static int g_log[128]; static int g_n = 0;
static void Log(int v) { if (g_n < 128) g_log[g_n++] = v; }
enum EAIActionReturn { AI_ACTION_FAILED = 0, AI_ACTION_FINISHED = 2, AI_ACTION_IN_PROGRESS = 4 };
enum EAIActionRepeat { ACTION_REPEAT_CALLBACK = 2 };
enum EMovementType { AI_MOVEMENT_WALKING = 3 };
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCharString { public: const char* s; };
static const CCharString ACTION_POINT = { "ACTION_POINT" };
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
class CThing;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& n, long, unsigned long, C3DVector& p, CRightHandedSet&, float s) { Log(std::strcmp(n.s, "ACTION_POINT") == 0 && s == 1.0f ? 10 : 99); p.X = p.Y = p.Z = 1.0f; return true; }
};
class CThing { public:
    CTCGraphicAppearance app;
    const C3DVector& GetPos() const { static C3DVector v; Log(11); return v; }
    CTCGraphicAppearance* GetTCGraphicAppearance() { return &app; }
};
static int g_assignedFails = 1000;
class CVillageTaskHorn { public:
    CThing* horn;
    CThing* GetHorn() const { return horn; }
    bool IsCreatureAssignedToTask(const CThing&) const { Log(12); return --g_assignedFails > 0; }
};
struct CTaskPointer {
    CVillageTaskHorn* p;
    bool operator!=(int) const { return p != 0; }
    bool operator==(int) const { return p == 0; }
    CVillageTaskHorn* operator->() const { return p; }
    CVillageTaskHorn& operator*() const { return *p; }
};
static bool g_accept = true;
class CTCVillage { public:
    bool SetHornTaskAsAcceptedByThing(CVillageTaskHorn&, const CThing&) { Log(13); return g_accept; }
    void SetHornTaskAsCompleted(const CTaskPointer& t) { Log(t.p ? 14 : 99); }
};
class CTCVillageMember { public: CTCVillage* v; CTCVillage* GetPVillage() const { return v; } };
static EAIActionReturn g_moves[8]; static int g_moveIdx = 0;
static EAIActionReturn g_uses[8]; static int g_useIdx = 0;
class CCreatureAction_UseObject { public: CCreatureAction_UseObject(CThing&, CThing&, bool b) { Log(b ? 15 : 99); } ~CCreatureAction_UseObject() { Log(16); } };
struct CAIStateGroup_VillageTaskHorn {
    CTCVillageMember* PTCVillageMember; CTaskPointer PTask; CThing creature;
    CThing& GetCreature() { return creature; }
    void Terminate() { Log(17); }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat r, C3DVector, float d, bool& reached, EMovementType m, bool a, bool b, bool c, const CThing* t, bool e, float f)
    { Log(r == 2 && d == DELTA_NOTIONAL_ZERO && !reached && m == 3 && a && !b && c && !t && e && f == 0.0f ? 18 : 99); return g_moves[g_moveIdx++]; }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_UseObject&, bool w) { Log(w ? 99 : 19); return g_uses[g_useIdx++]; }
    void Update();
};
void CAIStateGroup_VillageTaskHorn::Update()
{
    if (PTask != 0) {
        CTCVillage* pvillage = PTCVillageMember->GetPVillage();
        if (pvillage->SetHornTaskAsAcceptedByThing(*PTask, GetCreature())) {
            CThing* horn = PTask->GetHorn();
            horn->GetPos();
            C3DVector action_pos;
            CRightHandedSet rhset;
            horn->GetTCGraphicAppearance()->GetDummyObject(ACTION_POINT, 0, 0, action_pos, rhset, 1.0f);
            EAIActionReturn ret;
            do {
                bool nav_route_calculated = false;
                ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, action_pos, DELTA_NOTIONAL_ZERO, nav_route_calculated,
                    AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
                if (PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature())) {
                    Terminate();
                    return;
                }
            } while (ret == AI_ACTION_IN_PROGRESS);
            if (ret != AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
            for (long i = 0; i < 3; ++i) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_UseObject(GetCreature(), *horn, true), false);
                if (ret != AI_ACTION_FINISHED || PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature())) {
                    Terminate();
                    return;
                }
            }
            pvillage->SetHornTaskAsCompleted(PTask);
        }
    }
    Terminate();
}
static bool Check(const int* want, int n)
{
    if (g_n != n) return false;
    for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    CThing horn; CVillageTaskHorn task; task.horn = &horn; CTCVillage village; CTCVillageMember member; member.v = &village;
    CAIStateGroup_VillageTaskHorn g; g.PTCVillageMember = &member;
    bool ok = true;
    // no task: terminate only
    g.PTask.p = 0; g_n = 0; g.Update();
    { static const int w[] = { 17 }; ok = ok && Check(w, 1); }
    // task refused by the village
    g.PTask.p = &task; g_accept = false; g_n = 0; g.Update();
    { static const int w[] = { 13, 17 }; ok = ok && Check(w, 2); }
    // one in-progress move, arrived, three uses, completed
    g_accept = true; g_assignedFails = 1000; g_moves[0] = AI_ACTION_IN_PROGRESS; g_moves[1] = AI_ACTION_FINISHED; g_moveIdx = 0;
    g_uses[0] = g_uses[1] = g_uses[2] = AI_ACTION_FINISHED; g_useIdx = 0; g_n = 0; g.Update();
    { static const int w[] = { 13, 11, 10, 18, 12, 18, 12, 15, 19, 16, 12, 15, 19, 16, 12, 15, 19, 16, 12, 14, 17 }; ok = ok && Check(w, 21); }
    // the second use fails
    g_moves[0] = AI_ACTION_FINISHED; g_moveIdx = 0; g_uses[0] = AI_ACTION_FINISHED; g_uses[1] = AI_ACTION_FAILED; g_useIdx = 0; g_n = 0; g.Update();
    { static const int w[] = { 13, 11, 10, 18, 12, 15, 19, 16, 12, 15, 19, 16, 17 }; ok = ok && Check(w, 13); }
    // the creature loses the task while moving
    g_assignedFails = 1; g_moves[0] = AI_ACTION_IN_PROGRESS; g_moveIdx = 0; g_n = 0; g.Update();
    { static const int w[] = { 13, 11, 10, 18, 12, 17 }; ok = ok && Check(w, 6); }
    if (!ok) { std::printf("00940960_TEST FAIL\n"); return 1; }
    std::printf("00940960_TEST PASS\n");
    return 0;
}