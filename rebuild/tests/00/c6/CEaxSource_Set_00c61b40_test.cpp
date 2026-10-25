// behaviour test for 0x00C61B40 CEaxSource::Set: deferred bit 0x100000 stripped and forwarded, event queue handed back
#include <cstdio>
struct SEaxEvent { int id; };
struct CEaxContext {
    SEaxEvent* m_pEvents;
    int m_cleared;
    void ClearEvents() { ++m_cleared; m_pEvents = 0; }
};
struct CEaxSource {
    CEaxContext* m_pContext;
    unsigned int m_lastProperty;
    const void* m_lastValue;
    int m_lastDeferred;
    SEaxEvent m_event;
    int SetProperty(unsigned int property, const void* value, int deferred)
    {
        m_lastProperty = property; m_lastValue = value; m_lastDeferred = deferred;
        m_pContext->m_pEvents = &m_event;          // the setter queues a notification
        return property == 3 ? -3 : 0;
    }
    int Set(unsigned int property, const void* value, SEaxEvent** ppEvents)
    {
        int result;
        m_pContext->ClearEvents();
        if (property & 0x100000) result = SetProperty(property & ~0x100000u, value, 1);
        else result = SetProperty(property, value, 0);
        *ppEvents = m_pContext->m_pEvents;
        return result;
    }
};
int main()
{
    CEaxContext ctx = { 0, 0 };
    CEaxSource src; src.m_pContext = &ctx;
    int v = 5; SEaxEvent* ev = 0;
    if (src.Set(0x100007, &v, &ev) != 0 || src.m_lastProperty != 7 || src.m_lastDeferred != 1 || src.m_lastValue != &v
        || ev != &src.m_event || ctx.m_cleared != 1) { std::printf("00c61b40_TEST FAIL\n"); return 1; }
    ev = 0;
    if (src.Set(3, &v, &ev) != -3 || src.m_lastProperty != 3 || src.m_lastDeferred != 0 || ev != &src.m_event
        || ctx.m_cleared != 2) { std::printf("00c61b40_TEST FAIL\n"); return 1; }
    std::printf("00c61b40_TEST PASS\n");
    return 0;
}