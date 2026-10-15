// CEngineInterpolationInfo::CEngineInterpolationInfo(const CInterpolationInfo& info) -- converting constructor: zeroed sets (the server time is left to the assignment), then `*this = info`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069EDE0, length 110 bytes (`ret 4`, returns this).
// Callees: CEngineInterpolationInfo::operator=(const CInterpolationInfo&) @ 0x0069ED40 (out of line).
// Shape: no initialiser for WFServerCurrentTime (+0x48 is skipped by the zero run), the six named sets and the first list entry
// are zeroed by the sets' inline constructors through ecx, the other two list entries through the unrolled element loop
// (`lea eax,[esi+4Ch] ; add eax,0Ch`); the assignment is the out-of-line operator.
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

CEngineInterpolationInfo::CEngineInterpolationInfo(const CInterpolationInfo& info)
{
    *this = info;
}