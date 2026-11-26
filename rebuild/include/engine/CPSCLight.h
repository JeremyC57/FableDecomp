#if defined(FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE) && !defined(FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS)
#ifndef FABLE_ENGINE_CPSCLIGHT_H
#define FABLE_ENGINE_CPSCLIGHT_H
#include "engine/CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CArray.h"
namespace NParticleEngine {
// Retail opt-in (Codex pass22): original Ego_r TPI0x60784 fields; native size108 (Alloc00AF8020), donor112.
// Final CArray is12 bytes in TLC versus16 in donor; all field offsets and bitfields agree.
class CPSCLight:public CPSCBase {public:
    virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
#ifdef FABLE_RETAIL_CPSCLIGHT_PREPARE_PRIMITIVES_PASS28
    // Ego_r and FableWin virtual signature, retail slot 13 (0x00B19C60).
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
        const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);
    float SmoothFadeValue(float) const;
#endif
    long LightLifeTimer;
    long LightRespawnDelayTimer;
    bool LightIsAlive;
    unsigned long LightPositionParam;
    float LightLifeSecs;
    float LightRespawnDelaySecs;
    float LightStartTime;
    float LightTimelineSecs;
    float LightStartRenderWorldRadius;
    float LightEndRenderWorldRadius;
    long LightAttenuationFactor;
    long LightFadeInEndInteger;
    long LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable:1;
    bool LightUseLifeSecs:1;
    bool LightEnabled:1;
    bool LightRespawns:1;
    bool LightColourFadeEnable:1;
    bool LightUseStartColour:1;
    bool LightUseMidColour:1;
    bool LightUseEndColour:1;
    bool LightUseFadeColour:1;
    bool LightUseTimelineSecs:1;
    bool StartedTimer:1;
    CRGBColour LightStartColour;
    CRGBColour LightMidColour;
    CRGBColour LightEndColour;
    CEnginePrimitiveHandle PrimHandle;
    CArray<C3DVector> LightUserPointsArray;
};
}
#endif
#endif
#ifdef FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS
// Add-only per-TU opt-in (NF462, 2026-11-26, ruling r460d on r451a / r459d): the typed class above without STLport. The typed view's
// CArray<C3DVector> LightUserPointsArray (engine/CArray.h includes stlport/fable_containers.h -> <new>, which adds /DEFAULTLIB:"libcp" to
// the object's .drectve; retail's objects never had it, r450a) keeps its name and offset here as 12 opaque bytes (TLC CArray, see the typed
// view's comment); every other member is the typed view's, in its order. For TUs that only name or allocate the class (no member
// construction, destruction or CArray access). Define it TOGETHER with FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE.
#ifndef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#error "FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS needs FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE"
#endif
#ifdef FABLE_RETAIL_CPSCLIGHT_PREPARE_PRIMITIVES_PASS28
#error "FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS cannot be combined with FABLE_RETAIL_CPSCLIGHT_PREPARE_PRIMITIVES_PASS28 (use the typed view)"
#endif
#ifndef FABLE_ENGINE_CPSCLIGHT_H
#define FABLE_ENGINE_CPSCLIGHT_H
#include <stddef.h>
#include "engine/CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
namespace NParticleEngine {
class CPSCLight:public CPSCBase {public:
    virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
#ifdef FABLE_RETAIL_CPSCLIGHT_ALLOC
    // Opt-in (NF462, ruling r460d; the engine/NParticleEngine_CPSCOrbit.h pattern): the class factory and the members a TU needs to `new`
    // the class. Ego_r.pdb + FableWin.pdb: ??0CPSCLight@NParticleEngine@@QAE@XZ (0x00B1A640, out of line),
    // ?Alloc@CPSCLight@NParticleEngine@@SIPAVCPSCBase@2@XZ (0x00AF8020, public static __fastcall; RegisterPSCs 0x00AF8170 binds it) and the
    // ten overrides of CPSCBase's pure virtuals (DatabaseClone 0x00B1AE70 = vtable 0x012A0624 slot 2, its only retail reference).
    // Declarations only: the virtuals override CPSCBase slots, no new slot, no layout change.
    CPSCLight();
    static CPSCBase* FABLE_FASTCALL Alloc();
    virtual CPSCBase* DatabaseClone() const;
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&,const float);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
#endif
    long LightLifeTimer;
    long LightRespawnDelayTimer;
    bool LightIsAlive;
    unsigned long LightPositionParam;
    float LightLifeSecs;
    float LightRespawnDelaySecs;
    float LightStartTime;
    float LightTimelineSecs;
    float LightStartRenderWorldRadius;
    float LightEndRenderWorldRadius;
    long LightAttenuationFactor;
    long LightFadeInEndInteger;
    long LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable:1;
    bool LightUseLifeSecs:1;
    bool LightEnabled:1;
    bool LightRespawns:1;
    bool LightColourFadeEnable:1;
    bool LightUseStartColour:1;
    bool LightUseMidColour:1;
    bool LightUseEndColour:1;
    bool LightUseFadeColour:1;
    bool LightUseTimelineSecs:1;
    bool StartedTimer:1;
    CRGBColour LightStartColour;
    CRGBColour LightMidColour;
    CRGBColour LightEndColour;
    CEnginePrimitiveHandle PrimHandle;
    unsigned char LightUserPointsArray[0xc];          // CArray<C3DVector> (TLC 12 bytes)
};
}
FABLE_STATIC_ASSERT_AT(127, sizeof(NParticleEngine::CPSCLight) == 0x6c);
FABLE_STATIC_ASSERT_AT(128, offsetof(NParticleEngine::CPSCLight, LightUserPointsArray) == 0x60);
#endif
#elif defined(FABLE_RETAIL_CPSCLIGHT_ALLOC)
#error "FABLE_RETAIL_CPSCLIGHT_ALLOC is declared on the containers-free view: define FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS too"
#endif // FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS
