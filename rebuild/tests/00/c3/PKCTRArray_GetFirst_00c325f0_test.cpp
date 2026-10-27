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
// Behaviour test for 0x00C325F0 PKCTRArray::GetFirst
struct PKCTRArray {
    void** m_pEntries; unsigned int m_capacity; unsigned int m_count;
    void* GetFirst()
    {
        if (m_count == 0) return 0;
        if (m_pEntries[0] == 0) TRACE(".\\PKCTRArray.cpp", 204, "Error: Null item")
        return m_pEntries[0];
    }
};
int main()
{
    int a = 1, b = 2; void* e[2] = { &a, &b };
    PKCTRArray t; t.m_pEntries = e; t.m_capacity = 2; t.m_count = 0;
    if (t.GetFirst() != 0 || g_breaks != 0) return Fail("00c325f0");
    t.m_count = 2;
    if (t.GetFirst() != &a || g_breaks != 0) return Fail("00c325f0");
    e[0] = 0;
    if (t.GetFirst() != 0 || g_breaks != 1 || !std::strstr(g_debugText, ".\\PKCTRArray.cpp(204) : Error: Null item\n")) return Fail("00c325f0");
    std::printf("00c325f0_TEST PASS\n");
    return 0;
}