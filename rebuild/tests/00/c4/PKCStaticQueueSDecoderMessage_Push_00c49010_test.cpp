#include <cstdio>
#include <cstring>
// stand-ins for the libpk trace stream / error hook / debug break (0x00C0DEC0, 0x00C0DF00, 0x0143BB2C, 0x00C16750)
static char g_debugText[512];
static char* g_pDebugBuffer = g_debugText;
static int g_reports = 0, g_breaks = 0;
static void ReportStub(const char*) { ++g_reports; }
static void (*g_pfnErrorReport)(const char*) = ReportStub;
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { m_pBuffer[0] = 0; }
    CDebugStream& operator<<(const char* text) { std::strcat(m_pBuffer, text); return *this; }
    CDebugStream& operator<<(long value) { char n[16]; std::sprintf(n, "%ld", value); std::strcat(m_pBuffer, n); return *this; }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };
#define TRACE(file, line, msg) { { CDebugStream s(g_pDebugBuffer); s << file << "(" << (long)line << ") : " << msg << "\n"; \
    if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer); } CDebugBreak(); }
static int Fail(const char* a) { std::printf("%s_TEST FAIL\n", a); return 1; }
// Behaviour test for 0x00C49010 PKCStaticQueue<SDecoderMessage>::Push
struct Msg { int m_type; int m_param; };
struct PKCStaticQueue {
    unsigned int m_capacity; Msg* m_pRecords; unsigned int m_count; unsigned int m_head;
    int Push(const Msg* item)
    {
        if (m_count >= m_capacity) TRACE("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h", 57, "This queue is full!")
        m_pRecords[m_head] = *item;
        m_head = (m_head + 1) % m_capacity;
        return ++m_count;
    }
};
int main()
{
    Msg recs[3]; PKCStaticQueue q; q.m_capacity = 3; q.m_pRecords = recs; q.m_count = 0; q.m_head = 0;
    for (int i = 0; i < 3; ++i) { Msg m; m.m_type = i + 10; m.m_param = i; if (q.Push(&m) != i + 1) return Fail("00c49010"); }
    if (q.m_head != 0 || recs[2].m_type != 12 || g_breaks != 0) return Fail("00c49010");
    Msg m; m.m_type = 99; m.m_param = 0;
    if (q.Push(&m) != 4 || recs[0].m_type != 99 || q.m_head != 1 || g_breaks != 1) return Fail("00c49010");
    if (!std::strstr(g_debugText, "PKCStaticQueue.h(57) : This queue is full!\n")) return Fail("00c49010");
    std::printf("00c49010_TEST PASS\n");
    return 0;
}