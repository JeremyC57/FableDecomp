#include <cstdio>
static unsigned long g_frame = 0; static bool g_critical = false;
class CBrainUpdateZone { public: unsigned char _pad[4]; unsigned long m_updatePeriod; };
class CMainGameComponent { public: unsigned long GetFrameNumber() { return g_frame; } };
struct CThingAICreatureCalls { unsigned char _pad[0x18]; unsigned __int64 m_updateOffset; };
struct CAIStateGroupBaseCalls { virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual bool IsTimingCritical() { return g_critical; } };
class CAIBrain { public: void* __vftable; unsigned char _pad0[0x10]; CMainGameComponent* Component; CThingAICreatureCalls* Creature; unsigned char _pad1[0x1c]; CAIStateGroupBaseCalls* PActiveStateGroup; unsigned char _pad2[0x64]; CBrainUpdateZone* PCurrentUpdateZone; bool IsUpdateZoneActiveThisFrame(); };
bool CAIBrain::IsUpdateZoneActiveThisFrame()
{
    if (PCurrentUpdateZone == 0) return false;
    unsigned long period = PCurrentUpdateZone->m_updatePeriod; unsigned __int64 offset = Creature->m_updateOffset;
    if ((offset + Component->GetFrameNumber()) % period == 0) return true;
    if (PActiveStateGroup != 0 && PActiveStateGroup->IsTimingCritical()) return true;
    return false;
}
int main()
{
    CMainGameComponent comp; CThingAICreatureCalls cre; cre.m_updateOffset = 0x100000003ULL; CAIStateGroupBaseCalls grp; CBrainUpdateZone zone; zone.m_updatePeriod = 4;
    CAIBrain b; b.Component = &comp; b.Creature = &cre; b.PActiveStateGroup = 0; b.PCurrentUpdateZone = 0;
    if (b.IsUpdateZoneActiveThisFrame()) { std::printf("0088b810_TEST FAIL\n"); return 1; }
    b.PCurrentUpdateZone = &zone; g_frame = 1;      // (2^32+3+1) % 4 == 0
    if (!b.IsUpdateZoneActiveThisFrame()) { std::printf("0088b810_TEST FAIL\n"); return 1; }
    g_frame = 2; if (b.IsUpdateZoneActiveThisFrame()) { std::printf("0088b810_TEST FAIL\n"); return 1; }
    b.PActiveStateGroup = &grp; if (b.IsUpdateZoneActiveThisFrame()) { std::printf("0088b810_TEST FAIL\n"); return 1; }
    g_critical = true; if (!b.IsUpdateZoneActiveThisFrame()) { std::printf("0088b810_TEST FAIL\n"); return 1; }
    std::printf("0088b810_TEST PASS\n");
    return 0;
}