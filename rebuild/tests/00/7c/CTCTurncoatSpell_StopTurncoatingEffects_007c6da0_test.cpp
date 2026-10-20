// Behaviour test for 007c6da0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTurncoatSpell::StopTurncoatingEffects() -- stops the spell's looping sound on the caster's sound player and fades out the two turncoating effect things that are still alive
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
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
    void FadeOutEffect();                       // 0x0072AC40
};

struct CTCSoundPlayer {                         // interface 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};

struct CBaseIntelligentPointer {
    void* __vftable;                            // +0x00
    void* PData;                                // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCTurncoatSpell {
    void*                   __vftable;          // +0x00
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    unsigned char           _pad_0x08[0x48];
    unsigned long           LoopingSound;       // +0x50
    unsigned char           _pad_0x54[0x20];
    CBaseIntelligentPointer TurncoatingEffectOnMe;       // +0x74
    CBaseIntelligentPointer TurncoatingEffectOnTarget;   // +0x7c
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange;   // +0x84
    void StopTurncoatingEffects();
};

void CTCTurncoatSpell::StopTurncoatingEffects()
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10)) {
        sound->StopSample(LoopingSound);
        LoopingSound = 0;
    }
    if (IS_THING_ALIVE(TurncoatingEffectOnMe))
        TurncoatingEffectOnMe.GetPItem()->FadeOutEffect();
    if (IS_THING_ALIVE(TurncoatingEffectOnTarget))
        TurncoatingEffectOnTarget.GetPItem()->FadeOutEffect();
}

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CTCSoundPlayer g_player; static unsigned long g_stopped = 99; static int g_stops;
void CTCSoundPlayer::StopSample(unsigned long sampleID) { if (this == &g_player) ++g_stops; g_stopped = sampleID; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
static CThing* g_faded[4]; static int g_fades;
void CThing::FadeOutEffect() { if (g_fades < 4) g_faded[g_fades] = this; ++g_fades; }

int main()
{
    static CThing caster, onMe, onTarget;
    static CTCTurncoatSpell spell;
    spell.ParentThing = &caster;
    caster.m_presentBits[0] = 1UL << 16;
    g_entries[0].type = 0x10; g_entries[0].component = (CTCBase*)&g_player;
    caster.m_interfaces.first = g_entries; caster.m_interfaces.last = g_entries + 1;
    spell.LoopingSound = 1234;
    spell.TurncoatingEffectOnMe.PData = &onMe;
    spell.TurncoatingEffectOnTarget.PData = &onTarget;
    onTarget.m_flags = 1;                                 // already dead: not faded
    spell.StopTurncoatingEffects();
    if (g_stops != 1 || g_stopped != 1234 || spell.LoopingSound != 0 || g_fades != 1 || g_faded[0] != &onMe)
        { std::printf("007c6da0_TEST FAIL (1)\n"); return 1; }
    // no sound player: the looping sound id is kept
    caster.m_presentBits[0] = 0; spell.LoopingSound = 55; onTarget.m_flags = 0; spell.TurncoatingEffectOnMe.PData = 0;
    spell.StopTurncoatingEffects();
    if (g_stops != 1 || spell.LoopingSound != 55 || g_fades != 2 || g_faded[1] != &onTarget) { std::printf("007c6da0_TEST FAIL (2)\n"); return 1; }
    std::printf("007c6da0_TEST PASS\n");
    return 0;
}