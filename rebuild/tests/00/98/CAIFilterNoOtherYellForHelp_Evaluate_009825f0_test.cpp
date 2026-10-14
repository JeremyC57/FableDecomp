#include <cstdio>
struct CMessageEvent;
struct CMessageEventFilter_IsOfType { long m_type; CMessageEventFilter_IsOfType(long type) : m_type(type) { } };
static long g_seenType = 0; static CMessageEvent* g_result = 0;
struct CMessageEventManager { CMessageEvent* FindMostRecentMessage(CMessageEventFilter_IsOfType* filter) { g_seenType = filter->m_type; return g_result; } };
struct CThingCreature { unsigned char _pad[0x60]; CMessageEventManager* m_pMessageManager; CMessageEventManager* GetMessageManager() { return m_pMessageManager; } };
struct CAIFilter_NoOtherYellForHelp { CThingCreature* m_pCreature; virtual bool Evaluate(); };
bool CAIFilter_NoOtherYellForHelp::Evaluate() { CMessageEventFilter_IsOfType filter(11); return m_pCreature->GetMessageManager()->FindMostRecentMessage(&filter) == 0; }
int main()
{
    CMessageEventManager mgr; CThingCreature c; c.m_pMessageManager = &mgr; CAIFilter_NoOtherYellForHelp f; f.m_pCreature = &c;
    if (!f.Evaluate() || g_seenType != 11) { std::printf("009825f0_TEST FAIL\n"); return 1; }
    g_result = (CMessageEvent*)&mgr; if (f.Evaluate()) { std::printf("009825f0_TEST FAIL\n"); return 1; }
    std::printf("009825f0_TEST PASS\n");
    return 0;
}