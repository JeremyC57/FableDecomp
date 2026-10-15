// CEngineInterpolationInfo::operator=(const CInterpolationInfo& info) -- takes the game-side interpolation info: each of the six sets copies its two shared fields (render-at stays), the server time is copied, and the set list is re-seeded from the three "current" sets
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069ED40, length 121 bytes (`ret 4`, returns this). No callees.
// Layout: source PDB CInterpolationInfo (52 bytes: six 8-byte CInterpolationInfoSet {GTPredictedTimeSinceLastRenderFrame,
// WFInterpolate} then WFServerCurrentTime +0x30); destination PDB CEngineInterpolationInfo (see 0x0042FA30), whose 12-byte sets
// carry GTPredictedRenderAt first, so every pair lands at +4/+8 of its set.
// Shape: the set conversion is an inline `CEngineInterpolationInfoSet::operator=(const CInterpolationInfoSet&)`; every word goes
// through edx in declaration order, the source re-read for the three list entries.
// Leaf: ??4CEngineInterpolationInfo
struct CInterpolationInfoSet {                     // PDB, 8 bytes
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};

struct CInterpolationInfo {                        // PDB, 52 bytes
    CInterpolationInfoSet Current;                 // +0x00
    CInterpolationInfoSet Last;                    // +0x08
    CInterpolationInfoSet PausedCurrent;           // +0x10
    CInterpolationInfoSet PausedLast;              // +0x18
    CInterpolationInfoSet BulletTimeCurrent;       // +0x20
    CInterpolationInfoSet BulletTimeLast;          // +0x28
    long                  WFServerCurrentTime;     // +0x30
};

struct CEngineInterpolationInfoSet {               // PDB, 12 bytes
    float GTPredictedRenderAt;                     // +0x00
    float GTPredictedTimeSinceLastRenderFrame;     // +0x04
    float WFInterpolate;                           // +0x08
    CEngineInterpolationInfoSet() : GTPredictedRenderAt(0.0f), GTPredictedTimeSinceLastRenderFrame(0.0f), WFInterpolate(0.0f) { }
    CEngineInterpolationInfoSet& operator=(const CInterpolationInfoSet& set)
    {
        GTPredictedTimeSinceLastRenderFrame = set.GTPredictedTimeSinceLastRenderFrame;
        WFInterpolate = set.WFInterpolate;
        return *this;
    }
};

struct CEngineInterpolationInfo {                  // PDB, 112 bytes
    CEngineInterpolationInfoSet Current;           // +0x00
    CEngineInterpolationInfoSet Last;              // +0x0c
    CEngineInterpolationInfoSet PausedCurrent;     // +0x18
    CEngineInterpolationInfoSet PausedLast;        // +0x24
    CEngineInterpolationInfoSet BulletTimeCurrent; // +0x30
    CEngineInterpolationInfoSet BulletTimeLast;    // +0x3c
    long                        WFServerCurrentTime;   // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
    CEngineInterpolationInfo();
    CEngineInterpolationInfo(const CInterpolationInfo& info);
    CEngineInterpolationInfo& operator=(const CInterpolationInfo& info);
};

CEngineInterpolationInfo& CEngineInterpolationInfo::operator=(const CInterpolationInfo& info)
{
    Current = info.Current;
    Last = info.Last;
    PausedCurrent = info.PausedCurrent;
    PausedLast = info.PausedLast;
    BulletTimeCurrent = info.BulletTimeCurrent;
    BulletTimeLast = info.BulletTimeLast;
    WFServerCurrentTime = info.WFServerCurrentTime;
    InterpolationSetList[0] = info.Current;
    InterpolationSetList[1] = info.PausedCurrent;
    InterpolationSetList[2] = info.BulletTimeCurrent;
    return *this;
}