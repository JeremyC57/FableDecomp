#include <cstdio>
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
class CInterpolationInfo { public:
    CInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long WFServerCurrentTime;
};
struct CEngineInterpolationInfoSet {
    float GTPredictedRenderAt; float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate;
    CEngineInterpolationInfoSet() : GTPredictedRenderAt(0.0f), GTPredictedTimeSinceLastRenderFrame(0.0f), WFInterpolate(0.0f) { }
    CEngineInterpolationInfoSet& operator=(const CInterpolationInfoSet& s)
    { GTPredictedTimeSinceLastRenderFrame = s.GTPredictedTimeSinceLastRenderFrame; WFInterpolate = s.WFInterpolate; return *this; }
};
class CEngineInterpolationInfo { public:
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long WFServerCurrentTime;
    CEngineInterpolationInfoSet InterpolationSetList[3];
    CEngineInterpolationInfo() : WFServerCurrentTime(0)
    { InterpolationSetList[0] = Current; InterpolationSetList[1] = PausedCurrent; InterpolationSetList[2] = BulletTimeCurrent; }
    CEngineInterpolationInfo(const CInterpolationInfo& info) { *this = info; }
    CEngineInterpolationInfo& operator=(const CInterpolationInfo& info)
    {
        Current = info.Current; Last = info.Last; PausedCurrent = info.PausedCurrent; PausedLast = info.PausedLast;
        BulletTimeCurrent = info.BulletTimeCurrent; BulletTimeLast = info.BulletTimeLast;
        WFServerCurrentTime = info.WFServerCurrentTime;
        InterpolationSetList[0] = info.Current; InterpolationSetList[1] = info.PausedCurrent; InterpolationSetList[2] = info.BulletTimeCurrent;
        return *this;
    }
};
static void Fill(CInterpolationInfo& i)
{
    CInterpolationInfoSet* s[6] = { &i.Current, &i.Last, &i.PausedCurrent, &i.PausedLast, &i.BulletTimeCurrent, &i.BulletTimeLast };
    for (int k = 0; k < 6; ++k) { s[k]->GTPredictedTimeSinceLastRenderFrame = 1.0f + k; s[k]->WFInterpolate = 10.0f + k; }
    i.WFServerCurrentTime = 1234;
}
int main()
{
    CInterpolationInfo i; Fill(i);
    CEngineInterpolationInfo e; e.Last.GTPredictedRenderAt = 5.0f;
    CEngineInterpolationInfo& r = (e = i);
    if (&r != &e || e.Last.GTPredictedRenderAt != 5.0f || e.Last.GTPredictedTimeSinceLastRenderFrame != 2.0f || e.BulletTimeLast.WFInterpolate != 15.0f
        || e.WFServerCurrentTime != 1234 || e.InterpolationSetList[1].WFInterpolate != 12.0f || e.InterpolationSetList[2].GTPredictedTimeSinceLastRenderFrame != 5.0f) {
        std::printf("0069ed40_TEST FAIL\n"); return 1;
    }
    std::printf("0069ed40_TEST PASS\n");
    return 0;
}