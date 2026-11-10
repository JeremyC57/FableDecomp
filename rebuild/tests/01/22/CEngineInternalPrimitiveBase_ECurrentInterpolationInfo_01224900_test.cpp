// Behaviour test for the dynamic initializer @ 0x01224900: CEngineInternalPrimitiveBase::CurrentInterpolationInfo is built by
// CEngineInterpolationInfo's default constructor before main (every set zeroed, the list seeded from Current / PausedCurrent /
// BulletTimeCurrent as the landed 0x0042FA30 does), exactly once.
#include <cstdio>
static int g_ctorCalls;
class CEngineInterpolationInfoSet {
public:
    float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate;
    CEngineInterpolationInfoSet() : GTPredictedRenderAt(0.0f), GTPredictedTimeSinceLastRenderFrame(0.0f), WFInterpolate(0.0f) { }
};
class CEngineInterpolationInfo {
public:
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    int WFServerCurrentTime;   // long in the source (4 bytes under VC7.1)
    CEngineInterpolationInfoSet InterpolationSetList[3];
    CEngineInterpolationInfo() : WFServerCurrentTime(0)
    {
        ++g_ctorCalls;
        InterpolationSetList[0] = Current; InterpolationSetList[1] = PausedCurrent; InterpolationSetList[2] = BulletTimeCurrent;
    }
};
class CEngineInternalPrimitiveBase {
public:
    static CEngineInterpolationInfo CurrentInterpolationInfo;
};
CEngineInterpolationInfo CEngineInternalPrimitiveBase::CurrentInterpolationInfo;
int main()
{
    const CEngineInterpolationInfo& i = CEngineInternalPrimitiveBase::CurrentInterpolationInfo;
    bool ok = g_ctorCalls == 1 && i.WFServerCurrentTime == 0 && i.Current.WFInterpolate == 0.0f
           && i.InterpolationSetList[2].GTPredictedRenderAt == 0.0f && sizeof(CEngineInterpolationInfo) == 112;
    if (!ok) { std::printf("01224900_TEST FAIL\n"); return 1; }
    std::printf("01224900_TEST PASS\n");
    return 0;
}