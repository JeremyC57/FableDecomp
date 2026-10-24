// Behaviour test for 00c17740: CAutoDeletePointer::GetChecked -- returns the held pointer; when it is null, first traces "PKCAutoDelete.h(32) : Should have checked first..." through the error sink and breaks
#include <cstdio>
#include <cstring>

static char g_text[256];
static int g_reports = 0, g_breaks = 0;
static void (*g_pfnErrorReport)(const char*) = 0;
static void Report(const char* text) { ++g_reports; std::strcpy(g_text, text); }

struct CDebugStream {
    char m_buffer[256];
    CDebugStream() { m_buffer[0] = 0; }
    CDebugStream& operator<<(const char* text) { std::strcat(m_buffer, text); return *this; }
    CDebugStream& operator<<(long value) { char n[16]; std::sprintf(n, "%ld", value); std::strcat(m_buffer, n); return *this; }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct CAutoDeletePointer {
    void* m_pObject;
    void* GetChecked()
    {
        if (m_pObject == 0) {
            {
                CDebugStream s;
                s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32L << ") : " << "Should have checked first..." << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_buffer);
            }
            CDebugBreak();
        }
        return m_pObject;
    }
};

int main()
{
    int object = 0;
    CAutoDeletePointer p; p.m_pObject = &object;
    if (p.GetChecked() != &object || g_breaks != 0) { std::printf("00c17740_TEST FAIL\n"); return 1; }
    p.m_pObject = 0;
    if (p.GetChecked() != 0 || g_breaks != 1 || g_reports != 0) { std::printf("00c17740_TEST FAIL\n"); return 1; }   // no sink: break only
    g_pfnErrorReport = Report;
    if (p.GetChecked() != 0 || g_breaks != 2 || g_reports != 1) { std::printf("00c17740_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_text, "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h(32) : Should have checked first...\n") != 0) { std::printf("00c17740_TEST FAIL\n"); return 1; }
    std::printf("00c17740_TEST PASS\n");
    return 0;
}