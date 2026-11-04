// NParticleEngine::CPSCAttractor::~CPSCAttractor() -- the compiler-generated destructor: free the AttractorUserPointsArray block, then the inline CPSCBase / CParticleUpdateParamCallback destructors and ~CBaseClassNonCopyable
// Leaf: ??1CPSCAttractor@NParticleEngine@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B18780, 27 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00B18760 of vtable 0x012A04AC (RTTI
//   .?AVCPSCAttractor@NParticleEngine@@ : CPSCBase : CParticleUpdateParamCallback<KK> : CBaseClassNonCopyable : CBaseClass);
//   Ego_r.pdb and FableWin.pdb both spell ??1CPSCAttractor@NParticleEngine@@UAE@XZ. Ego_r.pdb's only non-trivial member is
//   AttractorUserPointsArray (CArray<C3DVector>) at +0x24 -- the landed ctor 0x00B18020 builds it there. No 0x012A04AC
//   store: the destructor is the implicit one. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: the chunk 699 CPSCSingleSprite 0x00B0A160 model (STLport vector under CArray, implicit CPSCBase chain); the
//   CPSCBase / CParticleUpdateParamCallback destructors are implicit and empty, so the base call is ~CBaseClassNonCopyable.
//   DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"
#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

#pragma pack(push, 1)
class C3DVector {                                       // retail C3DVector is 1-aligned
public:
    float X, Y, Z;
};
#pragma pack(pop)

namespace NParticleEngine {
class CParticleSystem;
template <class T1, class T2> class CParticleUpdateParameters;
template <class T1, class T2> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector& param, unsigned long index, float time, bool flag,
                                const CParticleUpdateParameters<T1, T2>& parameters, unsigned long extra);
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {   // vtable 0x0129F3F4
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool             UpdateStarted : 1;                 // +0x08
    bool             Initialised : 1;                   // +0x08
};
class CPSCAttractor : public CPSCBase {                 // vtable 0x012A04AC, 0x30 bytes in retail
public:
    CPSCAttractor();                                    // 0x00B18020
    bool              AttractorEnabled : 1;             // +0x0c
    bool              AttractorUseParamPosition : 1;    // +0x0c
    long              AttractorInfluenceFallOff;        // +0x10
    unsigned long     AttractorPositionParam;           // +0x14
    unsigned long     AttractorPositionParamName;       // +0x18
    float             AttractorInfluenceRadius;         // +0x1c
    float             AttractorInfluenceForce;          // +0x20
    CArray<C3DVector> AttractorUserPointsArray;         // +0x24
};
} // namespace NParticleEngine

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NParticleEngine::CPSCAttractor* component)
{
    component->NParticleEngine::CPSCAttractor::~CPSCAttractor();
}