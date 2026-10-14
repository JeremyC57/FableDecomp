#include <cstdio>

struct CBaseObject { int x; };
static int g_ctor = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseIntelligentPointer() : PItem((CBaseObject*)1) { ++g_ctor; }
    void SetPItem(CBaseObject* item) { PItem = item; } };

struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } };
struct CGroupMemberData { CIntelligentPointer PMember; bool IsActive; unsigned long JoinedFrame; unsigned long DeactivatedFrame;
    CGroupMemberData(CBaseObject* member, bool isActive, unsigned long joinedFrame, unsigned long deactivatedFrame)
        : PMember(member), IsActive(isActive), JoinedFrame(joinedFrame), DeactivatedFrame(deactivatedFrame) { } };
int main()
{
    CBaseObject o; CGroupMemberData d(&o, true, 10, 20);
    if (g_ctor != 1 || d.PMember.PItem != &o || !d.IsActive || d.JoinedFrame != 10 || d.DeactivatedFrame != 20 || sizeof(CGroupMemberData) != 0x14) { std::printf("00812890_TEST FAIL\n"); return 1; }
    std::printf("00812890_TEST PASS\n"); return 0;
}
