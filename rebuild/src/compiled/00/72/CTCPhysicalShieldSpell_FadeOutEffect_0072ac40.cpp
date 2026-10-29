// CTCPhysicalShieldSpell::FadeOutEffect(CThing* effect) -- if the effect thing exists and is not being removed, move its particle-emitter component (interface 0x4b) to the end of its timeline
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup as `T* tc = 0; if (HasTC) tc = GetTC<T>()` with the ternary entry pick: the zero preload lands in ecx and the component is loaded straight into it
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AC40, 75 bytes. Eight callers pass `pointer.GetPItem()` in ecx (spell / shot-definition
//   code); the null test on ecx makes it a static __fastcall helper (a CThing member would be the same bytes).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCDParticleEmitter::EnterTimelineEnd @ 0x006E05F0.
// Layout: CThing (retail): +0x20 the interface-presence bitset (type 0x4b = word +0x28, bit 11: `test dh,8`), +0x44 the sorted
//   interface map, +0x91 bit 0 = scheduled for removal. Local model (engine/CThing.h does not exist; the PDB CThing is larger).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x008386E0 family) with the component local
//   ZERO-initialised (`xor ecx,ecx` before the bitset test), the key in the `push ecx` slot, the map a named pointer (esi);
//   the emitter call is made even when the interface is absent (ecx = 0).
// Leaf: FadeOutEffect
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_removeFlags;            // +0x91 bit 0 = scheduled for removal
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};

class CTCDParticleEmitter { public:
    void EnterTimelineEnd();                // 0x006E05F0
};

class CTCPhysicalShieldSpell { public:
    static void __fastcall FadeOutEffect(CThing* effect);
};

void __fastcall CTCPhysicalShieldSpell::FadeOutEffect(CThing* effect)
{
    if (effect && !(effect->m_removeFlags & 1)) {
        CTCDParticleEmitter* emitter = 0;
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
}