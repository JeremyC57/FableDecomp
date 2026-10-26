// Behaviour test for CTCBulletTime::ImmediateCancel (0x0079D020).
#include <cstdio>

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static long g_removedTC = 0;
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    void RemoveTC(long t) { g_removedTC = t; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static int g_kills = 0;
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned short Flags40 : 5; unsigned short Dead : 1; unsigned short Flags40High : 10;
    bool IsDead() const { return Dead; }
    void Kill() { ++g_kills; }
};
}
struct CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    NParticleEngine::CParticleEmitter* GetPItem() const { return m_pItem; }
    void SetPItem(NParticleEngine::CParticleEmitter* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { T* item = o.GetPItem(); SetPItem(item); return *this; }
};
static long g_filterMode = -1; static void* g_filterKey = 0; static void* g_slowKey = 0; static unsigned long g_stopped = 0;
struct CEnvironmentSpellEffects { void RemoveFilter_006b83f0(long mode, void* key) { g_filterMode = mode; g_filterKey = key; } };
class CEnvironment { public: CEnvironmentSpellEffects* SpellEffects; };
class CBulletTimeManager { public: void RemoveBulletTimeSlowdown(void* key) { g_slowKey = key; } };
class CWorld { public:
    CEnvironment* PEnvironment; CBulletTimeManager* PBulletTimeManager;
    void RemoveBulletTimeSlowdown(void* key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
struct CTCSoundEmitter { void StopSample(unsigned long id) { g_stopped = id; } };
static CWorld* g_world = 0;
class CTCBulletTime { public:
    CThing* ParentThing; CIntelligentPointer<NParticleEngine::CParticleEmitter> LoopingEffect;
    unsigned long LoopSound; void* ScreenFilterKey; bool Pausable; bool Paused; bool CutSceneCancel;
    CTCBulletTime() : LoopingEffect(0) {}
    CWorld* GetWorld() { return g_world; }
    void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetInterface(&emitter, 0x10)) emitter->StopSample(sound);
    }
    void ImmediateCancel()
    {
        GetWorld()->RemoveBulletTimeSlowdown(this);
        StopSound(LoopSound);
        CWorld* world = GetWorld();
        world->PEnvironment->SpellEffects->RemoveFilter_006b83f0(0, ScreenFilterKey);
        CIntelligentPointer<NParticleEngine::CParticleEmitter>& effect = LoopingEffect;
        if (effect.GetPItem() && !effect.GetPItem()->IsDead()) {
            effect.GetPItem()->Kill();
            effect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
        }
        CutSceneCancel = true;
        Pausable = false;
        ParentThing->RemoveTC(0x92);
    }
};

int main()
{
    CEnvironmentSpellEffects fx; CEnvironment env; env.SpellEffects = &fx;
    CBulletTimeManager btm; CWorld world; world.PEnvironment = &env; world.PBulletTimeManager = &btm; g_world = &world;
    CTCSoundEmitter snd;
    CInterfaceMap::SEntry entries[1] = { { 0x10, (CTCBaseInterface*)&snd } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0] = 1UL << 16; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    NParticleEngine::CParticleEmitter emitter; emitter.Flags40 = 0; emitter.Dead = 0; emitter.Flags40High = 0;
    CTCBulletTime bt; bt.ParentThing = &thing; bt.LoopingEffect.m_pItem = &emitter; bt.LoopSound = 77;
    int key = 0; bt.ScreenFilterKey = &key; bt.Pausable = true; bt.Paused = false; bt.CutSceneCancel = false;
    bt.ImmediateCancel();
    bool ok = g_slowKey == &bt && g_stopped == 77 && g_filterMode == 0 && g_filterKey == &key && g_kills == 1
        && bt.LoopingEffect.GetPItem() == 0 && bt.CutSceneCancel && !bt.Pausable && g_removedTC == 0x92;
    emitter.Dead = 1; bt.LoopingEffect.m_pItem = &emitter;
    bt.ImmediateCancel();
    ok = ok && g_kills == 1 && bt.LoopingEffect.GetPItem() == &emitter;
    if (!ok) { std::printf("0079d020_TEST FAIL\n"); return 1; }
    std::printf("0079d020_TEST PASS\n");
    return 0;
}