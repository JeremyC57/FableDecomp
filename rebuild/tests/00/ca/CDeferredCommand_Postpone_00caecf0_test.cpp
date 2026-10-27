#include <cstdio>
// CDeferredCommand::Postpone: convert REFTIME seconds to 100ns units; VFW_E_TIME_ALREADY_PASSED if already due; else
// Remove, set m_time, Insert (Remove's failure returned).
struct DeferredCommand;
struct CmdQueue {
    long long now; int removeFail, inserted, removed; long long lastCheck; int lastStream;
    int CheckTime(long long t, int bStream) { lastCheck = t; lastStream = bStream; return t <= now; }
    int Remove(DeferredCommand*) { if (removeFail) return (int)0x80004005; ++removed; return 0; }
    int Insert(DeferredCommand*) { ++inserted; return 0; }
};
struct DeferredCommand {
    CmdQueue* m_pQueue; long long m_time; int m_bStream;
    int Postpone(double newtime)
    {
        long long convertor = (long long)(newtime * 10000000);
        if (m_pQueue->CheckTime(convertor, m_bStream)) return (int)0x80040233;
        int hr = m_pQueue->Remove(this);
        if (hr < 0) return hr;
        m_time = convertor;
        hr = m_pQueue->Insert(this);
        return hr;
    }
};
int main()
{
    CmdQueue q; q.now = 20000000; q.removeFail = 0; q.inserted = 0; q.removed = 0; q.lastCheck = 0; q.lastStream = -1;
    DeferredCommand c; c.m_pQueue = &q; c.m_time = 0; c.m_bStream = 1;
    if (c.Postpone(1.5) != (int)0x80040233 || q.lastCheck != 15000000 || q.lastStream != 1 || q.removed != 0) { std::printf("00caecf0_TEST FAIL\n"); return 1; }
    if (c.Postpone(3.0) != 0 || c.m_time != 30000000 || q.removed != 1 || q.inserted != 1) { std::printf("00caecf0_TEST FAIL\n"); return 1; }
    q.removeFail = 1; if (c.Postpone(4.0) != (int)0x80004005 || c.m_time != 30000000 || q.inserted != 1) { std::printf("00caecf0_TEST FAIL\n"); return 1; }
    std::printf("00caecf0_TEST PASS\n");
    return 0;
}