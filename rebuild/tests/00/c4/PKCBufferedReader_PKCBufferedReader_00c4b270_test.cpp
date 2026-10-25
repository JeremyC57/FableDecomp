// Behaviour test for 00c4b270: PKCBufferedReader::~PKCBufferedReader -- buffers returned, both arrays freed, then semaphore, lock, queue
#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
static void Log(const char* s) { std::strcat(g_seq, s); }
class PKCCriticalSection { public: PKCCriticalSection() { } ~PKCCriticalSection() { Log("L"); } unsigned char m_section[0x18]; };
class PKCSemaphore { public: PKCSemaphore() : m_handle(0) { } ~PKCSemaphore() { Log("S"); } void* m_handle; };
class PKCReadRequestQueue { public: PKCReadRequestQueue() { } ~PKCReadRequestQueue() { Log("Q"); } virtual void Pop() { } unsigned char _body04[0x10]; };
class PKCReadSignal : public PKCCriticalSection { public: PKCReadRequestQueue* m_pQueue; PKCSemaphore m_semaphore; };
class PKCBufferedReader {
public:
    PKCBufferedReader() : m_pBufferMemory(new unsigned char[1]), m_pBufferStates(new unsigned char[1]), m_outstanding(3) { }
    ~PKCBufferedReader() { ReturnAllBuffers(); delete[] m_pBufferMemory; delete[] m_pBufferStates; Log("F"); }
    virtual void v00() { }
    void ReturnBuffer(long) { Log("R"); --m_outstanding; }
    void ReturnAllBuffers() { while (m_outstanding != 0) ReturnBuffer(0); }
    unsigned char* m_pBufferMemory; unsigned long m_blockSize, m_0c, m_lookBehind, m_bufferCount; void* m_pSource;
    unsigned char* m_pBufferStates; unsigned long m_size, m_firstBlock, m_28, m_blockCount, m_position;
    PKCReadRequestQueue m_requests; PKCReadSignal m_signal; unsigned long m_outstanding; unsigned long m_6c;
};
int main()
{
    PKCBufferedReader* r = new PKCBufferedReader;
    delete r;
    int ok = std::strcmp(g_seq, "RRRFSLQ") == 0 && sizeof(PKCBufferedReader) == 0x70 && sizeof(PKCReadSignal) == 0x20;
    std::printf(ok ? "00c4b270_TEST PASS\n" : "00c4b270_TEST FAIL\n");
    return ok ? 0 : 1;
}
