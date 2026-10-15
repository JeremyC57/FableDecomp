// Behaviour test for 0x008527F0 (CScriptModuleConversationBase::IsThingsOk): both things present and distinct.
#include <cstdio>

struct CThing { int dummy; };
struct CIntelligentPointer_CThingConst {
    const CThing* m_pItem;
    const CThing* GetPItem() const { return m_pItem; }
};

struct CScriptModuleConversationBase {
    CIntelligentPointer_CThingConst PSpeaker;
    CIntelligentPointer_CThingConst PSpokenTo;
    bool IsThingsOk() const;
};

bool CScriptModuleConversationBase::IsThingsOk() const
{
    const CThing* speaker = PSpeaker.GetPItem();
    const CThing* spokenTo = PSpokenTo.GetPItem();
    return speaker && spokenTo && speaker != spokenTo;
}

int main()
{
    CThing a, b;
    CScriptModuleConversationBase m;
    m.PSpeaker.m_pItem = &a; m.PSpokenTo.m_pItem = &b;
    if (!m.IsThingsOk()) { std::printf("008527f0_TEST FAIL\n"); return 1; }
    m.PSpokenTo.m_pItem = &a;
    if (m.IsThingsOk()) { std::printf("008527f0_TEST FAIL\n"); return 1; }
    m.PSpokenTo.m_pItem = 0;
    if (m.IsThingsOk()) { std::printf("008527f0_TEST FAIL\n"); return 1; }
    m.PSpeaker.m_pItem = 0; m.PSpokenTo.m_pItem = &b;
    if (m.IsThingsOk()) { std::printf("008527f0_TEST FAIL\n"); return 1; }
    std::printf("008527f0_TEST PASS\n");
    return 0;
}