#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_setItem[4];
struct CEventBase_NEntityEvents { virtual void v00() { } long m_eventType; CEventBase_NEntityEvents() : m_eventType(0) { } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(void* item) { m_pItem = item; g_setItem[g_set++] = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); } };
namespace NEntityEvents { struct CEventSoldItem : CEventBase_NEntityEvents { CIntelligentPointer m_giver; CIntelligentPointer m_item; bool m_ownsItem; virtual void v00() { }
    CEventSoldItem(void* giver, void* item, bool ownsItem) : CEventBase_NEntityEvents(), m_giver(giver), m_item(item), m_ownsItem(ownsItem) { } }; }
int main() {
    int g, i; NEntityEvents::CEventSoldItem e(&g, &i, true); if (e.m_eventType != 0 || e.m_giver.m_pItem != &g || e.m_item.m_pItem != &i || g_set != 2 || g_setItem[0] != &g || g_setItem[1] != &i || !e.m_ownsItem) { std::printf("0084af30_TEST FAIL\n"); return 1; }
    NEntityEvents::CEventSoldItem e2(0, &i, false); if (e2.m_giver.m_pItem != 0 || e2.m_ownsItem) { std::printf("0084af30_TEST FAIL\n"); return 1; }
    std::printf("0084af30_TEST PASS\n"); return 0;
}
