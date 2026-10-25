// Behaviour test for 00c31db0: PKCAutoFile::GetChecked -- returns the held file pointer; when it is null, first traces ".\PKCAutoFile.cpp(26) : Should have checked first..." through the error sink and breaks
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

struct PKCAutoFile {
    void* m_pFile;
    void* GetChecked()
    {
        if (m_pFile == 0) {
            {
                CDebugStream s;
                s << ".\\PKCAutoFile.cpp" << "(" << 26L << ") : " << "Should have checked first..." << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_buffer);
            }
            CDebugBreak();
        }
        return m_pFile;
    }
};

int main()
{
    int file = 0;
    PKCAutoFile f; f.m_pFile = &file;
    if (f.GetChecked() != &file || g_breaks != 0) { std::printf("00c31db0_TEST FAIL\n"); return 1; }
    f.m_pFile = 0;
    if (f.GetChecked() != 0 || g_breaks != 1 || g_reports != 0) { std::printf("00c31db0_TEST FAIL\n"); return 1; }   // no sink: break only
    g_pfnErrorReport = Report;
    if (f.GetChecked() != 0 || g_breaks != 2 || g_reports != 1) { std::printf("00c31db0_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_text, ".\\PKCAutoFile.cpp(26) : Should have checked first...\n") != 0) { std::printf("00c31db0_TEST FAIL\n"); return 1; }
    std::printf("00c31db0_TEST PASS\n");
    return 0;
}