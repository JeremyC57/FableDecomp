#include <cstdio>
struct SSampleHeader { int value; };
static SSampleHeader g_items[5] = { { 10 }, { 11 }, { 12 }, { 13 }, { 14 } };
struct CSampleList {
    unsigned long count;
    virtual SSampleHeader* GetItem(unsigned long index) { return &g_items[index]; }
    virtual unsigned long GetCount() { return count; }
};
struct CBankData { CSampleList m_samples; };
struct CLoadLog { bool m_ok; int failAfter; bool IsOk() const { return m_ok; } };
struct CBankProcessor {
    CBankData* m_pData; int m_sum; CLoadLog* m_pLog; int m_added;
    void AddSample(const SSampleHeader* header) { m_sum += header->value; if (++m_added == m_pLog->failAfter) m_pLog->m_ok = false; }
    void AddSamples()
    {
        for (unsigned long i = 0; i < m_pData->m_samples.GetCount(); ++i) {
            AddSample(m_pData->m_samples.GetItem(i));
            if (!m_pLog->IsOk())
                return;
        }
    }
};
int main()
{
    CBankData data; data.m_samples.count = 5;
    CLoadLog log; log.m_ok = true; log.failAfter = 100;
    CBankProcessor p; p.m_pData = &data; p.m_sum = 0; p.m_pLog = &log; p.m_added = 0;
    p.AddSamples();
    bool ok = p.m_sum == 60 && p.m_added == 5;
    log.failAfter = 2; p.m_sum = 0; p.m_added = 0; p.AddSamples();
    ok = ok && p.m_sum == 21 && p.m_added == 2;
    data.m_samples.count = 0; log.m_ok = true; p.m_added = 0; p.AddSamples();
    ok = ok && p.m_added == 0;
    if (!ok) { std::printf("00c2e270_TEST FAIL\n"); return 1; }
    std::printf("00c2e270_TEST PASS\n");
    return 0;
}
