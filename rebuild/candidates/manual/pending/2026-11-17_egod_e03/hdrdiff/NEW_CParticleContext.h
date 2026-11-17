#ifndef FABLE_ENGINE_CPARTICLECONTEXT_H
#define FABLE_ENGINE_CPARTICLECONTEXT_H
// CParticleContext (egod e03, row 0x00B81820): Ego_d.pdb LF_CLASS 0x658DF (field list 0x658DE): base CBaseClassNonCopyable, virtual
// destructor (slot 0, inherited from CBaseClassNonCopyable) and five pure intro virtuals GetGravityVector (vftable offset 4), GetWindVector (8),
// GetDeltaTransform (12), GetMaximumIterations (16), GetMaximumDistanceError (20); sizeof 4. Ego_r.pdb / FableWin.pdb spellings of the
// overrides (UBE: public const). Ego_d marks both constructors and the destructor compiler-generated (method list 0x658D9, `~CParticleContext`
// attrs "public virtual compiler-generated"), so none is declared here: the implicit default constructor is inline (retail
// CEngineSimulationBase::CEngineSimulationBase 0x00B80900 calls CBaseClassNonCopyable's constructor directly; 0x00B80660 is its
// out-of-line COMDAT copy). Declarations only.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
class CMatrix3x4;
class CParticleContext : public CBaseClassNonCopyable {
public:
    virtual C3DVector GetGravityVector() const = 0;                      // slot 1
    virtual C3DVector GetWindVector() const = 0;                         // slot 2
    virtual void GetDeltaTransform(CMatrix3x4& transform) const = 0;     // slot 3
    virtual long GetMaximumIterations() const = 0;                       // slot 4
    virtual float GetMaximumDistanceError() const = 0;                   // slot 5
};
FABLE_STATIC_ASSERT(sizeof(CParticleContext) == 4);
#endif
