// NParticleEngine::CPSCSplineBase::~CPSCSplineBase() -- compiler-generated destructor: release the spline position-parameter and random-offset data (counted pointers), free the cardinal spline's control-point array, then the base chain down to ~CBaseClassNonCopyable
// Leaf: ??1CPSCSplineBase@NParticleEngine@@UAE@XZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A060, length 111 bytes, tail jump to ~CBaseClassNonCopyable.
// Identity (proven): called by the scalar deleting destructors of NParticleEngine::CPSCSplineBase (0x00B230C0, slot 0 of vtable
//   0x012A0B14) and NParticleEngine::CPSCSpline (0x00B1B490, slot 0 of 0x012A0934; CPSCSpline adds no members to destroy, its
//   implicit dtor is expanded into its ??_G), and tail-jumped to by ~CPSCSingleSprite (0x00B0A160, ??_G 0x00B0A140 = slot 0 of
//   0x0129FB84). RTTI base arrays: CPSCSpline and CPSCSingleSprite derive from CPSCSplineBase, which derives (single chain, all
//   mdisp 0) from CPSCBase : CParticleUpdateParamCallback<unsigned long, unsigned long> : CBaseClassNonCopyable : CBaseClass, so
//   the shared callee of the three is the CPSCSplineBase destructor. Ego_r.pdb ??1CPSCSplineBase@NParticleEngine@@UAE@XZ.
//   Label correction: the catalog's "C3DMeshStripifier::~C3DMeshStripifier" is propagated.
// Callees: operator delete @ 0x00BFE9BC (each count block), free @ 0x00BFEA14 (the control-point vector),
//   CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (tail jump); each count block's __fastcall DeleteFunc.
// Layout (Ego_r.pdb, re-laid with the 12-byte STLport vector; retail 0x34): CPSCBase {vptr, ParticleSystem +0x04, UpdateStarted /
//   Initialised bits +0x08}, SplineAnimSpeed +0x0c, Spline CCardinalSpline +0x10 {ControlPoints CArray<C3DVector> +0x10, Tension
//   +0x1c, EndPointsConnected +0x20}, SplineRandomOffsetData CCountedPointer +0x24, SplinePosParamData CCountedPointer +0x2c.
// Shape: no vtable store anywhere = compiler-generated destructors all the way down (CPSCBase and the callback base add no
//   store either); members in reverse: the two engine/CCountedPointer.h destructors (Data then Info zeroed, one shared zero
//   register), then the STLport 4.6 ~vector (_Destroy of trivially destroyed C3DVectors) + ~_Vector_base null-tested free
//   (chunk 662 text). The implicit ??1 is emitted the way retail's TU emits it: the PDB default constructor
//   ??0CPSCSplineBase@NParticleEngine@@QAE@XZ is defined here (its body is not modelled; only the destructor is graded).
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

#include "engine/CArray.h"

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)
#include "engine/CCountedPointer.h"                    // retail model (8 bytes: Data, Info)

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)


class CCardinalSpline {                                 // Ego_r.pdb, 0x14 bytes in retail
public:
    CArray<C3DVector> ControlPoints;                    // +0x00
    float             Tension;                          // +0x0c
    bool              EndPointsConnected;               // +0x10
};

namespace NParticleEngine {
class CParticleSystem;
template <class T1, class T2> class CParticleUpdateParameters;
template <class T1, class T2> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {   // RTTI base of CPSCBase, no data
public:
    virtual bool GetUpdateParam(C3DVector& param, unsigned long index, float time, bool flag,
                                const CParticleUpdateParameters<T1, T2>& parameters, unsigned long extra);   // FableWin.pdb
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {   // Ego_r.pdb, 12 bytes
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool             UpdateStarted : 1;                 // +0x08
    bool             Initialised : 1;                   // +0x08
};
class CPSCSplineBase : public CPSCBase {                // vtable 0x012A0B14
public:
    class CSplineRandomOffsetData;
    class CSplinePosParamData;
    CPSCSplineBase();
    float                                     SplineAnimSpeed;          // +0x0c
    CCardinalSpline                           Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;       // +0x2c
};

CPSCSplineBase::CPSCSplineBase()
{
}
} // namespace NParticleEngine