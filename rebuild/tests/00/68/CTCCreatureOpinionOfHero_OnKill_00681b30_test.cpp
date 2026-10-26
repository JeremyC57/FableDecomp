// Test for 00681b30 CTCCreatureOpinionOfHero::OnKill: kill the unknown-threat source thing only when set and alive.
#include <cstdio>
class CThing { public: bool Dead; int KillCalls; bool LastCheck; void Kill(bool c) { ++KillCalls; LastCheck = c; } };
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}
class CTCCreatureOpinionOfHero { public:
    CBaseIntelligentPointer PUnknownThreatOpinionSource;
    virtual ~CTCCreatureOpinionOfHero() {}
    virtual void OnKill()
    {
        if (IS_THING_ALIVE(PUnknownThreatOpinionSource))
            PUnknownThreatOpinionSource.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing alive = { false, 0, false }, dead = { true, 0, false };
    CTCCreatureOpinionOfHero o; o.PUnknownThreatOpinionSource.PItem = 0;
    o.OnKill();
    o.PUnknownThreatOpinionSource.PItem = &dead; o.OnKill();
    o.PUnknownThreatOpinionSource.PItem = &alive; o.OnKill();
    if (dead.KillCalls != 0 || alive.KillCalls != 1 || !alive.LastCheck) { std::printf("00681b30_TEST FAIL\n"); return 1; }
    std::printf("00681b30_TEST PASS\n");
    return 0;
}