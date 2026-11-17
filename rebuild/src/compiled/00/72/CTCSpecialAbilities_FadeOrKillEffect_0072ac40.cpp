// CTCSpecialAbilities::FadeOrKillEffect(CThing* effect) -- if the effect thing exists and is not being removed, move its particle-emitter component (interface 0x4b) to the end of its timeline
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup as `T* tc = 0; if (HasTC) tc = GetTC<T>()` with the ternary entry pick: the zero preload lands in ecx and the component is loaded straight into it
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AC40, 75 bytes. Eight callers pass `pointer.GetPItem()` in ecx (spell / shot-definition
//   code); the null test on ecx makes it a static __fastcall helper (a CThing member would be the same bytes).
// Name-fix (agentNF55, round 7; was the descriptive CTCPhysicalShieldSpell::FadeOutEffect, a manifest label propagated over a folded
//   body): Ego_r.pdb 0x00741F70 / FableWin.pdb 0x0254A8C0 ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z (public static
//   __fastcall), right after the CIntelligentPointer<CThing>& overload (Ego_r 0x00741EF0, retail 0x0072ABD0 landed) and
//   CreateParticleEffect (Ego_r 0x00741E80, retail 0x0072AB70), the PDB source order. Declared by engine/CTCSpecialAbilities.h
//   FULL + TURNCOAT (the CTCPhysicalShieldSpell stand-in class is gone); CThing / the emitter keep this row's local models.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCDParticleEmitter::EnterTimelineEnd @ 0x006E05F0.
// Layout: CThing (retail): +0x20 the interface-presence bitset (type 0x4b = word +0x28, bit 11: `test dh,8`), +0x44 the sorted
//   interface map, +0x91 bit 0 = scheduled for removal. Local model (engine/CThing.h does not exist; the PDB CThing is larger).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x008386E0 family) with the component local
//   ZERO-initialised (`xor ecx,ecx` before the bitset test), the key in the `push ecx` slot, the map a named pointer (esi);
//   the emitter call is made even when the interface is absent (ecx = 0).
// Leaf: ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
#include "fable_compiler.h"
class CTCBase;

#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad54[0x3d];             // +0x4c
    unsigned char m_removeFlags;            // +0x91 bit 0 = scheduled for removal
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
};

class CTCDParticleEmitter { public:
    void EnterTimelineEnd();                // 0x006E05F0
};

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT
#include "engine/CTCSpecialAbilities.h"

void FABLE_FASTCALL CTCSpecialAbilities::FadeOrKillEffect(CThing* effect)
{
    if (effect && !(effect->m_removeFlags & 1)) {
        CTCDParticleEmitter* emitter = 0;
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
}