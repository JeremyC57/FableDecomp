// Behaviour test for 0x008DBA90 NCreatureMode::CModeDragonFireBeam::KillBeamEffect: BeamActive is cleared; for a live
// effect thing the particle-emitter component (interface 0x4b) found through the vector map is told to fade/kill;
// a null or dead effect makes no call.
#include <cstdio>

class CTCBase { public: int calls; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    template <class T> void GetTC(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
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
class CTCDParticleEmitter : public CTCBase { public: void FadeOrKill() { ++calls; } };
struct CModeDragonFireBeam {
    CIntelligentPointer<CThing> PBeamWeaponEffect;
    bool BeamActive;
    void KillBeamEffect()
    {
        BeamActive = false;
        CIntelligentPointer<CThing>& effect = PBeamWeaponEffect;
        if (IS_THING_ALIVE(effect)) {
            CTCDParticleEmitter* emitter = 0;
            effect.GetPItem()->GetTC(&emitter, 0x4b);
            emitter->FadeOrKill();
        }
    }
};

int main()
{
    CTCDParticleEmitter emitter; emitter.calls = 0;
    CTCBase other = { 0 };
    CInterfaceMap::SEntry entries[3] = { { 0x10, &other }, { 0x4b, &emitter }, { 0x60, &other } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x4b >> 5] |= 1UL << (0x4b & 31);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3; thing.m_flags91 = 0;
    CModeDragonFireBeam mode; mode.PBeamWeaponEffect.m_pItem = &thing; mode.BeamActive = true;
    mode.KillBeamEffect();
    bool ok = !mode.BeamActive && emitter.calls == 1;
    thing.m_flags91 = 1; mode.BeamActive = true; mode.KillBeamEffect();          // dead: no call
    ok = ok && !mode.BeamActive && emitter.calls == 1;
    mode.PBeamWeaponEffect.m_pItem = 0; mode.KillBeamEffect();                   // null: no call
    ok = ok && emitter.calls == 1;
    if (!ok) { std::printf("008dba90_TEST FAIL\n"); return 1; }
    std::printf("008dba90_TEST PASS\n");
    return 0;
}