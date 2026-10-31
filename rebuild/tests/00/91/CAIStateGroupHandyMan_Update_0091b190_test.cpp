// Behaviour test for CAIStateGroup_HandyMan::Update (0x0091B190): the work-hours / alive / move / use-object flow.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CTCVillage { public: bool work; bool IsWorkTime() const { return work; } };
class CTCVillageMember { public: CTCVillage* village; CTCVillage* GetPVillage() const { return village; } };
struct CEntry { long first; void* second; };
class CThing { public:
    unsigned int UsingComponents[9];
    CEntry* start; CEntry* finish;
    bool dead;
    C3DVector pos;
    bool IsDead() const { return dead; }
    const C3DVector& GetPos() const { return pos; }
    bool GetTCVillageMember(CTCVillageMember** out)
    {
        bool has = ((UsingComponents[0x23 >> 5] >> (0x23 & 31)) & 1) != 0;
        if (has) {
            long count = finish - start; CEntry* first = start;
            while (count > 0) { long half = count / 2; CEntry* m = first + half;
                if (m->first < 0x23) { first = m + 1; count = count - half - 1; } else count = half; }
            *out = (CTCVillageMember*)((first == finish || 0x23 < first->first) ? finish : first)->second;
        }
        return has;
    }
};
static int g_moveCalls, g_useCalls, g_terminates, g_moveScript[8], g_useResult;
static C3DVector g_movedTo;
struct CAIStateGroup_HandyMan {
    CThing* creature;
    CThing* WorkSpot;
    void Terminate() { ++g_terminates; }
    bool IsInWorkHours()
    {
        CTCVillageMember* member;
        if (creature->GetTCVillageMember(&member)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->IsWorkTime()) return true;
        }
        return false;
    }
    int AIAction_MoveToPos(int, C3DVector pos, bool& arrived, int, bool, bool, const CThing*, bool, float)
    { g_movedTo = pos; arrived = false; return g_moveScript[g_moveCalls++]; }
    int UseAndWait(CThing&, CThing&) { ++g_useCalls; return g_useResult; }
    void Update()
    {
        C3DVector spot = WorkSpot->GetPos();
        int result;
        do {
            if (!IsInWorkHours()) { Terminate(); return; }
            if (!WorkSpot || WorkSpot->IsDead()) { Terminate(); return; }
            bool arrived = false;
            result = AIAction_MoveToPos(2, spot, arrived, 3, true, true, 0, true, 0.0f);
        } while (result == 4);
        if (result == 2) {
            bool failed = UseAndWait(*creature, *WorkSpot) != 2;
            if (failed) { Terminate(); return; }
        }
        Terminate();
    }
};
static void reset(const int* script, int n, int use) { g_moveCalls = g_useCalls = g_terminates = 0; for (int i = 0; i < n; ++i) g_moveScript[i] = script[i]; g_useResult = use; }
int main()
{
    CTCVillage village = { true };
    CTCVillageMember member = { &village };
    CEntry entries[3] = { { 0x10, 0 }, { 0x23, &member }, { 0x40, 0 } };
    CThing creature = { { 0 }, entries, entries + 3, false, { 0, 0, 0 } };
    creature.UsingComponents[1] = 1u << 3;
    CThing spot = { { 0 }, 0, 0, false, { 1.0f, 2.0f, 3.0f } };
    CAIStateGroup_HandyMan h = { &creature, &spot };
    int s1[3] = { 4, 4, 2 }; reset(s1, 3, 2); h.Update();
    if (g_moveCalls != 3 || g_useCalls != 1 || g_terminates != 1 || g_movedTo.Y != 2.0f) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    int s2[1] = { 1 }; reset(s2, 1, 2); h.Update();
    if (g_moveCalls != 1 || g_useCalls != 0 || g_terminates != 1) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    reset(s1, 3, 0); h.Update();                                  // use fails: still one Terminate
    if (g_useCalls != 1 || g_terminates != 1) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    village.work = false; reset(s1, 3, 2); h.Update();            // out of work hours
    if (g_moveCalls != 0 || g_terminates != 1) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    village.work = true; spot.dead = true; reset(s1, 3, 2); h.Update();
    if (g_moveCalls != 0 || g_terminates != 1) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    spot.dead = false; creature.UsingComponents[1] = 0; reset(s1, 3, 2); h.Update();   // no village-member TC
    if (g_moveCalls != 0 || g_terminates != 1) { std::printf("0091b190_TEST FAIL\n"); return 1; }
    std::printf("0091b190_TEST PASS\n");
    return 0;
}