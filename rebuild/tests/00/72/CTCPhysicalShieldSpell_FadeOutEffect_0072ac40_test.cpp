// Behaviour test for 0072ac40: body copied from src/0072ac40.cpp (CTCPhysicalShieldSpell::FadeOutEffect), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x45];
    unsigned char m_removeFlags;
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
static int g_endCalls = 0; static void* g_endThis = 0;
class CTCDParticleEmitter { public:
    void EnterTimelineEnd() { ++g_endCalls; g_endThis = this; }
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
int main()
{
    CThing effect; memset(&effect, 0, sizeof(effect)); CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x4b, (CTCBase*)&emitter } };
    effect.m_interfaces.first = entries; effect.m_interfaces.last = entries + 2;
    effect.m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
    // 1. null effect: nothing
    CTCPhysicalShieldSpell::FadeOutEffect(0); CHECK(g_endCalls == 0);
    // 2. effect scheduled for removal: nothing
    effect.m_removeFlags = 1; CTCPhysicalShieldSpell::FadeOutEffect(&effect); CHECK(g_endCalls == 0);
    // 3. live effect with the emitter component: EnterTimelineEnd on it
    effect.m_removeFlags = 0; CTCPhysicalShieldSpell::FadeOutEffect(&effect); CHECK(g_endCalls == 1); CHECK(g_endThis == &emitter);
    // 4. other removal bits do not block
    effect.m_removeFlags = 2; CTCPhysicalShieldSpell::FadeOutEffect(&effect); CHECK(g_endCalls == 2);
    if (fails == 0) printf("0072ac40_TEST PASS\n"); else printf("0072ac40_TEST FAIL\n");
    return fails;
}