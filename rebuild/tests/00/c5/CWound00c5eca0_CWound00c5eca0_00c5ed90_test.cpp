// Behaviour model of CSalOutputKS::CSalOutputKS (0x00C5ED90): the class vtable, m_filter / m_pin handles -1, m_eventTerminate /
// m_threadNotify handles 0, m_mutexFrameCount's critical section initialised once, then m_pBuffer, m_pPacketHeaderTable, m_pEventTable,
// m_nFrames zeroed (the GameCODA SDK class shapes; the Win32 import is a stub here).
#include <cstdio>
#include <cstring>
#include <new>
static int g_sectionInits = 0; static void* g_section = 0;
static void InitializeCriticalSectionStub(void* section) { ++g_sectionInits; g_section = section; }
struct CSalFile { virtual ~CSalFile() { } void* m_hDevice; CSalFile() : m_hDevice((void*)-1) { } };
struct CSalKsPin : CSalFile { virtual ~CSalKsPin() { } };
struct CSalEvent { virtual ~CSalEvent() { } void* m_handle; CSalEvent() : m_handle(0) { } };
struct CSalThread { virtual ~CSalThread() { } void* m_handle; unsigned long m_threadId; CSalThread() : m_handle(0) { } };
struct CSalMutex { virtual ~CSalMutex() { } unsigned char m_critSection[0x18]; CSalMutex() { InitializeCriticalSectionStub(m_critSection); } };
struct CCodaObjectBase { virtual ~CCodaObjectBase() { } };
struct CSalOutputKS : CCodaObjectBase {
    CSalFile m_filter; CSalKsPin m_pin;
    void* m_pPacketHeaderTable; void** m_pEventTable; char* m_pBuffer;
    unsigned int m_nFrames, m_nFrameCount, m_nFrameIndex;
    CSalEvent m_eventTerminate; CSalThread m_threadNotify; CSalMutex m_mutexFrameCount;
    CSalOutputKS() { m_pBuffer = 0; m_pPacketHeaderTable = 0; m_pEventTable = 0; m_nFrames = 0; }
};
int main()
{
    static double buf[sizeof(CSalOutputKS) / sizeof(double) + 1];
    std::memset(buf, 0xCD, sizeof(buf));
    CSalOutputKS* w = new (buf) CSalOutputKS();
    unsigned char* b = (unsigned char*)w;
    bool ok = (unsigned char*)&w->m_filter - b == 0x04 && (unsigned char*)&w->m_pin - b == 0x0c && (unsigned char*)&w->m_eventTerminate - b == 0x2c
        && (unsigned char*)&w->m_threadNotify - b == 0x34 && (unsigned char*)&w->m_mutexFrameCount - b == 0x40
        && w->m_filter.m_hDevice == (void*)-1 && w->m_pin.m_hDevice == (void*)-1 && w->m_eventTerminate.m_handle == 0
        && w->m_threadNotify.m_handle == 0 && g_sectionInits == 1 && g_section == w->m_mutexFrameCount.m_critSection
        && w->m_pPacketHeaderTable == 0 && w->m_pEventTable == 0 && w->m_pBuffer == 0 && w->m_nFrames == 0;
    if (!ok) { std::printf("00c5ed90_TEST FAIL\n"); return 1; }
    std::printf("00c5ed90_TEST PASS\n");
    return 0;
}
