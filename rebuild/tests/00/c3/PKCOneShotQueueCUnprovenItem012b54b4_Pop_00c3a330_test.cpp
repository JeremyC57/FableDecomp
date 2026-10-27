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
// Behaviour test for 0x00C3A330 PKCOneShotQueue<16-byte item>::Pop
struct Item { int a, b, c, d; };
struct PKCOneShotQueue {
    Item m_item; int m_bFilled;
    Item* Pop(Item* item)
    {
        if (m_bFilled == 0) TRACE("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h", 28, "Nothing to return!")
        m_bFilled = 0;
        *item = m_item;
        return item;
    }
};
int main()
{
    PKCOneShotQueue q; q.m_item.a = 1; q.m_item.b = 2; q.m_item.c = 3; q.m_item.d = 4; q.m_bFilled = 1;
    Item out = { 0, 0, 0, 0 };
    if (q.Pop(&out) != &out || out.a != 1 || out.d != 4 || q.m_bFilled != 0 || g_breaks != 0) return Fail("00c3a330");
    q.Pop(&out);
    if (g_breaks != 1 || g_reports != 1 || !std::strstr(g_debugText, "PKCOneShotQueue.h(28) : Nothing to return!\n")) return Fail("00c3a330");
    std::printf("00c3a330_TEST PASS\n");
    return 0;
}