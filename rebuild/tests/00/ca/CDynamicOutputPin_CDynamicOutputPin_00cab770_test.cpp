// Behaviour test for 0x00CAB770 CDynamicOutputPin::CDynamicOutputPin (standalone copy of the member initialisation and the Initialize failure path)
#include <cstdio>
typedef int HRESULT;
struct Pin {
    void* m_hStopEvent; void* m_pGraphConfig; int m_bPinUsesReadOnlyAllocator; int m_BlockState;
    void* m_hUnblockOutputPinEvent; void* m_hNotifyCallerPinBlockedEvent; unsigned m_dwBlockCallerThreadID; unsigned m_dwNumOutstandingOutputPinUsers;
    HRESULT Initialize(bool eventOk)
    {
        if (!eventOk) return (HRESULT)0x80070008L;
        m_hUnblockOutputPinEvent = &m_hUnblockOutputPinEvent;
        return 0;
    }
    Pin(HRESULT* phr, bool eventOk)
        : m_hStopEvent(0), m_pGraphConfig(0), m_bPinUsesReadOnlyAllocator(0), m_BlockState(0),
          m_hUnblockOutputPinEvent(0), m_hNotifyCallerPinBlockedEvent(0), m_dwBlockCallerThreadID(0), m_dwNumOutstandingOutputPinUsers(0)
    {
        HRESULT hr = Initialize(eventOk);
        if (hr < 0) { *phr = hr; return; }
    }
};
int main()
{
    HRESULT hr = 0;
    Pin ok(&hr, true);
    if (hr != 0 || ok.m_hUnblockOutputPinEvent == 0 || ok.m_BlockState != 0 || ok.m_dwNumOutstandingOutputPinUsers != 0) { std::printf("00cab770_TEST FAIL\n"); return 1; }
    Pin bad(&hr, false);
    if (hr != (HRESULT)0x80070008L || bad.m_hUnblockOutputPinEvent != 0) { std::printf("00cab770_TEST FAIL\n"); return 1; }
    std::printf("00cab770_TEST PASS\n");
    return 0;
}