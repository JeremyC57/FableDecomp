// Behaviour test for 007c7560 CTCTurncoatSpell::FinalCleanup: fades the out-of-range effect only when it exists and is not dead, and
// always stops the turncoating effects afterwards.
#include <cstdio>
class CThing { public:
    unsigned char m_flags;
    bool IsDead() const { return (m_flags & 1) != 0; }
};
static CThing* g_faded = 0;
static int g_fades = 0;
static int g_stops = 0;
class CBaseIntelligentPointer { public:
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CTCSpecialAbilities { public:
    static void FadeOrKillEffect(CThing* effect) { g_faded = effect; ++g_fades; }
};
class CTCTurncoatSpell { public:
    virtual ~CTCTurncoatSpell() { }
    virtual void OnKill() { FinalCleanup(); }
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange;
    void StopTurncoatingEffects() { ++g_stops; }
    void FinalCleanup()
    {
        if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange))
            CTCSpecialAbilities::FadeOrKillEffect(TurncoatingEffectOnMeOutOfRange.GetPItem());
        StopTurncoatingEffects();
    }
};
static void Run(CTCTurncoatSpell& spell) { spell.FinalCleanup(); }
int main()
{
    CTCTurncoatSpell spell;
    CThing alive; alive.m_flags = 0;
    CThing dead; dead.m_flags = 1;
    spell.TurncoatingEffectOnMeOutOfRange.PItem = &alive;
    Run(spell);
    if (g_fades != 1 || g_faded != &alive || g_stops != 1) { std::printf("007c7560_TEST FAIL\n"); return 1; }
    spell.TurncoatingEffectOnMeOutOfRange.PItem = &dead;
    Run(spell);
    if (g_fades != 1 || g_stops != 2) { std::printf("007c7560_TEST FAIL\n"); return 1; }
    spell.TurncoatingEffectOnMeOutOfRange.PItem = 0;
    Run(spell);
    if (g_fades != 1 || g_stops != 3) { std::printf("007c7560_TEST FAIL\n"); return 1; }
    std::printf("007c7560_TEST PASS\n");
    return 0;
}