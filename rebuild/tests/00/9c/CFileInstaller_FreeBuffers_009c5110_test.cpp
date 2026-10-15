#include <cstdio>
#include <cstring>
// CFileInstaller::FreeBuffers: under the lock, release the chunk array, GlobalFree and clear the read/write buffers
static char g_log[32]; static void* g_globalFreed[4]; static long g_globalFrees;
static void Log(const char* s) { std::strcat(g_log, s); }
static void* GlobalFreeHook(void* h) { g_globalFreed[g_globalFrees++] = h; Log("g"); return 0; }
struct RTL_CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock {
    RTL_CRITICAL_SECTION_Fable* m_pSection; bool m_locked;
    CCriticalSectionLock(RTL_CRITICAL_SECTION_Fable* section, bool tryOnly) : m_pSection(section), m_locked(!tryOnly) { Log("<"); }
    ~CCriticalSectionLock() { Log(">"); }
};
struct CBufferChunkArray { void* m_pFirst; void* m_pLast; void* m_pEnd; void DeallocateMemory() { m_pFirst = m_pLast = m_pEnd = 0; Log("d"); } };
struct CFileInstaller {
    RTL_CRITICAL_SECTION_Fable CriticalSection; CBufferChunkArray BufferChunks; unsigned char* PReadBuffer; unsigned char* PWriteBuffer;
    void FreeBuffers()
    {
        CCriticalSectionLock lock(&CriticalSection, false);
        BufferChunks.DeallocateMemory();
        if (PReadBuffer) { GlobalFreeHook(PReadBuffer); PReadBuffer = 0; }
        if (PWriteBuffer) { GlobalFreeHook(PWriteBuffer); PWriteBuffer = 0; }
    }
};
int main()
{
    unsigned char rd[4], wr[4];
    CFileInstaller f; f.BufferChunks.m_pFirst = rd; f.PReadBuffer = rd; f.PWriteBuffer = wr;
    f.FreeBuffers();
    bool ok = std::strcmp(g_log, "<dgg>") == 0 && g_globalFreed[0] == rd && g_globalFreed[1] == wr && f.PReadBuffer == 0 && f.PWriteBuffer == 0 && f.BufferChunks.m_pFirst == 0;
    g_log[0] = 0;
    f.FreeBuffers();                                  // nothing left to free
    ok = ok && std::strcmp(g_log, "<d>") == 0 && g_globalFrees == 2;
    if (!ok) { std::printf("009c5110_TEST FAIL\n"); return 1; }
    std::printf("009c5110_TEST PASS\n");
    return 0;
}