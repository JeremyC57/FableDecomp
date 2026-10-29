// Behaviour test for CAIBrain::~CAIBrain (0x0088BBE0): OnKill runs only when it has not run yet, then every state group is
// deleted from the back of the list, then the base dtor runs.
#include <stdio.h>
static int g_onKill = 0, g_baseDtor = 0;
static int g_deleted[8]; static int g_nDeleted = 0;
class CAIStateGroupBase { public:
    int Id;
    CAIStateGroupBase(int id) : Id(id) { }
    virtual ~CAIStateGroupBase() { g_deleted[g_nDeleted++] = Id; }
};
struct CStateGroupEntry { long first; CAIStateGroupBase* second; };
class CStateGroupList { public:                    // stand-in for the vector map: back / pop_back / empty
    CStateGroupEntry Items[8]; int Count;
    CStateGroupList() : Count(0) { }
    bool empty() const { return Count == 0; }
    CStateGroupEntry& back() { return Items[Count - 1]; }
    void pop_back() { --Count; }
};
class CSuspendableProcessMicrothread { public:
    virtual ~CSuspendableProcessMicrothread() { g_baseDtor++; }
};
class CAIBrain : public CSuspendableProcessMicrothread { public:
    CStateGroupList StateGroupList;
    bool OnKillCalled;
    CAIBrain() : OnKillCalled(false) { }
    void OnKill() { g_onKill++; OnKillCalled = true; }
    virtual ~CAIBrain()
    {
        if (!OnKillCalled)
            OnKill();
        while (!StateGroupList.empty()) {
            delete StateGroupList.back().second;
            StateGroupList.pop_back();
        }
    }
};
int main()
{
    CAIBrain* brain = new CAIBrain;
    for (int i = 0; i < 3; ++i) { brain->StateGroupList.Items[i].first = 10 - i; brain->StateGroupList.Items[i].second = new CAIStateGroupBase(i + 1); }
    brain->StateGroupList.Count = 3;
    delete brain;
    if (g_onKill != 1 || g_nDeleted != 3 || g_deleted[0] != 3 || g_deleted[1] != 2 || g_deleted[2] != 1 || g_baseDtor != 1)
        { printf("0088bbe0_TEST FAIL\n"); return 1; }
    CAIBrain* killed = new CAIBrain;                  // OnKill already ran: not again
    killed->OnKill();
    delete killed;
    if (g_onKill != 2 || g_nDeleted != 3 || g_baseDtor != 2) { printf("0088bbe0_TEST FAIL\n"); return 1; }
    printf("0088bbe0_TEST PASS\n");
    return 0;
}