#include <cstdio>

// Model of CCombatAction_GenericStrikeResponseBase::IsOverrideableByActionOfSamePriority @ 0x00865E70
struct CThing { unsigned char _pad[0x30]; unsigned long Flags; unsigned char _pad34[0x91 - 0x34]; unsigned char DeadFlags; };
static int g_gets = 0;
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { ++g_gets; return m_pItem; } };
struct CCombatAction_GenericStrikeResponseBase {
    virtual void v00() {}
    unsigned char _body[0x114 - 4]; CBaseIntelligentPointer PAttacker;
    bool IsOverrideableByActionOfSamePriority() const {
        if (PAttacker.GetPItem() != 0) { if (!(PAttacker.GetPItem()->DeadFlags & 1)) { if (PAttacker.GetPItem()->Flags & 0x40000) return false; } }
        return true;
    }
};
int main()
{
    CCombatAction_GenericStrikeResponseBase a; CThing t; a.PAttacker.m_pItem = 0;
    if (!a.IsOverrideableByActionOfSamePriority() || g_gets != 1) { std::printf("00865e70_TEST FAIL\n"); return 1; }
    a.PAttacker.m_pItem = &t; t.DeadFlags = 1; t.Flags = 0x40000;
    if (!a.IsOverrideableByActionOfSamePriority() || g_gets != 3) { std::printf("00865e70_TEST FAIL\n"); return 1; }
    t.DeadFlags = 0;
    if (a.IsOverrideableByActionOfSamePriority() || g_gets != 6) { std::printf("00865e70_TEST FAIL\n"); return 1; }
    t.Flags = 0;
    if (!a.IsOverrideableByActionOfSamePriority()) { std::printf("00865e70_TEST FAIL\n"); return 1; }
    std::printf("00865e70_TEST PASS\n"); return 0;
}