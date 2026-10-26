#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_setItem[4];
struct CEventBase_NEntityEvents { virtual void v00() { } long m_eventType; CEventBase_NEntityEvents() : m_eventType(0) { } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(void* item) { m_pItem = item; g_setItem[g_set++] = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); } };
namespace NEntityEvents { class CEventHitThing : public CEventBase_NEntityEvents { public: CIntelligentPointer m_thing; bool m_a, m_b, m_c, m_d, m_e; virtual void v00() { }
    CEventHitThing(void*, void* thing, bool a, bool b, bool c, void*, bool d, bool e) : CEventBase_NEntityEvents(), m_thing(thing), m_a(a), m_b(b), m_c(c), m_d(d), m_e(e) { } }; }
int main() {
    int t, u; NEntityEvents::CEventHitThing e(&u, &t, true, false, true, &u, false, true);
    if (e.m_eventType != 0 || e.m_thing.m_pItem != &t || g_set != 1 || !e.m_a || e.m_b || !e.m_c || e.m_d || !e.m_e) { std::printf("008110a0_TEST FAIL\n"); return 1; }
    std::printf("008110a0_TEST PASS\n"); return 0;
}