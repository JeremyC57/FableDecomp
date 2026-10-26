// Behaviour test for CAIStateGroup_GoToPub::OnReplace (0x009498B0): model copy with scripted stubs.
#include <cstdio>
#include <cstring>

class CThing { public: int id; };
struct CPointer { CThing* item; CPointer& operator=(CThing* p) { item = p; return *this; } };
class CCharString { public: const char* s; CCharString(const char* t, long = -1) : s(t) { } };
static unsigned long g_removed[8]; static int g_nr;
class CTCCreatureModeManager { public: void RemoveMode(unsigned long m) { g_removed[g_nr++] = m; } };
struct CCreature { CTCCreatureModeManager* modes; void GetInterface(CTCCreatureModeManager** o, long t) { if (t == 0x31 && modes) *o = modes; } };
class CAIBrain { public: CCreature* PCreature; };
static bool g_getUpResult; static int g_getUps; static const char* g_anim; static long g_movement;
static bool GetUpFromSitting(CCreature*, long movement, const CCharString* anim, CAIBrain*) { ++g_getUps; g_anim = anim->s; g_movement = movement; return g_getUpResult; }
static int g_freeSeat, g_freeBar;

struct CAIStateGroup_GoToPub {
    CAIBrain* PBrain;
    CPointer PPub, PBar, PTable, PBarman, PTankard;
    long SeatIndex; bool Sitting; long BarActionPointIndex; bool HandsOffMode;
    void FreeSeat() { ++g_freeSeat; }
    void FreeBarPoint() { ++g_freeBar; }
    void OnReplace();
};

void CAIStateGroup_GoToPub::OnReplace()
{
    bool removeMode = true;
    CTCCreatureModeManager* modes = 0;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    FreeSeat();
    if (BarActionPointIndex != -1)
        FreeBarPoint();
    if (Sitting) {
        bool gotUp;
        {
            CCharString anim("SIT_AT_TABLE_OUTOF");
            gotUp = GetUpFromSitting(PBrain->PCreature, 0xb, &anim, PBrain);
        }
        if (gotUp)
            removeMode = false;
        Sitting = false;
    }
    if (removeMode)
        modes->RemoveMode(0xb);
    if (HandsOffMode) {
        HandsOffMode = false;
        modes->RemoveMode(0x2d);
    }
    PPub = 0;
    PBar = 0;
    PTable = 0;
    PBarman = 0;
    PTankard = 0;
    SeatIndex = -1;
}

int main()
{
    int ok = 1;
    CThing t = { 1 };
    CTCCreatureModeManager mm; CCreature c = { &mm }; CAIBrain b = { &c };
    CAIStateGroup_GoToPub s; s.PBrain = &b;
    // sitting, get up succeeds: no 0xb removal; hands-off cleared
    s.PPub.item = s.PBar.item = s.PTable.item = s.PBarman.item = s.PTankard.item = &t;
    s.SeatIndex = 3; s.Sitting = true; s.BarActionPointIndex = 2; s.HandsOffMode = true; g_getUpResult = true; g_nr = 0;
    s.OnReplace();
    if (g_freeSeat != 1 || g_freeBar != 1 || g_getUps != 1 || std::strcmp(g_anim, "SIT_AT_TABLE_OUTOF") || g_movement != 0xb) ok = 0;
    if (g_nr != 1 || g_removed[0] != 0x2d || s.Sitting || s.HandsOffMode || s.SeatIndex != -1) ok = 0;
    if (s.PPub.item || s.PBar.item || s.PTable.item || s.PBarman.item || s.PTankard.item) ok = 0;
    // sitting, get up fails: remove the sitting mode
    s.Sitting = true; s.BarActionPointIndex = -1; g_getUpResult = false; g_nr = 0;
    s.OnReplace();
    if (g_freeBar != 1 || g_getUps != 2 || g_nr != 1 || g_removed[0] != 0xb) ok = 0;
    // not sitting: remove the mode without getting up
    g_nr = 0; s.OnReplace();
    if (g_getUps != 2 || g_nr != 1 || g_removed[0] != 0xb || g_freeSeat != 3) ok = 0;
    if (!ok) { std::printf("009498b0_TEST FAIL\n"); return 1; }
    std::printf("009498b0_TEST PASS\n");
    return 0;
}