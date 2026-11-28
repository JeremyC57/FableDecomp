#ifndef FABLE_ENGINE_CPSCSPLINE_H
#define FABLE_ENGINE_CPSCSPLINE_H

#include "engine/CPSCSplineBase.h"

namespace NParticleEngine {

// Ego_r and FableWin publish this derived layout and virtual override set.
// The retail CPSCSplineBase embeds a 12-byte spline array, four bytes shorter.
class CPSCSpline : public CPSCSplineBase {
public:
    CPSCSpline();
    virtual CPSCBase* DatabaseClone() const;
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&, const float);
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual bool GetUpdateParam(C3DVector&, unsigned long, float, bool,
        const CParticleUpdateParameters<unsigned long,unsigned long>&, unsigned long);
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&);
    virtual void ReadBinary(CDataInputStream&);
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,
                                         const unsigned long&);
    virtual void GetComponentCost(CParticleSystemCost&, const CParticleSystem&, const float, const float);

private:
    float SplineCurrentPosition;
    bool ReverseSpline;
    bool SplineBounce;
    bool ScaleSplineSpeed;
    float SplineAnimSpeed;
};

}
FABLE_STATIC_ASSERT_AT(38, sizeof(NParticleEngine::CPSCSpline)==64);
#endif
