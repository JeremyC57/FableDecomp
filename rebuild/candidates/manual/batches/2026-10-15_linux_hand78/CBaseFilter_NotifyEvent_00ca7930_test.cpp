#include <cstdio>
static long g_code = 0, g_p1 = 0; static const void* g_p2 = 0; static int g_calls = 0;
struct IMediaEventSinkLike { virtual long Notify(long code, long p1, const void* p2) { ++g_calls; g_code = code; g_p1 = p1; g_p2 = p2; return 7; } virtual ~IMediaEventSinkLike() {} };
struct IBaseFilterLike { int m_tag; };
struct CBaseFilter {
    int m_unknown[3];
    IBaseFilterLike m_filter;                      // the IBaseFilter sub-object
    IMediaEventSinkLike* m_pSink;
    int NotifyEvent(long eventCode, long eventParam1, const void* eventParam2)
    {
        IMediaEventSinkLike* pSink = m_pSink;
        if (pSink) {
            if (eventCode == 1) eventParam2 = &m_filter;
            return pSink->Notify(eventCode, eventParam1, eventParam2);
        } else {
            return (int)0x80004001L;
        }
    }
};
int main()
{
    IMediaEventSinkLike sink; int other = 0;
    CBaseFilter f; f.m_pSink = 0;
    if (f.NotifyEvent(1, 2, &other) != (int)0x80004001L || g_calls != 0) { std::printf("00ca7930_TEST FAIL\n"); return 1; }
    f.m_pSink = &sink;
    if (f.NotifyEvent(1, 2, &other) != 7 || g_code != 1 || g_p1 != 2 || g_p2 != &f.m_filter) { std::printf("00ca7930_TEST FAIL\n"); return 1; }
    if (f.NotifyEvent(5, 3, &other) != 7 || g_code != 5 || g_p1 != 3 || g_p2 != &other || g_calls != 2) { std::printf("00ca7930_TEST FAIL\n"); return 1; }
    std::printf("00ca7930_TEST PASS\n");
    return 0;
}
