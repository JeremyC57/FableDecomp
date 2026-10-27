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
// Behaviour test for 0x00C1DE50 PKCOneShotQueue<SFileReply>::Push
struct SFileReply { bool m_bSuccess; int m_value; };
struct PKCOneShotQueue {
    SFileReply m_item; int m_bFilled;
    void Push(const SFileReply* item)
    {
        if (m_bFilled != 0) TRACE("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h", 35, "Already filled this queue!")
        m_item = *item;
        m_bFilled = 1;
    }
};
int main()
{
    PKCOneShotQueue q; q.m_bFilled = 0; q.m_item.m_bSuccess = false; q.m_item.m_value = 0;
    SFileReply r; r.m_bSuccess = true; r.m_value = 77;
    q.Push(&r);
    if (q.m_bFilled != 1 || q.m_item.m_value != 77 || !q.m_item.m_bSuccess || g_reports != 0 || g_breaks != 0) return Fail("00c1de50");
    r.m_value = 5; q.Push(&r);
    if (q.m_item.m_value != 5 || g_reports != 1 || g_breaks != 1) return Fail("00c1de50");
    if (!std::strstr(g_debugText, "PKCOneShotQueue.h(35) : Already filled this queue!\n")) return Fail("00c1de50");
    std::printf("00c1de50_TEST PASS\n");
    return 0;
}