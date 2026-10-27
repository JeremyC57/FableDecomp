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
// Behaviour test for 0x00C4AB90 PKCBufferedReader::OnReadComplete
static unsigned int g_sent[4]; static int g_nSent = 0;
struct CMailbox { int Send(void* message) { g_sent[g_nSent++] = *(unsigned int*)message; return 0; } };
struct PKCBufferedReader {
    CMailbox m_results;
    void OnReadComplete(bool success, unsigned char* pRequestId)
    {
        if (!success) TRACE(".\\PKCBufferedReader.cpp", 5, "Error: Read failed")
        unsigned int id = *pRequestId;
        m_results.Send(&id);
    }
};
int main()
{
    PKCBufferedReader r; unsigned char id = 0xC8;
    r.OnReadComplete(true, &id);
    if (g_nSent != 1 || g_sent[0] != 0xC8 || g_breaks != 0) return Fail("00c4ab90");
    id = 3; r.OnReadComplete(false, &id);
    if (g_nSent != 2 || g_sent[1] != 3 || g_breaks != 1 || !std::strstr(g_debugText, "(5) : Error: Read failed\n")) return Fail("00c4ab90");
    std::printf("00c4ab90_TEST PASS\n");
    return 0;
}