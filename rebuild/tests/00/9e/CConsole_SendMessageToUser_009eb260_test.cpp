#include <cstdio>
#include <stddef.h>
struct CCharStringData { const char* PData; unsigned long StringLength; };
static int g_copies = 0, g_stringDtors = 0, g_baseCtors = 0, g_baseDtors = 0, g_adds = 0; static const char* g_addedText = 0; static bool g_addedUser = true; static long g_addedCursor = -1;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
    ~CCharString() { ++g_stringDtors; }
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->StringLength; }
};
struct CBaseClass { virtual void v00() { } CBaseClass() { ++g_baseCtors; } ~CBaseClass() { ++g_baseDtors; } };
struct CConsoleCommandLine : CBaseClass { CCharString CurrentEntry; bool UserEntry; long CursorPosition; virtual void v00() { }
    CConsoleCommandLine(const CCharString& text) : CBaseClass(), CurrentEntry(text), UserEntry(false), CursorPosition(text.GetLength()) { } };
struct CConsole_Retail {
    virtual void v00() { }
    void AddCommandLineToHistory(const CConsoleCommandLine& line) { ++g_adds; g_addedText = line.CurrentEntry.PStringData ? line.CurrentEntry.PStringData->PData : 0; g_addedUser = line.UserEntry; g_addedCursor = line.CursorPosition; }
    void SendMessageToUser(const CCharString& message) { CConsoleCommandLine line(message); AddCommandLineToHistory(line); }
};
int main()
{
    CCharStringData data = { "hello", 5 }; CCharString message(&data); CConsole_Retail console;
    console.SendMessageToUser(message);
    if (g_copies != 1 || g_baseCtors != 1 || g_adds != 1 || g_addedText != data.PData || g_addedUser || g_addedCursor != 5 || g_stringDtors != 1 || g_baseDtors != 1
        || offsetof(CConsoleCommandLine, CurrentEntry) != 4 || offsetof(CConsoleCommandLine, CursorPosition) != 0xc || sizeof(CConsoleCommandLine) != 0x10) { std::printf("009eb260_TEST FAIL\n"); return 1; }
    CCharString empty((CCharStringData*)0); console.SendMessageToUser(empty);
    if (g_adds != 2 || g_addedCursor != 0) { std::printf("009eb260_TEST FAIL\n"); return 1; }
    std::printf("009eb260_TEST PASS\n"); return 0;
}