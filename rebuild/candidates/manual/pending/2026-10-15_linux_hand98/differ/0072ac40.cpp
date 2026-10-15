// CTCPhysicalShieldSpell::FadeOutEffect(CThing* effect) -- if the effect thing exists and is not being removed, move its particle-emitter component (interface 0x4b) to the end of its timeline
// Grade: DIFFER(ours v retail) -- 75v75: register colouring only -- retail copies the thing to eax ('mov eax,ecx') and keeps the zero-initialised component in ecx ('xor ecx,ecx', 'mov ecx,[eax+4]' straight into this); the lane keeps the thing in ecx and the component in eax (+ 'mov ecx,eax')
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AC40, 75 bytes. Eight callers pass `pointer.GetPItem()` in ecx (spell / shot-definition
//   code); the null test on ecx makes it a static __fastcall helper (a CThing member would be the same bytes).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCDParticleEmitter::EnterTimelineEnd @ 0x006E05F0.
// Layout: CThing (retail): +0x20 the interface-presence bitset (type 0x4b = word +0x28, bit 11: `test dh,8`), +0x44 the sorted
//   interface map, +0x91 bit 0 = scheduled for removal. Local model (engine/CThing.h does not exist; the PDB CThing is larger).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x008386E0 family) with the component local
//   ZERO-initialised (`xor ecx,ecx` before the bitset test), the key in the `push ecx` slot, the map a named pointer (esi);
//   the emitter call is made even when the interface is absent (ecx = 0).
// Leaf: FadeOutEffect
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_removeFlags;            // +0x91 bit 0 = scheduled for removal
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCDParticleEmitter {
    void EnterTimelineEnd();                // 0x006E05F0
};

struct CTCPhysicalShieldSpell {
    static void __fastcall FadeOutEffect(CThing* effect);
};

void __fastcall CTCPhysicalShieldSpell::FadeOutEffect(CThing* effect)
{
    if (effect && !(effect->m_removeFlags & 1)) {
        CTCDParticleEmitter* emitter = 0;
        effect->GetInterface(&emitter, 0x4b);
        emitter->EnterTimelineEnd();
    }
}
