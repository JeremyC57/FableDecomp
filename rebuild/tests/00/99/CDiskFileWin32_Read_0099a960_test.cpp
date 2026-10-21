#include <cstdio>
#include "engine/CDiskFileWin32.h"

extern "C" __declspec(dllimport) int __stdcall ReadFile(void* file, void* buffer, unsigned long toRead,
                                                         unsigned long* read, void* overlapped);   // 0x0143FED0

struct CCharString {
    char* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    operator const char*() const;                  // 0x0099E4C0
};

__forceinline void ReportFileError(CCharString message)
{
    (const char*)message;
}

struct CExceptionBase { };

struct CBBBFileException : CExceptionBase {
    long Error;
    CBBBFileException(long error) : Error(error) { }
    __forceinline CBBBFileException(long error, const CCharString& message) : Error(error) { ReportFileError(message); }
};

enum {
    BBB_FILE_ERROR_NULL_DESTINATION = 1,
    BBB_FILE_ERROR_READ_PAST_END    = 3,
    BBB_FILE_ERROR_READ_FAILED      = 4
};

class CASuspendableProcess;

struct CDiskFileWin32_Methods : CDiskFileWin32 {
    void Read(void* destination, long size, CASuspendableProcess* process);
};

void CDiskFileWin32_Methods::Read(void* destination, long size, CASuspendableProcess* process)
{
    if (destination == 0) {
        CCharString message("Null dst parameter", -1);
        throw CBBBFileException(BBB_FILE_ERROR_NULL_DESTINATION, message);
    }
    if (size != 0) {
        if (Position == Length) {
            CCharString message("Cannot read past end of file", -1);
            throw CBBBFileException(BBB_FILE_ERROR_READ_PAST_END, message);
        }
        unsigned long bytesRead;
        if (ReadFile(WinFileHandle, destination, size, &bytesRead, 0) != 1) {
            CCharString message("Cannot read file", -1);
            throw CBBBFileException(BBB_FILE_ERROR_READ_FAILED, message);
        }
        Position += bytesRead;
    }
}

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
static char g_lastMessage[64]; static int g_reads = 0; static int g_readResult = 1; static unsigned long g_readCount = 0;
static void* g_readFile; static void* g_readBuffer; static unsigned long g_readSize; static void* g_overlapped;
CCharString::CCharString(const char* text, long length) { strcpy(g_lastMessage, text); m_pData = 0; }
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) {}
CCharString::~CCharString() {}
CCharString::operator const char*() const { return g_lastMessage; }
extern "C" int __stdcall ReadFile(void* file, void* buffer, unsigned long toRead, unsigned long* read, void* overlapped)
{
    ++g_reads; g_readFile = file; g_readBuffer = buffer; g_readSize = toRead; g_overlapped = overlapped;
    *read = g_readCount;
    return g_readResult;
}
static long Try(CDiskFileWin32_Methods& f, void* dst, long size)
{
    try { f.Read(dst, size, 0); }
    catch (CBBBFileException& e) { return e.Error; }
    return -1;
}
int main()
{
    static CDiskFileWin32_Methods f;
    char buf[16];
    f.Position = 10; f.Length = 100; f.WinFileHandle = (void*)0x1234;
    check(Try(f, 0, 4) == 1 && strcmp(g_lastMessage, "Null dst parameter") == 0 && g_reads == 0, "null destination");
    check(Try(f, buf, 0) == -1 && g_reads == 0 && f.Position == 10, "zero size no-op");
    f.Position = 100;
    check(Try(f, buf, 4) == 3 && strcmp(g_lastMessage, "Cannot read past end of file") == 0 && g_reads == 0, "read at end");
    f.Position = 10; g_readResult = 0;
    check(Try(f, buf, 4) == 4 && strcmp(g_lastMessage, "Cannot read file") == 0 && g_reads == 1, "read failure");
    check(f.Position == 10, "no advance on failure");
    g_readResult = 1; g_readCount = 3;
    check(Try(f, buf, 4) == -1 && g_reads == 2, "read ok");
    check(g_readFile == (void*)0x1234 && g_readBuffer == buf && g_readSize == 4 && g_overlapped == 0, "ReadFile args");
    check(f.Position == 13, "advance by bytes read");
    if (!g_fail) printf("0099a960_TEST PASS\n");
    return g_fail;
}