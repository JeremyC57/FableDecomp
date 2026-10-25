// behaviour test for 0x008571C0 CCombatActionBase::GetFrameUpdateTime() const, linked against the built override: the
// base CCreatureActionBase frame time, scaled by the speed variation's multiplier only strictly inside the strike window
// and only when there is an animation entry
#include <cstdio>
#include <cstring>
struct CSpeedVariation { unsigned char _pad[8]; float Multiplier; };
struct CSpeedVariationHandle { const CSpeedVariation* m_pVariation; CSpeedVariationHandle(const CSpeedVariation* p) : m_pVariation(p) { } };
static CSpeedVariation g_var; static int g_handleCalls = 0; static int g_baseCalls = 0;
struct CAnimationEntry { CSpeedVariationHandle GetSpeedVariation(); };
CSpeedVariationHandle CAnimationEntry::GetSpeedVariation() { ++g_handleCalls; return CSpeedVariationHandle(&g_var); }
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
protected:
    virtual float GetFrameUpdateTime() const;
};
CCreatureActionBase::~CCreatureActionBase() { }
float CCreatureActionBase::GetFrameUpdateTime() const { ++g_baseCalls; return 0.5f; }
class CCombatActionBase : public CCreatureActionBase {
public:
    unsigned char _pad04[0x40 - 4]; float RunningTimeSeconds; unsigned char _pad44[0x74 - 0x44]; CAnimationEntry* PAnimEntry;
    unsigned char _pad78[0xd0 - 0x78]; float StrikeSpeedVariationStartTime; float StrikeSpeedVariationEndTime; unsigned char _padd8[0x114 - 0xd8];
    virtual float GetFrameUpdateTime() const;
};
int main()
{
    static unsigned char mem[sizeof(CCombatActionBase)];
    std::memset(mem, 0, sizeof(mem));
    CCombatActionBase* a = (CCombatActionBase*)mem;
    g_var.Multiplier = 3.0f; CAnimationEntry e;
    a->PAnimEntry = 0; a->RunningTimeSeconds = 2.0f; a->StrikeSpeedVariationStartTime = 1.0f; a->StrikeSpeedVariationEndTime = 3.0f;
    bool ok = a->CCombatActionBase::GetFrameUpdateTime() == 0.5f && g_handleCalls == 0 && g_baseCalls == 1;
    a->PAnimEntry = &e;
    ok = ok && a->CCombatActionBase::GetFrameUpdateTime() == 1.5f && g_handleCalls == 1;
    a->RunningTimeSeconds = 1.0f; ok = ok && a->CCombatActionBase::GetFrameUpdateTime() == 0.5f;
    a->RunningTimeSeconds = 3.0f; ok = ok && a->CCombatActionBase::GetFrameUpdateTime() == 0.5f && g_baseCalls == 4;
    if (!ok) { std::printf("008571c0_TEST FAIL\n"); return 1; }
    std::printf("008571c0_TEST PASS\n"); return 0;
}
