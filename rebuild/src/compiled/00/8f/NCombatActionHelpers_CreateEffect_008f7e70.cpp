// NCombatActionHelpers::CreateEffect(CThingCreatureBase&, long, const C3DVector&, const CCharString*) -- spawn the effect thing at the creature, attach it to the named helper if any, hand its emitter the DIRECTION vector
// Leaf: ?CreateEffect@NCombatActionHelpers@@YIXAAVCThingCreatureBase@@
// Retail 0x008F7E70, 0xD4 bytes, __fastcall (creature in ecx, effect type in edx; direction and the optional helper
//   name on the stack, `ret 8`). Manifest name matches the FableWin PDB name set
//   (`?CreateEffect@NCombatActionHelpers@@YIXAAVCThingCreatureBase@@JABVC3DVector@@PBVCCharString@@@Z`).
// Extra callees: CThing::GetPos @ 0x004C73D0 (spawn at the creature), CTCDParticleEmitter::AttachToThing @
//   0x006E0BE0 (creature, attach type 12 = helper point, *helperName, 0, 0) when a helper name is given.
// Callees: CreateEffectThing __fastcall @ 0x006E0880 (type, position, false), CRC CCRC::Calc @ 0x004014A0 ("DIRECTION" @
//   0x01272FC0, 9 bytes), CBaseClass ctor @ 0x0099A2D0, vector<pair<unsigned long, C3DVector>>::push_back @ 0x0076B120,
//   CTCDParticleEmitter::SetEmitterUpdateParameters @ 0x006E11A0, ~CParticleUpdateParameters @ 0x006E2500.
// Layout: the particle emitter component is the thing's PTCDriver +0x5C when component kind 0x4B is present (bit 11 of
//   the presence word +0x28); CParticleUpdateParameters<unsigned long, unsigned long> (vtable 0x0125FFDC, RTTI
//   .?AV?$CParticleUpdateParameters@KK@NParticleEngine@@) = CBaseClass vptr + VectorParams CVectorMap +0x04 (Dirty +0x11)
//   + UpdateParams +0x14 (Dirty +0x21), the model of the landed CTCShotDefinitionBase::InitAttachedEffects 0x007C25A0.
// Shape: CBaseClass must be POLYMORPHIC (its vptr at +0): with a non-polymorphic empty base VC7.1 puts the base
//   subobject at +4 and passes `lea ecx,[params+4]` to its constructor (182v182 / 212v212). The key/vector pair is built
//   by value (the vector copied through integer registers: C3DVector has no user copy ctor here), then the parameters
//   object, push_back, the emitter call and the out-of-line destructor.
class CCharString {
public:
    void* PStringData;                          // +0x00
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
};

class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0: CRC

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
struct SVectorParam {                           // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct SUpdateParam;
struct CVectorParamVector {                     // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    __forceinline CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void push_back(const SVectorParam& value);  // 0x0076B120
};
struct CUpdateParamVector {
    SUpdateParam* _M_start;
    SUpdateParam* _M_finish;
    SUpdateParam* _M_end_of_storage;
    __forceinline CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap {                        // CVectorMap<unsigned long, C3DVector, ...>
    CVectorParamVector Vector;                  // +0x00
    CCompare           Compare;                 // +0x0c
    bool               Dirty;                   // +0x0d
    __forceinline CVectorParamMap() : Dirty(false) {}
};
struct CUpdateParamMap {                        // CVectorMap<unsigned long, CParticleUpdateParamDesc, ...>
    CUpdateParamVector Vector;                  // +0x00
    CCompare           Compare;                 // +0x0c
    bool               Dirty;                   // +0x0d
    __forceinline CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBaseClass { // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;               // +0x04
    CUpdateParamMap UpdateParams;               // +0x14
    __forceinline CParticleUpdateParameters() {}
    virtual ~CParticleUpdateParameters();       // 0x006E2500
};
}

class CThing;

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float flags);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CThing {
public:
    const C3DVector& GetPos() const;       // 0x004C73D0
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];           // +0x20
    unsigned char   _pad44[0x5c - 0x44];
    void*           PTCDriver;                  // +0x5c (the particle emitter component when kind 0x4b is present)
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};

class CThingCreatureBase : public CThing {
};

namespace NCombatActionHelpers {
void __fastcall CreateEffect(CThingCreatureBase& creature, long effectType, const C3DVector& direction,
                             const CCharString* helperName)
{
    if (!effectType)
        return;
    CThing* effect = CreateEffectThing(effectType, &creature.GetPos(), false);
    if (!effect)
        return;
    CTCDParticleEmitter* emitter = 0;
    if (effect->HasTC(0x4b))
        emitter = (CTCDParticleEmitter*)effect->PTCDriver;
    if (helperName)
        emitter->AttachToThing(creature, 12, *helperName, 0, 0);
    unsigned long key = CCRC::Calc(0, "DIRECTION", 9);
    NParticleEngine::SVectorParam directionParam(key, direction);
    NParticleEngine::CParticleUpdateParameters parameters;
    parameters.VectorParams.Vector.push_back(directionParam);
    emitter->SetEmitterUpdateParameters(parameters);
}
}