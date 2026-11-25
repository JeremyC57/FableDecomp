// Behaviour model of CSalOutputKS::~CSalOutputKS (0x00C5ECA0): ReleaseBuffer once (the class's own slot), the filter file closed explicitly,
// then the members in reverse (mutex section deleted, thread / event handles closed when non-null, pin / filter handles closed when not -1);
// a -1 / null handle is not closed twice. The Win32 imports are stubs here.
#include <cstdio>
#include <new>
static int g_close = 0, g_del = 0, g_release = 0;
static void CloseStub(void*) { ++g_close; } static void DelStub(void*) { ++g_del; }
struct CSalFile { void* m_hDevice; void Close() { if (m_hDevice != (void*)-1) { CloseStub(m_hDevice); m_hDevice = (void*)-1; } } virtual ~CSalFile() { Close(); } };
struct CSalKsPin : CSalFile { virtual ~CSalKsPin() { } };
struct CSalEvent { void* m_handle; void Close() { if (m_handle) { CloseStub(m_handle); m_handle = 0; } } virtual ~CSalEvent() { Close(); } };
struct CSalThread { void* m_handle; unsigned long m_threadId; void Close() { if (m_handle) { CloseStub(m_handle); m_handle = 0; } } virtual ~CSalThread() { Close(); } };
struct CSalMutex { unsigned char m_critSection[0x18]; virtual ~CSalMutex() { DelStub(m_critSection); } };
struct CCodaObjectBase { virtual ~CCodaObjectBase() { } };
struct CSalOutput : CCodaObjectBase { virtual ~CSalOutput() { } virtual void ReleaseBuffer() = 0; };
struct CSalOutputKS : CSalOutput {
    CSalFile m_filter; CSalKsPin m_pin;
    void* m_pPacketHeaderTable; void** m_pEventTable; char* m_pBuffer;
    unsigned int m_nFrames, m_nFrameCount, m_nFrameIndex;
    CSalEvent m_eventTerminate; CSalThread m_threadNotify; CSalMutex m_mutexFrameCount;
    virtual void ReleaseBuffer() { ++g_release; }
    virtual ~CSalOutputKS() { ReleaseBuffer(); m_filter.Close(); }
};
static bool Check()
{
    int h;
    CSalOutputKS* w = new CSalOutputKS;
    w->m_filter.m_hDevice = &h; w->m_pin.m_hDevice = (void*)-1; w->m_eventTerminate.m_handle = &h; w->m_threadNotify.m_handle = 0;
    delete w;
    return g_release == 1 && g_close == 2 && g_del == 1;
}
int main()
{
    if (!Check()) { std::printf("00c5eca0_TEST FAIL\n"); return 1; }
    std::printf("00c5eca0_TEST PASS\n");
    return 0;
}
