// Behaviour test for 0x008B8F30 CCreatureAction_ScreamerDie::FinishAction: clear the live effect's emitter, base FinishAction, kill the creature.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CTCDParticleEmitter { void ClearAttachments() { Log(1); } };
struct CThing {
    unsigned int m_presentBits[9];
    CTCDParticleEmitter* m_pParticles;
    unsigned char m_flags91;
    int m_killed;
    void Kill(bool immediately) { m_killed = immediately ? 2 : 1; Log(3); }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_pParticles; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
struct CAction {
    CBaseIntelligentPointer PCreature, PEffect;
    void FinishAction()
    {
        if (IS_THING_ALIVE(PEffect)) {
            CTCDParticleEmitter* pEmitter = 0;
            PEffect.GetPItem()->GetInterface(&pEmitter, 0x4b);
            pEmitter->ClearAttachments();
        }
        Log(2);   // CCombatActionBase::FinishAction
        PCreature.GetPItem()->Kill(true);
    }
};
static int Fail() { std::printf("008b8f30_TEST FAIL\n"); return 1; }
int main()
{
    CTCDParticleEmitter emitter;
    CThing me, effect; me.m_killed = 0;
    for (int i = 0; i < 9; ++i) effect.m_presentBits[i] = 0;
    effect.m_presentBits[0x4b >> 5] |= 1u << (0x4b & 31); effect.m_pParticles = &emitter; effect.m_flags91 = 0;
    CAction a; a.PCreature.m_pItem = &me; a.PEffect.m_pItem = &effect;
    a.FinishAction();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || me.m_killed != 2) return Fail();
    g_n = 0; effect.m_flags91 = 1; a.FinishAction();
    if (g_n != 2 || g_log[0] != 2 || g_log[1] != 3) return Fail();
    g_n = 0; a.PEffect.m_pItem = 0; a.FinishAction();
    if (g_n != 2) return Fail();
    std::printf("008b8f30_TEST PASS\n");
    return 0;
}