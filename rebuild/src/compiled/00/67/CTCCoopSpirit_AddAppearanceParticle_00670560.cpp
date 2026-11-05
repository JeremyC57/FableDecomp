// CTCCoopSpirit::AddAppearanceParticle(const CCharString& particleThingName) -- create the named particle thing at the spirit's position and attach its emitter to the spirit (bone 0x88, scale 1.1)
// Leaf: ?AddAppearanceParticle@CTCCoopSpirit@@QAEXABVCCharString@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x00670560, 141 bytes (`ret 4`). Callees: CThing::GetPos() const @ 0x004C73D0, the
// global Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (`__fastcall`),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 (the empty string 0x0122D70E), CTCDParticleEmitter::AttachToThing
// (CThing*, long, const CCharString&, long, float) @ 0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Shape: the emitter (interface 0x4b: bitset word 2 bit 11, `mov ecx,[eax+28h] ; test ch,8`) comes
// from a `__forceinline` helper around the out-parameter GetInterface whose uninitialised out pointer,
// the lookup key and the empty CCharString (a named local in its own `{}` block) all share the dead
// argument slot (a clear bit reads it back, `mov esi,[esp+0Ch]`); with the out pointer and the string
// as plain locals of the body VC opens a `push ecx` frame word instead.
class CTCBase;
class C3DVector;

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

__forceinline CTCDParticleEmitter* GetParticleEmitter(CThing* thing)
{
    CTCDParticleEmitter* emitter;
    thing->GetTC(TCI_PARTICLE_EMITTER, &emitter);                    // TC_PARTICLE_EMITTER
    return emitter;
}

class CTCCoopSpirit { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void AddAppearanceParticle(const CCharString& particleThingName);
};

void CTCCoopSpirit::AddAppearanceParticle(const CCharString& particleThingName)
{
    CThing* particle = Create(particleThingName, ParentThing->GetPos(), false);
    if (particle)
    {
        CTCDParticleEmitter* emitter = GetParticleEmitter(particle);
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(*ParentThing, 0x88, noBone, 0, 1.1f);
        }
    }
}