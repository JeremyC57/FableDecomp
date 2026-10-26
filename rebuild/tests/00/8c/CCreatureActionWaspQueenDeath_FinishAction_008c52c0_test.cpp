// Behaviour test for 0x008C52C0 CCreatureAction_WaspQueenDeath::FinishAction: runs the reconstructed body over stubbed
// callees; checks the emitter guards and ClearAttachments, the base call and the WaspQueenDie(owner, 0) restart.
#include <cstdio>
static int g_cleared = 0;
class CTCDParticleEmitter { public: void ClearAttachments_006e0de0() { ++g_cleared; } };
class CThing { public:
    unsigned long m_interfaceBits28; CTCDParticleEmitter* m_pParticleEmitter; unsigned char m_flags91;
    bool GetTCParticleEmitter(CTCDParticleEmitter** out) { if (m_interfaceBits28 & 0x800) { *out = m_pParticleEmitter; return true; } return false; }
};
class CSoundPair { public: long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
class CCombatActionBase;
static int g_set = 0; static long g_code = 0; static long g_loop = -1;
class CThingCreatureBase : public CThing { public: bool SetCurrentAction(const CCombatActionBase& action); };
struct CBaseIntelligentPointer { void* m_pItem; CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)m_pItem; } };
static int g_base = 0, g_dtors = 0;
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature; long m_code;
    CCombatActionBase() : m_code(0) { }
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair&) : m_code(actionCode) { PCreature.m_pItem = owner; }
    ~CCombatActionBase() { ++g_dtors; }
    void FinishAction() { ++g_base; }
};
class CCreatureAction_WaspQueenDie : public CCombatActionBase { public:
    long LoopNum;
    CCreatureAction_WaspQueenDie(CThingCreatureBase* owner, long loopNum) : CCombatActionBase(owner, 250, CSoundPair()), LoopNum(loopNum) { }
};
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& a)
{
    ++g_set; g_code = a.m_code; g_loop = ((const CCreatureAction_WaspQueenDie&)a).LoopNum; return a.PCreature.m_pItem == this;
}
class CCreatureAction_WaspQueenDeath : public CCombatActionBase { public:
    CBaseIntelligentPointer PEmitter;
    void FinishAction();
};
void CCreatureAction_WaspQueenDeath::FinishAction()
{
    CTCDParticleEmitter* emitter;
    if (PEmitter.GetPItem() && !(PEmitter.GetPItem()->m_flags91 & 1) && PEmitter.GetPItem()->GetTCParticleEmitter(&emitter))
        emitter->ClearAttachments_006e0de0();
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_WaspQueenDie(PCreature.GetPItem(), 0));
}
int main()
{
    CTCDParticleEmitter pe; CThingCreatureBase owner = CThingCreatureBase(); CThingCreatureBase em = CThingCreatureBase();
    em.m_interfaceBits28 = 0x800; em.m_pParticleEmitter = &pe;
    CCreatureAction_WaspQueenDeath a; a.PCreature.m_pItem = &owner; a.PEmitter.m_pItem = &em;
    a.FinishAction();
    bool ok = g_cleared == 1 && g_base == 1 && g_set == 1 && g_code == 250 && g_loop == 0 && g_dtors == 1;
    em.m_interfaceBits28 = 0; a.FinishAction();       // no emitter component
    ok = ok && g_cleared == 1 && g_set == 2;
    em.m_interfaceBits28 = 0x800; em.m_flags91 = 1; a.FinishAction();   // dead emitter
    ok = ok && g_cleared == 1 && g_set == 3;
    if (!ok) { std::printf("008c52c0_TEST FAIL\n"); return 1; }
    std::printf("008c52c0_TEST PASS\n");
    return 0;
}