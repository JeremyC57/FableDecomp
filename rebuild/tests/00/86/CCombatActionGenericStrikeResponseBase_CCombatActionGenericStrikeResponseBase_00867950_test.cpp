#include <cstdio>
#include <new>

// Model of CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase() @ 0x00867950
struct CThing;
static int g_ptrCtor = 0; static long g_code = -1; static int g_baseCalls = 0;
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_ptrCtor; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() { } };
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4]; CCombatActionBase(long code) { ++g_baseCalls; g_code = code; } };
struct CCombatAction_GenericStrikeResponseBase : CCombatActionBase {
    CIntelligentPointer PAttacker; unsigned char _pad[0x135 - 0x11c]; bool m_respondToStrike;
    virtual void v00() {}
    CCombatAction_GenericStrikeResponseBase() : CCombatActionBase(250), m_respondToStrike(true) { }
};
int main()
{
    unsigned char raw[sizeof(CCombatAction_GenericStrikeResponseBase)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0;
    CCombatAction_GenericStrikeResponseBase* a = new (raw) CCombatAction_GenericStrikeResponseBase();
    if (g_baseCalls != 1 || g_code != 250 || g_ptrCtor != 1 || a->PAttacker.m_pItem != 0 || !a->m_respondToStrike
        || (sizeof(void*) == 4 && (unsigned char*)&a->PAttacker - raw != 0x114) || (sizeof(void*) == 4 && (unsigned char*)&a->m_respondToStrike - raw != 0x135)) { std::printf("00867950_TEST FAIL\n"); return 1; }
    std::printf("00867950_TEST PASS\n"); return 0;
}