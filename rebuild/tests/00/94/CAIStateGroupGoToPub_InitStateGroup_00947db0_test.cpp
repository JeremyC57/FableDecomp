// Behaviour test for CAIStateGroup_GoToPub::InitStateGroup (0x00947DB0): every reset field ends at its
// initial value and the tankard pointer is cleared; untouched fields keep their values.
#include <cstdio>

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CThing { int dummy; };
struct CWorld {}; struct CThingAICreature {};

struct CAIStateGroup_GoToPub {
    CAIStateGroup_GoToPub() : PTankard(0) {}
    CIntelligentPointer<CThing> PTankard;
    long SeatIndex, ActionPointIndex, NextFramePub, FrameLeavePub;
    bool Sitting;
    long BarActionPointIndex, BarIndex;
    bool HandsOffMode;
    long TimeBeforeAskingForBeer;
    void InitStateGroup(const CWorld&, CThingAICreature&)
    {
        NextFramePub = 0;
        Sitting = false;
        ActionPointIndex = -1;
        PTankard = CIntelligentPointer<CThing>(0);
        HandsOffMode = false;
        TimeBeforeAskingForBeer = 0;
        BarActionPointIndex = -1;
        SeatIndex = -1;
    }
};

int main()
{
    CThing tankard; CWorld w; CThingAICreature c;
    CAIStateGroup_GoToPub g;
    g.PTankard.SetPItem(&tankard);
    g.SeatIndex = 3; g.ActionPointIndex = 4; g.NextFramePub = 5; g.FrameLeavePub = 6; g.Sitting = true;
    g.BarActionPointIndex = 7; g.BarIndex = 8; g.HandsOffMode = true; g.TimeBeforeAskingForBeer = 9;
    g.InitStateGroup(w, c);
    if (g.PTankard.GetPItem() != 0 || g.SeatIndex != -1 || g.ActionPointIndex != -1 || g.NextFramePub != 0
        || g.FrameLeavePub != 6 || g.Sitting || g.BarActionPointIndex != -1 || g.BarIndex != 8 || g.HandsOffMode
        || g.TimeBeforeAskingForBeer != 0) { std::printf("00947db0_TEST FAIL\n"); return 1; }
    std::printf("00947db0_TEST PASS\n");
    return 0;
}