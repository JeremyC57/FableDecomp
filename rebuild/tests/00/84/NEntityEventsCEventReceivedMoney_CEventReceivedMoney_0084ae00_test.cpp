#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_setItem[4];
struct CEventBase_NEntityEvents { virtual void v00() { } long m_eventType; CEventBase_NEntityEvents() : m_eventType(0) { } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(void* item) { m_pItem = item; g_setItem[g_set++] = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); } };
namespace NEntityEvents { class CEventReceivedMoney : public CEventBase_NEntityEvents { public: CIntelligentPointer m_giver; long m_amount; virtual void v00() { }
    CEventReceivedMoney(void* giver, long amount) : CEventBase_NEntityEvents(), m_giver(giver), m_amount(amount) { } }; }
int main() {
    int g; NEntityEvents::CEventReceivedMoney e(&g, 250); if (e.m_eventType != 0 || e.m_giver.m_pItem != &g || g_set != 1 || g_setItem[0] != &g || e.m_amount != 250) { std::printf("0084ae00_TEST FAIL\n"); return 1; }
    std::printf("0084ae00_TEST PASS\n"); return 0;
}