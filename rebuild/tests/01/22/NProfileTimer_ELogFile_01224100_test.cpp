#include <cstdio>
#include <cstring>
#include <cmath>
static int g_closed = 0;
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { } };
class CAFile { public: virtual ~CAFile() { } virtual void Close() = 0; };
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close() { ++g_closed; Opened = false; }
    int AccessType; int Position; int Length; void* WinFileHandle; bool Opened; CWideString Pathname;
};

int main()
{
    {
        CDiskFileWin32 logFile;
        if (logFile.Opened || logFile.Pathname.PStringData != 0) do { std::printf("01224100_TEST FAIL\n"); return 1; } while (0);
    }
    if (g_closed != 1) do { std::printf("01224100_TEST FAIL\n"); return 1; } while (0);
    std::printf("01224100_TEST PASS\n");
    return 0;
}