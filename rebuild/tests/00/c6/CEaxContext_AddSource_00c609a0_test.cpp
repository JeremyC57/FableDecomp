// behaviour test for 0x00C609A0 CEaxContext::AddSource: push a node; mark the source dirty when the context flag is set
#define ADDR "00c609a0"
#include <cstdio>
#include <cstdlib>
static int g_nodesDeleted = 0;
static int g_eventsFreed = 0;
static int g_failAlloc = 0;
struct CPKObject {
    virtual ~CPKObject() { }
};
struct CEaxSource {
    unsigned int m_dirty;
    int m_result;                       // what MarkAllDirty reports (retail: always 0)
    int MarkAllDirty() { m_dirty |= 0x80000000u; return m_result; }
};
struct CEaxSourceNode : CPKObject {
    CEaxSourceNode* m_pNext;
    CEaxSource* m_pSource;
    ~CEaxSourceNode() { ++g_nodesDeleted; }
};
struct SEaxEvent { unsigned int m_flags, m_type, m_environment; SEaxEvent* m_pNext; };
struct CEaxContext {
    CEaxSourceNode* m_pSources;
    unsigned int m_environment;
    int m_sourceFlags;
    SEaxEvent* m_pEvents;
    int m_queueEvents;
    CEaxContext() : m_pSources(0), m_environment(0), m_sourceFlags(0), m_pEvents(0), m_queueEvents(0) { }
    void ClearEvents()
    {
        while (m_pEvents) { SEaxEvent* e = m_pEvents; m_pEvents = e->m_pNext; std::free(e); ++g_eventsFreed; }
    }
    int RemoveSource(CEaxSource* source)
    {
        CEaxSourceNode* prev = 0;
        CEaxSourceNode* node = m_pSources;
        while (node) { if (node->m_pSource == source) break; prev = node; node = node->m_pNext; }
        if (node) { if (prev) prev->m_pNext = node->m_pNext; else m_pSources = node->m_pNext; delete node; }
        return 0;
    }
    int AddSource(CEaxSource* source)
    {
        int result = 0;
        CEaxSourceNode* node = g_failAlloc ? 0 : new CEaxSourceNode;
        if (!node) return -5;
        node->m_pNext = m_pSources; node->m_pSource = source; m_pSources = node;
        if (m_sourceFlags & 0x80000000) result = source->MarkAllDirty();
        return result;
    }
    int InvalidateAll()
    {
        int result = 0;
        if (m_queueEvents == 1) {
            SEaxEvent* ev = (SEaxEvent*)std::malloc(sizeof(SEaxEvent));
            ev->m_flags = 0; ev->m_type = 2; ev->m_environment = m_environment; ev->m_pNext = 0;
            if (!m_pEvents) m_pEvents = ev;
            else { SEaxEvent* t = m_pEvents; while (t->m_pNext) t = t->m_pNext; t->m_pNext = ev; }
        }
        m_sourceFlags |= 0x80000000;
        for (CEaxSourceNode* node = m_pSources; node != 0; node = node->m_pNext) {
            result = node->m_pSource->MarkAllDirty();
            if (result < 0) return result;
        }
        return result;
    }
    void Destroy()
    {
        ClearEvents();
        CEaxSourceNode* node = m_pSources;
        while (m_pSources) { m_pSources = node->m_pNext; delete node; node = m_pSources; }
    }
};
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL line %d\n", ADDR, __LINE__); return 1; } } while (0)
int main()
{
    CEaxContext ctx; CEaxSource a = {0,0}, b = {0,7};
    CHECK(ctx.AddSource(&a) == 0 && ctx.m_pSources->m_pSource == &a && a.m_dirty == 0);
    ctx.m_sourceFlags = (int)0x80000000;
    CHECK(ctx.AddSource(&b) == 7 && ctx.m_pSources->m_pSource == &b && ctx.m_pSources->m_pNext->m_pSource == &a);
    CHECK(b.m_dirty == 0x80000000u);
    g_failAlloc = 1;
    CHECK(ctx.AddSource(&a) == -5 && ctx.m_pSources->m_pSource == &b);
    g_failAlloc = 0;
    ctx.Destroy();
    std::printf("00c609a0_TEST PASS\n");
    return 0;
}