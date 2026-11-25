// CTCCoopSpirit::SetAppearanceParticle(const CCharString& particleThingName) -- kill the current appearance particle when it is still alive, create the named particle thing at the spirit's position as the new one and, when it lives, attach its emitter to the spirit (bone 0x88, scale 1.1)
// Leaf: ?SetAppearanceParticle@CTCCoopSpirit@@AAEXABVCCharString@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006705F0, 282 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x7) / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / dtor @
//   0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill(bool) @ 0x004C9B80, CThing::GetPos @
//   0x004C73D0, the global Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (`__fastcall`),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x4b), CCharString(const char*, long) @
//   0x0099EBF0 ("" @ 0x0122D70E), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @
//   0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CTCCoopSpirit: ParentThing +0x04, the appearance particle (CIntelligentPointer<CThing>) +0x2c.
// Shape: the landed AddAppearanceParticle 0x00670560 attach block (the `__forceinline` GetParticleEmitter helper with an
//   unset out pointer, the key and the empty bone name sharing the dead argument slot); both liveness tests are the inline
//   IS_THING_ALIVE (two GetPItem calls); the new pointer is assigned through a CIntelligentPointer<CThing> temporary (inline
//   converting ctor = base ctor + vtable + SetPItem, inline operator= = SetPItem(other.GetPItem()), out-of-line base dtor).
#include "fable_compiler.h"
class CTCBase;
class C3DVector;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006E0970 ?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z (was ?Create@@YIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z)
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
 public: static CThing* __fastcall Create(CCharString const&, C3DVector const&, bool); };
FABLE_FORCEINLINE CTCDParticleEmitter* GetParticleEmitter(CThing* thing)
{
    CTCDParticleEmitter* emitter;
    thing->GetTC(TCI_PARTICLE_EMITTER, &emitter);                    // TC_PARTICLE_EMITTER
    return emitter;
}
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CTCCoopSpirit { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x2c - 0x08];
    CIntelligentPointer<CThing> PAppearanceParticle;                      // +0x2c
    private:
    void SetAppearanceParticle(const CCharString& particleThingName);
    public:
};

void CTCCoopSpirit::SetAppearanceParticle(const CCharString& particleThingName)
{
    if (IS_THING_ALIVE(PAppearanceParticle))
        PAppearanceParticle.Get()->Kill(true);
    PAppearanceParticle = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(particleThingName, ParentThing->GetPos(), false));
    if (IS_THING_ALIVE(PAppearanceParticle)) {
        CTCDParticleEmitter* emitter = GetParticleEmitter(PAppearanceParticle.Get());
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(*ParentThing, 0x88, noBone, 0, 1.1f);
        }
    }
}