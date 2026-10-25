// Behaviour test for 00caf500: CCmdQueue::CCmdQueue -- lock, two lists, manual-reset event, cleared advise/clock/running/times
#include <cstdio>
#include <cstring>
#include <new>
static char g_seq[16] = ""; static int g_manual = -1;
struct CCritSec { CCritSec() { std::strcat(g_seq, "L"); } ~CCritSec() { } };
struct CBaseList { const unsigned short* m_name; CBaseList(const unsigned short* n) : m_name(n) { std::strcat(g_seq, "B"); } };
template <class T> struct CGenericList : CBaseList { CGenericList(const unsigned short* n) : CBaseList(n) { } };
struct CAMEvent { CAMEvent(int manual = 0) { g_manual = manual; std::strcat(g_seq, "E"); } };
struct CRefTime { __int64 m_time; CRefTime() { m_time = 0; } };
struct CDeferredCommand;
struct CCmdQueue {
    CCritSec m_Lock; CGenericList<CDeferredCommand> m_listPresentation, m_listStream; CAMEvent m_evDue;
    unsigned long m_dwAdvise; CRefTime m_tCurrentAdvise; void* m_pClock; int m_bRunning; CRefTime m_StreamTimeOffset;
    CCmdQueue() : m_listPresentation(0), m_listStream(0), m_evDue(1), m_dwAdvise(0), m_pClock(0), m_bRunning(0) { }
};
int main()
{
    unsigned char raw[sizeof(CCmdQueue)]; std::memset(raw, 0xCD, sizeof raw);
    CCmdQueue* q = new (raw) CCmdQueue;
    int ok = !std::strcmp(g_seq, "LBBE") && g_manual == 1 && q->m_dwAdvise == 0 && q->m_pClock == 0 && q->m_bRunning == 0
          && q->m_tCurrentAdvise.m_time == 0 && q->m_StreamTimeOffset.m_time == 0 && q->m_listPresentation.m_name == 0;
    std::printf(ok ? "00caf500_TEST PASS\n" : "00caf500_TEST FAIL\n");
    return ok ? 0 : 1;
}
