#include <cstdio>
static int g_base = 0;
struct CScriptModuleBase { virtual void v00() { } unsigned char m_state[0x4C - 4]; CScriptModuleBase() { ++g_base; } };
namespace NCameraModes {
struct CScriptModuleSpline : CScriptModuleBase {
    float Index, CurrentSplineFOV, CurrentSplineRollAngle; bool ReallyFinished, FinishedSpline; long NodeToStartOn, NodeToEndOn;
    CScriptModuleSpline(long a, long b) : CScriptModuleBase(), Index(0.0f), CurrentSplineFOV(0.2f), CurrentSplineRollAngle(0.0f), ReallyFinished(false), FinishedSpline(false), NodeToStartOn(a), NodeToEndOn(b) { }
}; }
int main()
{
    NCameraModes::CScriptModuleSpline s(3, 9);
    if (g_base != 1 || s.Index != 0.0f || s.CurrentSplineFOV != 0.2f || s.CurrentSplineRollAngle != 0.0f || s.ReallyFinished || s.FinishedSpline || s.NodeToStartOn != 3 || s.NodeToEndOn != 9) { std::printf("00852710_TEST FAIL\n"); return 1; }
    std::printf("00852710_TEST PASS\n"); return 0;
}
