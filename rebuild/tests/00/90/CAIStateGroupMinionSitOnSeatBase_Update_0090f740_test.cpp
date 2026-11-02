// Test: CAIStateGroup_MinionSitOnSeatBase::Update() sits down (movement type 3), idles while DoNothing returns 4 and the group stays valid, then Terminate().
#include <cstdio>
static int g_terminate = 0, g_doNothing = 0, g_valid = 0, g_lastMove = -1;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() { }
    int DoNothingResults[8];
    int ValidLeft;
    bool CanSit;
    void Terminate() { ++g_terminate; }
protected:
    virtual void Update() { }
    virtual bool IsStateGroupStillValid() const { ++g_valid; return true; }
    int AIAction_DoNothing() { return DoNothingResults[g_doNothing++]; }
};
class CAIStateGroup_UsesSeat : public CAIStateGroupBase { public:
protected:
    bool MoveToSeatAndSitDown(int movementType) { g_lastMove = movementType; return CanSit; }
};
class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat { public:
    void RunUpdate() { Update(); }
protected:
    virtual void Update();
    virtual bool IsStateGroupStillValid() const { ++g_valid; return g_valid < ValidLeft; }
};
void CAIStateGroup_MinionSitOnSeatBase::Update()
{
    if (MoveToSeatAndSitDown(3)) {
        while (AIAction_DoNothing() == 4 && IsStateGroupStillValid())
            ;
    }
    Terminate();
}
int main()
{
    CAIStateGroup_MinionSitOnSeatBase group;
    for (int i = 0; i < 8; ++i) group.DoNothingResults[i] = 4;
    group.ValidLeft = 3; group.CanSit = true;
    group.RunUpdate();                            // valid twice, the third check fails
    if (g_lastMove != 3 || g_doNothing != 3 || g_valid != 3 || g_terminate != 1) { std::printf("0090f740_TEST FAIL\n"); return 1; }
    g_doNothing = 0; g_valid = 0; group.DoNothingResults[1] = 2; group.ValidLeft = 100;
    group.RunUpdate();                            // second DoNothing is not 4
    if (g_doNothing != 2 || g_valid != 1 || g_terminate != 2) { std::printf("0090f740_TEST FAIL\n"); return 1; }
    g_doNothing = 0; g_valid = 0; group.CanSit = false;
    group.RunUpdate();
    if (g_doNothing != 0 || g_terminate != 3) { std::printf("0090f740_TEST FAIL\n"); return 1; }
    std::printf("0090f740_TEST PASS\n");
    return 0;
}