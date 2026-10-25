// behaviour test for 0x00C5FF60 CEaxContext::RemoveSource: unlink the matching node, delete it, return 0
#define ADDR "00c5ff60"
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
    CEaxContext ctx; CEaxSource a = {0,0}, b = {0,0}, c = {0,0}, x = {0,0};
    ctx.AddSource(&a); ctx.AddSource(&b); ctx.AddSource(&c);         // list: c b a
    CHECK(ctx.RemoveSource(&x) == 0 && g_nodesDeleted == 0);          // absent: no change
    CHECK(ctx.RemoveSource(&b) == 0 && g_nodesDeleted == 1);          // middle
    CHECK(ctx.m_pSources->m_pSource == &c && ctx.m_pSources->m_pNext->m_pSource == &a && !ctx.m_pSources->m_pNext->m_pNext);
    CHECK(ctx.RemoveSource(&c) == 0 && ctx.m_pSources->m_pSource == &a);   // head
    CHECK(ctx.RemoveSource(&a) == 0 && ctx.m_pSources == 0 && g_nodesDeleted == 3);
    CHECK(ctx.RemoveSource(&a) == 0);                                 // empty list
    std::printf("00c5ff60_TEST PASS\n");
    return 0;
}