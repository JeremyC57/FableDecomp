// NFire::CFirePoint::Reset() -- kill the live fire and singe effects; when the point was burning, spawn the "Flaming_Hands_Die" effect at the parent's position and attach its particle emitter to the parent's helper point; the point stops burning
// Leaf: Reset
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780700, length 249 bytes (+ int3 padding), plain `ret`.
// Signature from Ego_r.pdb: ?Reset@CFirePoint@NFire@@QAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CThing::GetPos @ 0x004C73D0,
//   the __fastcall effect-thing factory Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (landed callers
//   0x00781F10), CCharString(const char*, long) @ 0x0099EBF0 ("Flaming_Hands_Die" @ 0x0126B18C) / ~CCharString @ 0x0099EAE0,
//   the interface map LowerBound @ 0x0040F020, CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
//   @ 0x006E0BE0.
// Layout = Ego_r.pdb NFire::CFirePoint (44 bytes): HelperName +0, HelperIndex +4, Burning +0x10, PParentThing +0x18,
//   PEffect +0x1c, PSingeEffect +0x24 (CIntelligentPointer<CThing>); retail CThing interface bitset +0x20, map +0x44, dead
//   bit +0x91 bit 0.
// Shape: each effect is fetched three times through GetPItem (null test, dead test, Kill); the name is a named local in its
//   own block; the emitter lookup is `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the preloaded `xor ecx,ecx`) and is used
//   unchecked; GetTC is `__forceinline` with a map pointer local (`add edi,44h`, `this` stays in esi) and the end/hit
//   choice as one ternary (the component lands straight in ecx); the Burning clear sits after the join.
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006E0970 ?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z (was ?Create@@YIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z)
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
 public: static CThing* __fastcall Create(CCharString const&, C3DVector const&, bool); };
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


namespace NFire {
class CFirePoint { public:
    CCharString             HelperName;                 // +0x00
    long                    HelperIndex;                // +0x04
    long                    EffectIndex;                // +0x08
    long                    Resistance;                 // +0x0c
    bool                    Burning;                    // +0x10
    long                    FireDamageAfterFrame;       // +0x14
    CThing*                 PParentThing;               // +0x18
    CIntelligentPointer<CThing> PEffect;                    // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;               // +0x24
    void Reset();
};

void CFirePoint::Reset()
{
    if (PEffect.Get() && !PEffect.Get()->IsDead())
        PEffect.Get()->Kill(true);
    if (PSingeEffect.Get() && !PSingeEffect.Get()->IsDead())
        PSingeEffect.Get()->Kill(true);
    if (Burning) {
        CThing* effect;
        {
            CCharString name("Flaming_Hands_Die", -1);
            effect = CTCDParticleEmitter::Create(name, PParentThing->GetPos(), false);
        }
        if (effect && !effect->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(TCI_PARTICLE_EMITTER))
                emitter = ((CTCDParticleEmitter*)&effect->GetTC(TCI_PARTICLE_EMITTER));
            emitter->AttachToThing(*PParentThing, 0x1c, HelperName, HelperIndex, 0.0f);
        }
    }
    Burning = false;
}
}