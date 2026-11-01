// Behaviour test for NParticleEngine::CPSCSplineBase::~CPSCSplineBase (0x00B0A060): SplinePosParamData then SplineRandomOffsetData
// are released (deleted on the last reference), the spline's control-point array is freed, then the base chain.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_log[64];
static int g_logLen;
static void Log(char c) { if (g_logLen < 63) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; } }
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer { public:                     // engine/CCountedPointer.h destructor
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Share(T* data, CCPPointerInfo* info) { Data = data; Info = info; ++info->RefCount; }
};
struct CPayload { char Tag; };
static void DeletePayload(void* data) { Log(((CPayload*)data)->Tag); }

class C3DVector { public: float X, Y, Z; };
struct CArray { C3DVector* Start; C3DVector* Finish; C3DVector* End; ~CArray() { if (Start) { Log('a'); std::free(Start); } } };
class CCardinalSpline { public: CArray ControlPoints; float Tension; bool EndPointsConnected; };
struct CBaseClassNonCopyable { ~CBaseClassNonCopyable() { Log('b'); } };
struct CPSCBase : CBaseClassNonCopyable { void* ParticleSystem; unsigned char Flags; };
struct CPSCSplineBase : CPSCBase {
    float SplineAnimSpeed;
    CCardinalSpline Spline;
    CCountedPointer<CPayload> SplineRandomOffsetData;
    CCountedPointer<CPayload> SplinePosParamData;
};
int main()
{
    CPayload offset = { 'O' }, pos = { 'P' };
    CCPPointerInfo* offsetInfo = new CCPPointerInfo; offsetInfo->RefCount = 1; offsetInfo->DeleteFunc = DeletePayload; offsetInfo->Data = &offset;
    CCPPointerInfo posInfo = { 0, DeletePayload, &pos };
    CPSCSplineBase* spline = new CPSCSplineBase;
    spline->Spline.ControlPoints.Start = (C3DVector*)std::malloc(4 * sizeof(C3DVector));
    spline->SplineRandomOffsetData.Data = &offset; spline->SplineRandomOffsetData.Info = offsetInfo;
    spline->SplinePosParamData.Share(&pos, &posInfo); posInfo.RefCount = 2;   // shared: survives
    delete spline;
    CPSCSplineBase* empty = new CPSCSplineBase;
    empty->Spline.ControlPoints.Start = 0;
    delete empty;
    if (std::strcmp(g_log, "Oabb") != 0 || posInfo.RefCount != 1) { std::printf("00b0a060_TEST FAIL\n"); return 1; }
    std::printf("00b0a060_TEST PASS\n");
    return 0;
}