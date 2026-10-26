#include <math.h>
#include <stdio.h>
#include <stdlib.h>
const float Zero_0122dedc = 0.0f;
// BEGIN exact source snapshot SHA256 05619e533eccf782f9e30bfb07d8f73c77e38772ea9dc223a349c047eb3ca5fc
// 00B109B0 NParticleEngine::CPSCEmitterGeneric::GetApproximateSplineLength.
// PDB: ?GetApproximateSplineLength@CPSCEmitterGeneric@NParticleEngine@@UAEMXZ
// (Ego_r row 30272, debug address 00AD17F0). Local layout follows the
// landed CPSCEmitterGeneric::WriteBinary model: Spline counted pointer +0x18,
// EmitterType bits 20-21 of word +0x30, and CCardinalSpline control-point
// vector at offsets 0/+4/+8. Algorithm follows the matching spline-base
// implementation: sum squared deltas for consecutive points, then sqrt once.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float Zero_0122dedc; // retail zero constant at 0x0122DEDC

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& b) : X(b.X), Y(b.Y), Z(b.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& b) const
    {
        C3DVector r;
        r.X = X - b.X; r.Y = Y - b.Y; r.Z = Z - b.Z;
        return r;
    }
    float SquaredLength() const { return Y * Y + (Z * Z + X * X); }
};
struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
class CCardinalSpline { public: CPointVector ControlPoints; };
struct SCountedInfo;
template<class T> class CCountedPointer { public: T* Data; SCountedInfo* Info; };
namespace NParticleEngine {
class CPSCEmitterGeneric { public:
    unsigned char _pad00[0x18];
    CCountedPointer<CCardinalSpline> Spline;
    unsigned long _pad20;
    unsigned long _pad24;
    long _pad28;
    float _pad2c;
    unsigned long NoParticlesToStart : 10;
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long _pad32 : 10;
    C3DVector* begin() const { return Spline.Data->ControlPoints._M_start; }
    C3DVector& at(int i) const { return *(begin() + i); }
    int size() const { return (int)(Spline.Data->ControlPoints._M_finish - begin()); }
    float GetApproximateSplineLength();
};
float CPSCEmitterGeneric::GetApproximateSplineLength()
{
    float total = Zero_0122dedc;
    if (EmitterType == 3 && Spline.Data != 0) {
        int count = size();
        for (int i = 0; i < count - 1; ++i) {
            total += (at(i) - at(i + 1)).SquaredLength();
        }
    }
    float result = (float)sqrt(total);
    return result;
}
}

// END exact source snapshot
static int nearf(float a, float b) { return fabs(a - b) < 0.0001f; }
int main()
{
    using namespace NParticleEngine;
    static CPSCEmitterGeneric emitter = {};
    if (!nearf(emitter.GetApproximateSplineLength(), 0.0f)) return 1;
    emitter.EmitterType = 3;
    if (!nearf(emitter.GetApproximateSplineLength(), 0.0f)) return 2;
    static C3DVector points[3];
    points[0] = C3DVector(0.0f, 0.0f, 0.0f);
    points[1] = C3DVector(3.0f, 0.0f, 0.0f);
    points[2] = C3DVector(3.0f, 4.0f, 0.0f);
    static CCardinalSpline spline = {};
    spline.ControlPoints._M_start = points;
    spline.ControlPoints._M_finish = points + 3;
    spline.ControlPoints._M_end_of_storage = points + 3;
    static SCountedInfo* emptyInfo = 0;
    emitter.Spline.Data = &spline;
    emitter.Spline.Info = emptyInfo;
    // Retail sums squared segment distances (9 + 16), then takes one root.
    if (!nearf(emitter.GetApproximateSplineLength(), 5.0f)) return 3;
    emitter.EmitterType = 2;
    if (!nearf(emitter.GetApproximateSplineLength(), 0.0f)) return 4;
    puts("00b109b0_TEST PASS");
    return 0;
}