#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_setItem[4];
struct CEventBase_NEntityEvents { virtual void v00() { } long m_eventType; CEventBase_NEntityEvents() : m_eventType(0) { } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(void* item) { m_pItem = item; g_setItem[g_set++] = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); } };
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; CIVCountedPointer() : Object(0) { } CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); } } };
namespace NEntityEvents { struct CEventExpressionPerformedTo : CEventBase_NEntityEvents { CIntelligentPointer m_target; CIVCountedPointer m_data; long m_expression; long m_a; long m_b; virtual void v00() { }
    CEventExpressionPerformedTo(void* target, long expression, const CIVCountedPointer& data, long a, long b) : CEventBase_NEntityEvents(), m_target(target), m_data(data), m_expression(expression), m_a(a), m_b(b) { } }; }
int main() {
    CIVCountedPointeeBase obj; obj.IVRefCount = 1; CIVCountedPointer data; data.Object = &obj; int t;
    { NEntityEvents::CEventExpressionPerformedTo e(&t, 7, data, 8, 9); if (e.m_eventType != 0 || e.m_target.m_pItem != &t || e.m_data.Object != &obj || obj.IVRefCount != 2 || e.m_expression != 7 || e.m_a != 8 || e.m_b != 9) { std::printf("0084b010_TEST FAIL\n"); return 1; } }
    if (obj.IVRefCount != 1) { std::printf("0084b010_TEST FAIL\n"); return 1; } data.Object = 0;
    std::printf("0084b010_TEST PASS\n"); return 0;
}