// CEngineInterpolationInfo::CEngineInterpolationInfo() -- default constructor: every interpolation set and the server time zeroed, then the three-entry set list seeded from Current, PausedCurrent and BulletTimeCurrent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042FA30, length 158 bytes. No callees.
// Layout: PDB CEngineInterpolationInfo (112 bytes): six 12-byte CEngineInterpolationInfoSet (GTPredictedRenderAt,
// GTPredictedTimeSinceLastRenderFrame, WFInterpolate) at +0x00..+0x3C, WFServerCurrentTime +0x48, InterpolationSetList[3] +0x4C.
// Shape: the set's inline zeroing constructor runs for the six named sets (one zero register edx), the server time is an
// initialiser, and the array-of-sets construction is the inlined, unrolled element loop (`lea esi,[eax+4Ch]`, `add ecx,0Ch` steps,
// including a dead final step); the three body copies are the implicit memberwise set assignment through ecx/edx.
struct CInterpolationInfoSet {                     // PDB, 8 bytes
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};

class CInterpolationInfo { public:                        // PDB, 52 bytes
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

class CEngineInterpolationInfo { public:                  // PDB, 112 bytes
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

CEngineInterpolationInfo::CEngineInterpolationInfo()
    : WFServerCurrentTime(0)
{
    InterpolationSetList[0] = Current;
    InterpolationSetList[1] = PausedCurrent;
    InterpolationSetList[2] = BulletTimeCurrent;
}