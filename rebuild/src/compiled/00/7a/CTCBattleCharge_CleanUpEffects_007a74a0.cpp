// CTCBattleCharge::CleanUpEffects() -- kill the live shield effect and re-attach the live left/right feet particle effects to the charging thing
// Leaf: CleanUpEffects
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail 0x007A74A0, 336 bytes, `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CVectorMap<ETCInterfaceType,CTCBase*>::
// LowerBound 0x0040F020 (interface 0x4B, the particle emitter), CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E),
// ~CCharString 0x0099EAE0, CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0.
// Layout = Ego_r.pdb CTCBattleCharge: ParentThing +4, ShieldEffect +0x50, LeftFeetEffect +0x58, RightFeetEffect +0x60
// (CIntelligentPointer<CThing>); CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
// Shape: IS_THING_ALIVE (two GetPItem calls) then a third GetPItem for the use; one emitter local at function scope filled
// by the out-parameter GetInterface (left as is when the component is absent: retail then reuses the previous value or the
// shared key slot); the "" helper name is an implicit-conversion temporary.
class CTCBase;
class CBaseObject;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
class CTCBattleCharge {
public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x50 - 0x08];
    CBaseIntelligentPointer ShieldEffect;               // +0x50
    CBaseIntelligentPointer LeftFeetEffect;             // +0x58
    CBaseIntelligentPointer RightFeetEffect;            // +0x60
    void CleanUpEffects();
};

void CTCBattleCharge::CleanUpEffects()
{
    if (IS_THING_ALIVE(ShieldEffect))
        ((CThing*)ShieldEffect.GetPItem())->Kill(true);
    CThing* owner = ParentThing;
    CTCDParticleEmitter* emitter;
    if (IS_THING_ALIVE(LeftFeetEffect)) {
        ((CThing*)LeftFeetEffect.GetPItem())->GetTC(TCI_PARTICLE_EMITTER, &emitter);
        emitter->AttachToThing(*owner, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(RightFeetEffect)) {
        ((CThing*)RightFeetEffect.GetPItem())->GetTC(TCI_PARTICLE_EMITTER, &emitter);
        emitter->AttachToThing(*owner, 1, "", 0, 0.0f);
    }
}