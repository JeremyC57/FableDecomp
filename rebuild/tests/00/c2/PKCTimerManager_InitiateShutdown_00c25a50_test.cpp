// behaviour test for 0x00C25A50 PKCTimerManager::InitiateShutdown: warn on a second shutdown, then set the flag
#include <cstdio>
#include <cstring>

static char g_log[256];
static int g_breaks = 0;
typedef void (*PFNLOGSINK)(const char* text);
static void Sink(const char* text) { std::strcat(g_log, text); }
static PFNLOGSINK g_pfnLogSink = Sink;

class CString {
public:
    char m_pchData[200];
    CString() { m_pchData[0] = 0; }
    CString* Append(const char* text) { std::strcat(m_pchData, text); return this; }
    CString* AppendNumber(long value) { char b[16]; std::sprintf(b, "%ld", value); return Append(b); }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct PKCTimerManager {
    bool m_bShutdown;
    void InitiateShutdown();
};

void PKCTimerManager::InitiateShutdown()
{
    if (m_bShutdown) {
        {
            CString line;
            line.Append(".\\PKCTimerManager.cpp")->Append("(")->AppendNumber(33)->Append(") : ")->Append("Shutdown already initiated...")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_pchData);
        }
        CDebugBreak();
    }
    m_bShutdown = true;
}

int main()
{
    PKCTimerManager m; m.m_bShutdown = false; g_log[0] = 0;
    m.InitiateShutdown();
    if (!m.m_bShutdown || g_breaks != 0 || g_log[0] != 0) { std::printf("00c25a50_TEST FAIL\n"); return 1; }
    m.InitiateShutdown();
    if (!m.m_bShutdown || g_breaks != 1 || std::strcmp(g_log, ".\\PKCTimerManager.cpp(33) : Shutdown already initiated...\n") != 0) { std::printf("00c25a50_TEST FAIL\n"); return 1; }
    std::printf("00c25a50_TEST PASS\n");
    return 0;
}