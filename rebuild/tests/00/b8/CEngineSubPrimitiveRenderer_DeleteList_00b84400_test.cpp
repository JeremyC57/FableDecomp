// Behaviour test for 0x00B84400 CEngineSubPrimitiveRenderer::DeleteList: every chained sub destroyed once, vector emptied.
#include <cstdio>

static int g_destroyed = 0;
struct CSub {
    CSub* m_pNext;
    bool m_alive;
    virtual void Destroy(unsigned int flags) { if (m_alive && flags == 1) ++g_destroyed; m_alive = false; }
};
struct SEntry { long m_key; CSub* m_pFirst; };
struct CVec {
    SEntry* m_pFirst; SEntry* m_pLast; SEntry* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    static SEntry* Copy(SEntry* first, SEntry* last, SEntry* result)
    {
        for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
        return result;
    }
    void Erase(SEntry* first, SEntry* last) { SEntry* kept = Copy(last, m_pLast, first); m_pLast = kept; }
};

static void DeleteList(CVec* list)
{
    unsigned long count = list->GetCount();
    for (unsigned long i = 0; i < count; ++i) {
        CSub* sub = list->m_pFirst[i].m_pFirst;
        do { CSub* next = sub->m_pNext; sub->Destroy(1); sub = next; } while (sub);
    }
    list->Erase(list->m_pFirst, list->m_pLast);
}

int main()
{
    CSub s[4];
    for (int i = 0; i < 4; ++i) { s[i].m_alive = true; s[i].m_pNext = 0; }
    s[0].m_pNext = &s[1]; s[1].m_pNext = &s[2];
    SEntry e[2]; e[0].m_pFirst = &s[0]; e[1].m_pFirst = &s[3];
    CVec v; v.m_pFirst = e; v.m_pLast = e + 2; v.m_pEnd = e + 2;
    DeleteList(&v);
    if (g_destroyed != 4 || v.m_pLast != v.m_pFirst || v.m_pEnd != e + 2) { std::printf("00b84400_TEST FAIL\n"); return 1; }
    std::printf("00b84400_TEST PASS\n");
    return 0;
}