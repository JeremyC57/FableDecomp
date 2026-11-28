#ifndef FABLE_ENGINE_CPSCATTRACTOR_H
#define FABLE_ENGINE_CPSCATTRACTOR_H
#ifdef FABLE_RETAIL_CPSCATTRACTOR_INTERPOLATE_PASS30
#include "engine/CPSCBase.h"
#include "engine/CArray.h"
namespace NParticleEngine {
class CPSCAttractor : public CPSCBase {
public:
    CPSCAttractor();
    virtual ~CPSCAttractor();
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual CPSCBase* DatabaseClone() const;
    virtual void InterpolateTo(const CPSCBase& to, const float interpolate);
    virtual void DatabaseCopy(const CPSCBase& copy_from);
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool&);
    virtual void GetComponentCost(CParticleSystemCost&, const CParticleSystem&, const float, const float);

    // Layout: Ego_d LF_MEMBER makes all eight fields public. Native InterpolateTo
    // reads the two bits at +12, the scalar fields at +16 through +32, and the array at +36.
    bool AttractorEnabled : 1;
    bool AttractorUseParamPosition : 1;
    long AttractorInfluenceFallOff;
    unsigned long AttractorPositionParam;
    unsigned long AttractorPositionParamName;
    float AttractorInfluenceRadius;
    float AttractorInfluenceForce;
    CArray<C3DVector> AttractorUserPointsArray;
};
FABLE_STATIC_ASSERT_AT(177, sizeof(CPSCAttractor) == 48);
}
#endif
#endif
