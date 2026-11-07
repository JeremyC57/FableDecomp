// NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams() -- aim the storm's beams: from the caster's WEAPON_FOCUS_01 dummy, each live main beam emitter gets LIGHTNING_START = the focus and LIGHTNING_END = a point on the storm circle (radius StormRadius, angle i/count plus the fractional StormRotation, at the caster's height); each live end emitter is moved to its circle point; the sky beam runs from the focus to 15 units above it
// Grade: DIFFER(2005v2073) -- 481 difflines (643 for chunk 796's parked source): with the sky-beam block in one do { } while (0) and a visible ~CParticleUpdateParameters body, the overflow copy loops and the dtor now expand in that block as retail does and the call sequence equals retail's, except that the sky block's CVectorMap::LowerBound is still called (retail expands the halving loop there, both beam loops call 0x0040F020); plus frame-slot packing
// Next idea: an inline-able LowerBound for the sky site only: the opt-in inline LowerBound (probe n2, 2053v2073 / 472) expands it in the second beam loop too; a per-site __forceinline lookup helper (chunk 827's 0x0094AB00 lever) on the sky block's GetTC is the untried next step
// Inline-split: one do { } while (0) round the sky-beam block only (lines with `do {` / `} while (0);`) heats that block so VC7.1 expands the overflow copies and ~CParticleUpdateParameters there while both loops call them, as retail does; retail's own construct is unproven
// Leaf: ?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E18D0, 2073 bytes, private thiscall, plain `ret`, frame 0xB0, no EH frame.
// Identity (proven): Ego_r.pdb ?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ; the body walks
//   LightningBeamParticleEmitters (+0x74) and LightningBeamEndParticleEmitters (+0x80), then PSkyBeam (+0x8c), reading StormRotation
//   (+0xbc) / StormRadius (+0xc0) (Ego_r - 0xc after the two retail 12-byte vectors), like the landed CreateLightningBeams 0x008DDAD0.
// Callees: CThing::GetPos 0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0, C3DVector::RotateXY 0x00A14040,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CBaseClass()
//   0x0099A2D0 / ~ 0x0099A2E0, vector<pair<unsigned long, C3DVector> >::_M_insert_overflow 0x0076ADA0, the two __fastcall std::sort
//   instantiations 0x0076C000 / 0x0076C1A0, CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0 / SetEmitterPosition 0x006E0500,
//   ~CParticleUpdateParameters 0x006E2500, malloc / free; the CRC table 0x0129A168 (the keys are CRCs of the literals, inline).
// Model: the landed 0x00767DA0 UpdateParticleAttachment CParticleUpdateParameters model; hdr/ opt-ins FABLE_RETAIL_CVECTORMAP_INSERT
//   (CVectorMap::Insert / Update) and FABLE_RETAIL_STLP_SORT_FASTCALL.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CBaseClass.h"
#include "engine/CIntelligentPointer.h"

extern const unsigned long g_crcTable_0129a168[256];    // 0x0129A168
__forceinline unsigned long CalcCRC(const char* text)
{
    unsigned long crc = 0;
    unsigned long length = strlen(text);
    for (unsigned long i = 0; i < length; ++i)
        crc = g_crcTable_0129a168[((const unsigned char*)text)[i] ^ (crc & 0xff)] ^ (crc >> 8);
    return crc;
}

namespace NParticleEngine {
template <class K, class V> class CParticleUpdateParamCallback;
template <class K, class V> class CParticleUpdateParamDesc : public CBaseClass {
public:
    CParticleUpdateParamCallback<K, V>* Callback;            // +0x04
    unsigned long Index;                                     // +0x08
};
template <class K, class V> class CParticleUpdateParameters : public CBaseClass {   // vtable 0x0125FFDC
public:
    template <class T> class CParticleUpdateCCompareVector { public:
        bool operator()(const std::pair<T, C3DVector>& a, const std::pair<T, C3DVector>& b) const { return a.first < b.first; }
    };
    template <class T> class CParticleUpdateCCompareParamDesc { public:
        bool operator()(const std::pair<T, CParticleUpdateParamDesc<K, V> >& a, const std::pair<T, CParticleUpdateParamDesc<K, V> >& b) const { return a.first < b.first; }
    };
    CVectorMap<K, C3DVector, CParticleUpdateCCompareVector<K> > VectorParams;                                 // +0x04
    CVectorMap<K, CParticleUpdateParamDesc<K, V>, CParticleUpdateCCompareParamDesc<K> > UpdateParams;        // +0x14
    CParticleUpdateParameters() { }
    virtual ~CParticleUpdateParameters() { }                 // 0x006E2500
    void Update() { VectorParams.Update(); UpdateParams.Update(); }
};
}
class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    void SetEmitterPosition(const C3DVector& position);     // 0x006E0500
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& parameters);   // 0x006E11A0
};

