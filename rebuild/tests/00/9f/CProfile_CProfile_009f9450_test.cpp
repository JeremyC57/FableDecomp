// Behaviour test for CProfile::CProfile(const CProfile&) (0x009F9450): every member copied, the name through its own
// copy constructor.
#include <cstdio>
static int g_nameCopies = 0;
class CWideString { public: int id; CWideString() : id(0) { } CWideString(const CWideString& o) : id(o.id) { ++g_nameCopies; } };
class CProfile { public:
    void* Manager; double StartTime; double TotalTime; double LastTime; bool Running; double Percentage; CWideString Name;
    long TreeDepth; bool NewNode; long Visible; long CallCounter; long UpdatedCallCounter;
    CProfile() { }
    CProfile(const CProfile& o)
        : Manager(o.Manager), StartTime(o.StartTime), TotalTime(o.TotalTime), LastTime(o.LastTime), Running(o.Running), Percentage(o.Percentage),
          Name(o.Name), TreeDepth(o.TreeDepth), NewNode(o.NewNode), Visible(o.Visible), CallCounter(o.CallCounter), UpdatedCallCounter(o.UpdatedCallCounter) { }
};
int main()
{
    int mgr;
    CProfile a; a.Manager = &mgr; a.StartTime = 1.5; a.TotalTime = 2.5; a.LastTime = 3.5; a.Running = true; a.Percentage = 42.25; a.Name.id = 9;
    a.TreeDepth = 3; a.NewNode = true; a.Visible = 1; a.CallCounter = 10; a.UpdatedCallCounter = 11;
    CProfile b(a);
    bool ok = b.Manager == &mgr && b.StartTime == 1.5 && b.TotalTime == 2.5 && b.LastTime == 3.5 && b.Running && b.Percentage == 42.25
           && b.Name.id == 9 && g_nameCopies == 1 && b.TreeDepth == 3 && b.NewNode && b.Visible == 1 && b.CallCounter == 10 && b.UpdatedCallCounter == 11;
    if (!ok) { std::printf("009f9450_TEST FAIL\n"); return 1; }
    std::printf("009f9450_TEST PASS\n");
    return 0;
}