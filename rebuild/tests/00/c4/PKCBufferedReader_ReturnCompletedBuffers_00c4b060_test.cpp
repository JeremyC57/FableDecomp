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
// Behaviour test for 0x00C4B060 PKCBufferedReader::ReturnCompletedBuffers
struct PKCMailbox { int m_count; int GetCount() { return m_count; } };
struct PKCBufferedReader {
    PKCMailbox m_mailbox; int m_outstanding; int m_blockAt; int m_returned;
    void ReturnBuffer(bool* more) { --m_mailbox.m_count; --m_outstanding; ++m_returned; *more = m_returned != m_blockAt; }
    void ReturnCompletedBuffers()
    {
        while (m_outstanding != 0) {
            if (m_mailbox.GetCount() == 0) break;
            bool more;
            ReturnBuffer(&more);
            if (!more) TRACE(".\\PKCBufferedReader.cpp", 471, "Error: Should not be blocked here...")
        }
    }
};
int main()
{
    PKCBufferedReader r; r.m_mailbox.m_count = 2; r.m_outstanding = 5; r.m_blockAt = -1; r.m_returned = 0;
    r.ReturnCompletedBuffers();                                     // stops when the mailbox is empty
    if (r.m_returned != 2 || r.m_outstanding != 3 || g_breaks != 0) return Fail("00c4b060");
    r.m_mailbox.m_count = 9; r.m_blockAt = 4;
    r.ReturnCompletedBuffers();                                     // stops when nothing is outstanding
    if (r.m_outstanding != 0 || r.m_returned != 5 || g_breaks != 1) return Fail("00c4b060");
    if (!std::strstr(g_debugText, "(471) : Error: Should not be blocked here...\n")) return Fail("00c4b060");
    std::printf("00c4b060_TEST PASS\n");
    return 0;
}