namespace NCreatureMode {
class CModeThunderLightningStorm : public CTCCreatureModeManager::CModeBase {
private:
    void UpdateLightningBeams();
    unsigned char _pad_0x1c[0x74 - 0x1c];
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;      // +0x74
    std::vector<CIntelligentPointer<CThing> > LightningBeamEndParticleEmitters;   // +0x80
    CIntelligentPointer<CThing> PSkyBeam;                                         // +0x8c
    CIntelligentPointer<CThing> PCreatorEffect;                                   // +0x94
    CIntelligentPointer<CThing> PHandGlow;                                        // +0x9c
    void* PLightningStormDef;                                                     // +0xa4
    std::vector<CIntelligentPointer<CThing> > HitThings;                          // +0xa8
    bool MainBeamsCreated;                                                        // +0xb4
    long Timer;                                                                   // +0xb8
    float StormRotation;                                                          // +0xbc
    float StormRadius;                                                            // +0xc0
    unsigned long LoopingSound;                                                   // +0xc4
};
}
using namespace NCreatureMode;

void CModeThunderLightningStorm::UpdateLightningBeams()
{
    long count = LightningBeamParticleEmitters.size();
    float step = 1.0f / count;
    float rotation = StormRotation;
    float offsetAngle = rotation - (long)rotation;
    float radius = StormRadius;
    C3DVector focus = ParentThing.GetPos();
    CRightHandedSet frame;
    ParentThing.GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, focus, frame, 1.0f);
    long i;
    for (i = 0; i < count; ++i) {
        const CIntelligentPointer<CThing>& beam = LightningBeamParticleEmitters[i];
        if (beam.Get() && !beam->IsDead()) {
            C3DVector direction(0.0f, 1.0f, 0.0f);
            direction.RotateXY(i * step + offsetAngle);
            C3DVector end = direction * radius + ParentThing.GetPos();
            end.Z = ParentThing.GetPos().Z;
            CTCDParticleEmitter* emitter = 0;
            LightningBeamParticleEmitters[i]->GetTC(&emitter);
            std::pair<unsigned long, C3DVector> startParam(CalcCRC("LIGHTNING_START"), focus);
            std::pair<unsigned long, C3DVector> endParam(CalcCRC("LIGHTNING_END"), end);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
            parameters.VectorParams.Insert(startParam);
            parameters.VectorParams.Insert(endParam);
            parameters.Update();
            emitter->SetEmitterUpdateParameters(parameters);
        }
    }
    long endCount = LightningBeamEndParticleEmitters.size();
    for (i = 0; i < endCount; ++i) {
        const CIntelligentPointer<CThing>& beam = LightningBeamEndParticleEmitters[i];
        if (beam.Get() && !beam->IsDead()) {
            C3DVector direction(0.0f, 1.0f, 0.0f);
            direction.RotateXY(i * step + offsetAngle);
            C3DVector end = direction * radius + ParentThing.GetPos();
            end.Z = ParentThing.GetPos().Z;
            CTCDParticleEmitter* emitter = 0;
            LightningBeamEndParticleEmitters[i]->GetTC(&emitter);
            emitter->SetEmitterPosition(end);
        }
    }
    do {
    if (PSkyBeam.Get() && !PSkyBeam->IsDead()) {
        CTCDParticleEmitter* emitter = 0;
        PSkyBeam->GetTC(&emitter);
        C3DVector top = focus;
        top.Z += 15.0f;
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
        parameters.VectorParams.Insert(std::pair<unsigned long, C3DVector>(CalcCRC("LIGHTNING_START"), focus));
        parameters.VectorParams.Insert(std::pair<unsigned long, C3DVector>(CalcCRC("LIGHTNING_END"), top));
        parameters.Update();
        emitter->SetEmitterUpdateParameters(parameters);
    }
    } while (0);
}
