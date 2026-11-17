// NCombatActionHelpers::CreateEffect(CThingCreatureBase&, long, const C3DVector&, const CCharString*) -- spawn the effect thing at the creature, attach it to the named helper if any, hand its emitter the DIRECTION vector
// Leaf: ?CreateEffect@NCombatActionHelpers@@YIXAAVCThingCreatureBase@@JABVC3DVector@@PBVCCharString@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CVECTORMAP_CTOR
#include "fable_compiler.h"
class CCharString {
public:
    void* PStringData;                          // +0x00
};

#include "engine/C3DVector.h"

class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0: CRC

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// NF55 (2026-11-17): the parameter block is the shared NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>
//   (engine/NParticleEngine_CParticleUpdateParameters.h: CBaseClass + VectorParams / UpdateParams CVectorMaps; Ego_r.pdb / FableWin.pdb
//   ??0?$CParticleUpdateParameters@KK@NParticleEngine@@QAE@XZ 0x006E24D0) and the parameter pushes CArray<pair<unsigned long, C3DVector> >::push_back
//   (Ego_r.pdb 0x0077A190 + FableWin.pdb 0x025F4840, out of line at 0x0076B120; FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE keeps its body visible
//   as the old same-TU stand-in body did). They were the local SVectorParam / CVectorParamVector / CVectorParamMap / CParticleUpdateParameters
//   stand-ins.
#include "engine/NParticleEngine_CParticleUpdateParameters.h"

class CThing;

class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private:
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float flags);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& parameters);   // 0x006E11A0
};


class CThing {
public:
    const C3DVector& GetPos() const;       // 0x004C73D0
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];           // +0x20
    unsigned char   _pad44[0x5c - 0x44];
    void*           PTCDriver;                  // +0x5c (the particle emitter component when kind 0x4b is present)
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};

class CThingCreatureBase : public CThing {
};

namespace NCombatActionHelpers {
void __fastcall CreateEffect(CThingCreatureBase& creature, long effectType, const C3DVector& direction,
                             const CCharString* helperName)
{
    if (!effectType)
        return;
    CThing* effect = CTCDParticleEmitter::Create(effectType, *(&creature.GetPos()), false);
    if (!effect)
        return;
    CTCDParticleEmitter* emitter = 0;
    if (effect->HasTC(0x4b))
        emitter = (CTCDParticleEmitter*)effect->PTCDriver;
    if (helperName)
        emitter->AttachToThing(creature, 12, *helperName, 0, 0);
    unsigned long key = CCRC::Calc(0, "DIRECTION", 9);
    std::pair<unsigned long, C3DVector> directionParam(key, direction);
    NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
    parameters.VectorParams.push_back(directionParam);
    emitter->SetEmitterUpdateParameters(parameters);
}
}