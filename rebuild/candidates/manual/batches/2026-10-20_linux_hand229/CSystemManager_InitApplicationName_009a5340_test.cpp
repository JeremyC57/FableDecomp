// Behaviour test for CSystemManager::InitApplicationName (0x009A5340): the executable name from the command line.
#include <cstdio>
#include <cstring>
#include <wchar.h>

static const wchar_t* g_commandLine = 0;
static const wchar_t* GetCommandLineW_stub() { return g_commandLine; }

struct CCharString { char text[300]; CCharString() { text[0] = 0; } };
struct CWideString {
    const wchar_t* w;
    CWideString(const wchar_t* t) : w(t) {}
    CCharString ConvertToCharString() const
    {
        CCharString r; int i = 0;
        for (; w[i]; ++i) r.text[i] = (char)w[i];
        r.text[i] = 0;
        return r;
    }
};
struct CSystemManager {
    CCharString ApplicationName;
    void InitApplicationName()
    {
        wchar_t commandLine[261];
        wchar_t exeName[261];
        wchar_t terminator = L' ';
        memset(exeName, 0, sizeof(exeName));
        wcsncpy(commandLine, GetCommandLineW_stub(), 261);
        if (commandLine[0] == L'"') {
            terminator = L'"';
            long length = (long)wcslen(commandLine);
            for (long i = 0; i < length; ++i)
                commandLine[i] = commandLine[i + 1];
        }
        wchar_t* p = commandLine;
        while (*p && *p != terminator)
            ++p;
        *p = 0;
        wchar_t* slash = wcsrchr(commandLine, L'\\');
        if (slash) {
            wcscpy(exeName, slash + 1);
            ApplicationName = CWideString(exeName).ConvertToCharString();
        }
    }
};

int main()
{
    int ok = 1;
    CSystemManager m;
    g_commandLine = L"\"C:\\Program Files\\Fable\\Fable.exe\" -nointro";
    m.InitApplicationName();
    if (std::strcmp(m.ApplicationName.text, "Fable.exe") != 0) ok = 0;
    CSystemManager n;
    g_commandLine = L"D:\\Games\\fable.exe -windowed";
    n.InitApplicationName();
    if (std::strcmp(n.ApplicationName.text, "fable.exe") != 0) ok = 0;
    CSystemManager q;
    g_commandLine = L"fable.exe";            // no path: the name is left alone
    q.InitApplicationName();
    if (q.ApplicationName.text[0] != 0) ok = 0;
    if (!ok) { std::printf("009a5340_TEST FAIL\n"); return 1; }
    std::printf("009a5340_TEST PASS\n");
    return 0;
}
