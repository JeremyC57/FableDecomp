// NCreatureMode::CModeDragonFireBeam::KillBeamEffect() -- clear BeamActive, then fade or kill the beam weapon effect (the inlined CTCSpecialAbilities::FadeOrKillEffect body on PBeamWeaponEffect)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DBA90, length 111 bytes, plain `ret`.
// Identity: Ego_r.pdb CModeDragonFireBeam (0x94 bytes): PBeamWeaponEffect CIntelligentPointer<CThing> at +0x84,
//   BeamActive bool at +0x90 -- exactly the offsets used here; the PDB lists KillBeamEffect beside OnDeactivate, and
//   the only caller of this body is CModeDragonFireBeam::OnDeactivate (0x008DBB03). The catalog label
//   "CCreatureAction_CastWillReclaimUndeadFollower::PerformAction" is propagated. No engine header exists.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020,
//          CTCDParticleEmitter::FadeOrKill @ 0x006E05F0 (manifest label "EnterTimelineEnd" is propagated).
// Shape: the member's address is taken once (esi), the bool store is scheduled right after it; then the
//   FadeOrKillEffect body of 0x0072ABD0 (IS_THING_ALIVE, a null-default interface 0x4b lookup into edi, the emitter
//   call made even on null).
// Leaf: KillBeamEffect
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    template <class T> __forceinline void GetTC(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCDParticleEmitter { public:                // interface type 0x4b component
    void FadeOrKill();                      // 0x006E05F0
};


namespace NCreatureMode {
struct CModeDragonFireBeam {
    unsigned char _pad00[0x80];
    long Timer;                                         // +0x80
    CIntelligentPointer<CThing> PBeamWeaponEffect;      // +0x84
    long LastExplodeFrame;                              // +0x8c
    bool BeamActive;                                    // +0x90
    void KillBeamEffect();
};

void CModeDragonFireBeam::KillBeamEffect()
{
    BeamActive = false;
    CIntelligentPointer<CThing>& effect = PBeamWeaponEffect;
    if (IS_THING_ALIVE(effect)) {
        CTCDParticleEmitter* emitter = 0;
        effect.GetPItem()->GetTC(&emitter, 0x4b);
        emitter->FadeOrKill();
    }
}
}