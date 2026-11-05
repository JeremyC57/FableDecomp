// 00B109B0 NParticleEngine::CPSCEmitterGeneric::GetApproximateSplineLength.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local SquaredLength respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// PDB: ?GetApproximateSplineLength@CPSCEmitterGeneric@NParticleEngine@@UAEMXZ
// (Ego_r row 30272, debug address 00AD17F0). Local layout follows the
// landed CPSCEmitterGeneric::WriteBinary model: Spline counted pointer +0x18,
// EmitterType bits 20-21 of word +0x30, and CCardinalSpline control-point
// vector at offsets 0/+4/+8. Algorithm follows the matching spline-base
// implementation: sum squared deltas for consecutive points, then sqrt once.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float Zero_0122dedc; // retail zero constant at 0x0122DEDC

#include "engine/C3DVector.h"
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
            total += (at(i) - at(i + 1)).GetSquaredMagnitude();
        }
    }
    float result = (float)sqrt(total);
    return result;
}
}