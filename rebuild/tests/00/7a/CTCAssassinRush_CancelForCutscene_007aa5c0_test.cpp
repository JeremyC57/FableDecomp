// Behaviour test for CTCAssassinRush::CancelForCutscene (0x007AA5C0).
#include <cstdio>

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer { void* m_pItem; CThing* GetPItem() const { return (CThing*)m_pItem; } };
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
struct CIEngine;
static int g_particles = 0, g_decals = 0;
namespace NParticleEngine {
struct CParticleEmitter {
    int id;
    void ClearAllParticles(CIEngine* engine) { if (!engine) ++g_particles; }
    void ClearAllDecals(CIEngine* engine) { if (!engine) ++g_decals; }
};
}
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)m_pItem; }
};
struct CTCDParticleEmitter { CIntelligentPointerEmitter PEmitter; };
class CTCAssassinRush {
public:
    CBaseIntelligentPointer AssassinTrail;
    CBaseIntelligentPointer AssassinDisplacementTrail;
    void CancelForCutscene()
    {
        if (IsThingAlive(AssassinTrail)) {
            CTCDParticleEmitter* trail = 0;
            AssassinTrail.GetPItem()->GetInterface(&trail, 0x4b);
            NParticleEngine::CParticleEmitter* emitter = trail->PEmitter.GetPItem();
            if (emitter) { emitter->ClearAllParticles(0); emitter->ClearAllDecals(0); }
        }
        if (IsThingAlive(AssassinDisplacementTrail)) {
            CTCDParticleEmitter* trail = 0;
            AssassinDisplacementTrail.GetPItem()->GetInterface(&trail, 0x4b);
            trail->PEmitter.GetPItem();
        }
    }
};

int main()
{
    NParticleEngine::CParticleEmitter emitter = { 1 };
    CTCDParticleEmitter tc; tc.PEmitter.m_pItem = &emitter;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&tc } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[2] = 1UL << 11; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2; thing.m_flags91 = 0;
    CTCAssassinRush r; r.AssassinTrail.m_pItem = &thing; r.AssassinDisplacementTrail.m_pItem = &thing;
    r.CancelForCutscene();
    bool ok = g_particles == 1 && g_decals == 1;
    thing.m_flags91 = 1;                       // dead trail: nothing
    r.CancelForCutscene();
    ok = ok && g_particles == 1;
    thing.m_flags91 = 0; tc.PEmitter.m_pItem = 0;   // no emitter: nothing
    r.CancelForCutscene();
    ok = ok && g_particles == 1 && g_decals == 1;
    r.AssassinTrail.m_pItem = 0; r.AssassinDisplacementTrail.m_pItem = 0;
    r.CancelForCutscene();
    ok = ok && g_particles == 1;
    if (!ok) { std::printf("007aa5c0_TEST FAIL\n"); return 1; }
    std::printf("007aa5c0_TEST PASS\n");
    return 0;
}