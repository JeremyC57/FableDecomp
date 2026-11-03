// behaviour test for 0x00C60930 ~CEAXListener: free queued events and delete every source node
#define ADDR "00c60930"
#include <cstdio>
#include <cstdlib>
static int g_nodesDeleted = 0;
static int g_eventsFreed = 0;
static int g_failAlloc = 0;
struct CCodaObjectBase {
    virtual ~CCodaObjectBase() { }
};
struct CEAXBuffer {
    unsigned int m_dirty;
    int m_result;                       // what MarkAllDirty reports (retail: always 0)
    int MarkAllDirty() { m_dirty |= 0x80000000u; return m_result; }
};
struct CEAXBufferList : CCodaObjectBase {
    CEAXBufferList* m_pNext;
    CEAXBuffer* m_pSource;
    ~CEAXBufferList() { ++g_nodesDeleted; }
};
struct _CHANGES { unsigned int m_flags, m_type, m_environment; _CHANGES* m_pNext; };
struct CEAXListener {
    CEAXBufferList* m_pSources;
    unsigned int m_environment;
    int m_sourceFlags;
    _CHANGES* m_pEvents;
    int m_queueEvents;
    CEAXListener() : m_pSources(0), m_environment(0), m_sourceFlags(0), m_pEvents(0), m_queueEvents(0) { }
    void ClearEvents()
    {
        while (m_pEvents) { _CHANGES* e = m_pEvents; m_pEvents = e->m_pNext; std::free(e); ++g_eventsFreed; }
    }
    int RemoveSource(CEAXBuffer* source)
    {
        CEAXBufferList* prev = 0;
        CEAXBufferList* node = m_pSources;
        while (node) { if (node->m_pSource == source) break; prev = node; node = node->m_pNext; }
        if (node) { if (prev) prev->m_pNext = node->m_pNext; else m_pSources = node->m_pNext; delete node; }
        return 0;
    }
    int AddSource(CEAXBuffer* source)
    {
        int result = 0;
        CEAXBufferList* node = g_failAlloc ? 0 : new CEAXBufferList;
        if (!node) return -5;
        node->m_pNext = m_pSources; node->m_pSource = source; m_pSources = node;
        if (m_sourceFlags & 0x80000000) result = source->MarkAllDirty();
        return result;
    }
    int InvalidateAll()
    {
        int result = 0;
        if (m_queueEvents == 1) {
            _CHANGES* ev = (_CHANGES*)std::malloc(sizeof(_CHANGES));
            ev->m_flags = 0; ev->m_type = 2; ev->m_environment = m_environment; ev->m_pNext = 0;
            if (!m_pEvents) m_pEvents = ev;
            else { _CHANGES* t = m_pEvents; while (t->m_pNext) t = t->m_pNext; t->m_pNext = ev; }
        }
        m_sourceFlags |= 0x80000000;
        for (CEAXBufferList* node = m_pSources; node != 0; node = node->m_pNext) {
            result = node->m_pSource->MarkAllDirty();
            if (result < 0) return result;
        }
        return result;
    }
    void Destroy()
    {
        ClearEvents();
        CEAXBufferList* node = m_pSources;
        while (m_pSources) { m_pSources = node->m_pNext; delete node; node = m_pSources; }
    }
};
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL line %d\n", ADDR, __LINE__); return 1; } } while (0)
int main()
{
    CEAXListener ctx; CEAXBuffer a = {0,0}, b = {0,0};
    ctx.AddSource(&a); ctx.AddSource(&b);
    ctx.m_queueEvents = 1; ctx.InvalidateAll(); ctx.InvalidateAll();
    ctx.Destroy();
    CHECK(g_nodesDeleted == 2 && g_eventsFreed == 2 && ctx.m_pSources == 0 && ctx.m_pEvents == 0);
    CEAXListener empty; empty.Destroy();
    CHECK(g_nodesDeleted == 2);
    std::printf("00c60930_TEST PASS\n");
    return 0;
}