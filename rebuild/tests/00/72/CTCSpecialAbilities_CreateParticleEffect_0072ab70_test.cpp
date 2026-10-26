// Behaviour test for 0x0072AB70 CTCSpecialAbilities::CreateParticleEffect: spawns an emitter thing at the position
// (not attached) and, when it exists and is not dead, sets its particle-emitter component (0x4B) to always update.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 (bit 0: dead)
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(interfaceType < found->type))
                it = found;
            else
                it = map->last;
            result = (T*)it->component;
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


static CThing* g_spawn = 0;
static long g_def = 0;
static const C3DVector* g_pos = 0;
static bool g_attach = true;
static int g_always = 0;
static bool g_alwaysValue = false;
CThing* __fastcall CTCDParticleEmitter::Create(long particleDef, const C3DVector& position, bool attach)
{ g_def = particleDef; g_pos = &position; g_attach = attach; return g_spawn; }
void CTCDParticleEmitter::SetAlwaysUpdate(bool v) { ++g_always; g_alwaysValue = v; }

int main()
{
    static CThing effect;
    memset(&effect, 0, sizeof(effect));
    static CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x4B; entries[0].component = (CTCBase*)&emitter;
    entries[1].type = 0x60; entries[1].component = 0;
    effect.m_interfaces.first = entries;
    effect.m_interfaces.last = entries + 2;
    effect.m_presentBits[0x4B >> 5] |= 1UL << (0x4B & 31);
    C3DVector pos;
    memset(&pos, 0, sizeof(pos));

    int ok = 1;
    if (CTCSpecialAbilities::CreateParticleEffect(7, pos) != 0 || g_always != 0) ok = 0;   // nothing spawned
    if (g_def != 7 || g_pos != &pos || g_attach) ok = 0;
    g_spawn = &effect;
    effect.m_flags91 = 1;
    if (CTCSpecialAbilities::CreateParticleEffect(8, pos) != &effect || g_always != 0) ok = 0;   // dead
    effect.m_flags91 = 0;
    if (CTCSpecialAbilities::CreateParticleEffect(9, pos) != &effect || g_always != 1 || !g_alwaysValue) ok = 0;

    printf(ok ? "0072ab70_TEST PASS\n" : "0072ab70_TEST FAIL\n");
    return ok ? 0 : 1;
}