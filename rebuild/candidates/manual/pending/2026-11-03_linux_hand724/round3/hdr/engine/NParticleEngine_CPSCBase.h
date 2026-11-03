#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPSCBASE_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPSCBASE_H
// New header (chunk 724 round 3): NParticleEngine::CPSCBase, the particle-system component root.
// Layout (Ego_r TPI CPSCBase, size 0xC): vptr +0 (from CParticleUpdateParamCallback<K,V>, whose ??1 is UAE in FableWin.pdb),
//   ParticleSystem +4, UpdateStarted / Initialised bits +8 (the landed CPSCSpline / CPSCDecalRenderer rows).
// Virtual slots (retail vtables 0x012A09BC CPSCDecalRenderer and 0x012A0934 CPSCSpline read from Fable.exe; names from the
//   FableWin.pdb ?<name>@CPSCBase@NParticleEngine@@ bodies, which sit in slot order 0x02F10210..0x02F103C0, and the CPSCSpline /
//   CPSCDecalRenderer overrides): 0 dtor, 1 CParticleUpdateParamCallback::GetUpdateParam, 2 DatabaseClone, 3 GetClassTypeName,
//   4 GetComponentType (decal 0x00B1FF30 `mov eax,2`), 5 GetComponentSubType (0x00B1FF40 `mov eax,9`), 6 GetGUIName, 7 OnCreate,
//   8 StartUpdate, 9 Update, 10 CollectUpdateParameters, 11 TransferUpdateParametersToDialog, 12 UpdateAddParticle,
//   13 PreparePrimitives (decal 0x00B1F200), 14 InterpolateTo (0x00B1BDA0), 15 ReadBinary (0x00B1C880), 16 WriteBinary (0x00B1BA10),
//   17 DatabaseSerialise (0x00B1E380, spline 0x00B1B2E0), 18 DatabaseCopy (0x00B1B810), 19 AddToBoundingSphere, 20 GetComponentCost,
//   21 GetParticleLifeSecs, 22 GetNumberOfParticles, 23 GetDecalLifeSecs (decal 0x00B1FF70 `*out = DecalLifeSecs ; mov al,1`),
//   24 GetNumberOfDecals, 25 EnterTimelineEnd, 26 GetParticleCount, 27 GetDecalCount, 28 Initialise, 29 ClearAllPrimitives
//   (decal 0x00B1DBE0, called through [vtbl+74h] by PreparePrimitives), 30 GetUserPoint, 31 GetApproximateSplineLength.
#include "rebuild_abi.h"
class C3DVector;
class C3DBoundingSphere;
class CCharString;
class CDataInputStream;
class CDataOutputStream;
class CEnginePrimitiveHandle;
class CEnginePrimitivePrepareInfo;
class CIEngine;
class CPersistContext;
class CSymbolMap;
namespace NParticleEngine {
class CParticleSystem;
class CParticleSystemCost;
class CEmitterDatabaseSerialiseInfo;
template <class K, class V> class CParticleUpdateParameters;
enum EPSCType;
enum EPSCSubType;
template <class K, class V> class CParticleUpdateParamCallback {
public:
    CParticleUpdateParamCallback();
    virtual ~CParticleUpdateParamCallback();
    virtual bool GetUpdateParam(C3DVector&, K, float, bool, const CParticleUpdateParameters<K, V>&, V);
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CPSCBase();
    virtual ~CPSCBase();
    virtual CPSCBase* DatabaseClone() const = 0;
    virtual CCharString GetClassTypeName() const = 0;
    virtual EPSCType GetComponentType() const = 0;
    virtual EPSCSubType GetComponentSubType() const = 0;
    virtual CCharString GetGUIName() const = 0;
    virtual void OnCreate();
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&);
    virtual void Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool&);
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&, const unsigned long&);
    virtual void TransferUpdateParametersToDialog(const CSymbolMap&);
    virtual void UpdateAddParticle(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, C3DVector, C3DVector, float);
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
        const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
    virtual void ReadBinary(CDataInputStream&) = 0;
    virtual void WriteBinary(CDataOutputStream&) const = 0;
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);
    virtual void DatabaseCopy(const CPSCBase&) = 0;
    virtual bool AddToBoundingSphere(C3DBoundingSphere&, float, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&);
    virtual void GetComponentCost(CParticleSystemCost&, const CParticleSystem&, float, float) = 0;
    virtual bool GetParticleLifeSecs(float&);
    virtual bool GetNumberOfParticles(float&, float);
    virtual bool GetDecalLifeSecs(float&);
    virtual bool GetNumberOfDecals(float&, float);
    virtual void EnterTimelineEnd();
    virtual long GetParticleCount();
    virtual long GetDecalCount();
    virtual bool Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);
    virtual void ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&);
    virtual bool GetUserPoint(C3DVector&) const;
    virtual float GetApproximateSplineLength();
protected:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
};
} // namespace NParticleEngine
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCBase) == 0xc);
#endif
