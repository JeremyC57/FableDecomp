// Behaviour test for 0x0122A790 NProfileTimer's atexit destructor of LogFile: the inline ~CDiskFileWin32 closes the file,
// then the Pathname string and the CBaseClassNonCopyable base are destroyed, in that order.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CWideString { public: ~CWideString() { Log("P"); } void* PStringData; };
class CBaseClassNonCopyable { protected: CBaseClassNonCopyable() { } virtual ~CBaseClassNonCopyable() { Log("B"); } };
class CAFile : public CBaseClassNonCopyable { public: virtual ~CAFile() { } virtual void Close() = 0; };
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close() { Log("C"); Opened = false; }
    bool        Opened;
    CWideString Pathname;
};
int main()
{
    g_log[0] = 0;
    {
        CDiskFileWin32 logFile;                 // NProfileTimer::LogFile; its scope end stands for the atexit call
        logFile.Opened = true;
    }
    if (std::strcmp(g_log, "CPB") != 0) { std::printf("0122a790_TEST FAIL\n"); return 1; }
    std::printf("0122a790_TEST PASS\n");
    return 0;
}