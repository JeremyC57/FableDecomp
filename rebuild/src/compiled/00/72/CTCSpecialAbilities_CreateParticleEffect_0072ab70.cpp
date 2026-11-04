// CTCSpecialAbilities::CreateParticleEffect(long particleDef, const C3DVector& position) -- spawn a particle-emitter thing at the position and, when it is alive, mark its emitter component always-updating; return the new thing
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses
// Leaf: ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AB70, 88 bytes (96 with padding), `ret` (static __fastcall).
// Label: catalog "CreateParticleEffect" (module _global); Ego_r.pdb name set has
//   ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (static, __fastcall); ~74 callers in
//   the 0x0078C000..0x0079C000 spell components.
// Callees: CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880 (static __fastcall; ecx/edx passed
//   straight through, `push 0`), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//   CTCDParticleEmitter::SetAlwaysUpdate(bool) @ 0x006E0590 (landed).
// Layout: CThing interface-presence bitset +0x20 (interface 0x4B = bit 11 of the word at +0x28: `test ah,8`),
//   interface map +0x44 (first/last), dead flag bit 0 of +0x91.
// Shape: the emitter lookup is GetTC with a null default (`xor ecx,ecx` before the presence test); the component is
//   used unconditionally (ecx may be null on the call, as in retail).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 (bit 0: dead)
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* found = map->LowerBound((const ETCInterfaceType&)type);
            CInterfaceMap::value_type* it;
            if (found != map->end() && !(interfaceType < found->first))
                it = found;
            else
                it = map->end();
            result = (T*)it->second;
        }
        return result;
    }
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long particleDef, const C3DVector& position, bool attach);   // 0x006E0880
    void SetAlwaysUpdate(bool alwaysUpdate);                                                    // 0x006E0590
};
class CTCSpecialAbilities { public:
    static CThing* __fastcall CreateParticleEffect(long particleDef, const C3DVector& position);
};

CThing* __fastcall CTCSpecialAbilities::CreateParticleEffect(long particleDef, const C3DVector& position)
{
    CThing* effect = CTCDParticleEmitter::Create(particleDef, position, false);
    if (effect && !(effect->m_flags91 & 1))
        effect->GetTC<CTCDParticleEmitter>(0x4B)->SetAlwaysUpdate(true);
    return effect;
}