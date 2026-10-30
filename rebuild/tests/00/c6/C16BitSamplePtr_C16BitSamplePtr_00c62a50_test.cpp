// Behaviour test for C16BitSamplePtr::C16BitSamplePtr (0x00C62A50): the buffer and channel count are stored; a null buffer
// logs ".\C16BitSamplePtr.cpp(8) : Null buffer\n" and a zero channel count ".\C16BitSamplePtr.cpp(9) : Expecting at least
// 1 channel!\n" through the report sink, each followed by the trace break; valid arguments log nothing.
#include <cstdio>
#include <cstring>

static char g_last[256];
static int g_reports = 0, g_breaks = 0;

struct CDebugStream {
    char m_text[256];
    char* m_pBuffer;
    CDebugStream(const char* init) : m_pBuffer(m_text) { std::strcpy(m_text, init); }
    CDebugStream& operator<<(const char* t) { std::strcat(m_text, t); return *this; }
    CDebugStream& operator<<(long v) { char b[16]; std::sprintf(b, "%ld", v); std::strcat(m_text, b); return *this; }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };
static void Report(const char* t) { ++g_reports; std::strcpy(g_last, t); }
static void (*g_pfnErrorReport)(const char*) = Report;

class C16BitSamplePtr {
public:
    short* m_pSamples;
    long   m_channels;
    C16BitSamplePtr(short* pSamples, long channels) : m_pSamples(pSamples), m_channels(channels)
    {
        if (m_pSamples == 0) {
            { CDebugStream s(""); s << ".\\C16BitSamplePtr.cpp" << "(" << 8L << ") : " << "Null buffer" << "\n";
              if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer); }
            CDebugBreak();
        }
        if (m_channels == 0) {
            { CDebugStream s(""); s << ".\\C16BitSamplePtr.cpp" << "(" << 9L << ") : " << "Expecting at least 1 channel!" << "\n";
              if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer); }
            CDebugBreak();
        }
    }
};

int main()
{
    int ok = 1;
    short buf[8];
    C16BitSamplePtr a(buf, 2);
    ok &= a.m_pSamples == buf && a.m_channels == 2 && g_reports == 0 && g_breaks == 0;
    C16BitSamplePtr b(0, 1);
    ok &= b.m_pSamples == 0 && g_reports == 1 && g_breaks == 1 && std::strcmp(g_last, ".\\C16BitSamplePtr.cpp(8) : Null buffer\n") == 0;
    C16BitSamplePtr c(buf, 0);
    ok &= c.m_channels == 0 && g_reports == 2 && g_breaks == 2 && std::strcmp(g_last, ".\\C16BitSamplePtr.cpp(9) : Expecting at least 1 channel!\n") == 0;
    C16BitSamplePtr d(0, 0);
    ok &= g_reports == 4 && g_breaks == 4;
    if (!ok) { std::printf("00c62a50_TEST FAIL\n"); return 1; }
    std::printf("00c62a50_TEST PASS\n");
    return 0;
}