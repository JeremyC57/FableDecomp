#include <cstdio>
static int g_items[5] = { 10, 11, 12, 13, 14 };
struct CItemSource {
    unsigned long count;
    virtual void* GetItem(unsigned long index) { return &g_items[index]; }
    virtual unsigned long GetCount() { return count; }
};
struct CLoadStatus { bool m_ok; int failAfter; bool IsOk() const { return m_ok; } };
struct CChildListLoader {
    CItemSource* m_pSource; int m_sum; CLoadStatus* m_pStatus; int m_added;
    void AddChild(void* item) { m_sum += *(int*)item; if (++m_added == m_pStatus->failAfter) m_pStatus->m_ok = false; }
    void AddChildren()
    {
        for (unsigned long i = 0; i < m_pSource->GetCount(); ++i) {
            AddChild(m_pSource->GetItem(i));
            if (!m_pStatus->IsOk())
                return;
        }
    }
};
int main()
{
    CItemSource src; src.count = 5;
    CLoadStatus st; st.m_ok = true; st.failAfter = 100;
    CChildListLoader l; l.m_pSource = &src; l.m_sum = 0; l.m_pStatus = &st; l.m_added = 0;
    l.AddChildren();
    bool ok = l.m_sum == 60 && l.m_added == 5;
    st.failAfter = 2; l.m_sum = 0; l.m_added = 0; l.AddChildren();
    ok = ok && l.m_sum == 21 && l.m_added == 2;
    src.count = 0; st.m_ok = true; l.m_added = 0; l.AddChildren();
    ok = ok && l.m_added == 0;
    if (!ok) { std::printf("00c2e270_TEST FAIL\n"); return 1; }
    std::printf("00c2e270_TEST PASS\n");
    return 0;
}
