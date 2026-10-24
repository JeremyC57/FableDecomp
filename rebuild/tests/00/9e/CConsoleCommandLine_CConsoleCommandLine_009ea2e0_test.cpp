// behaviour test for 009ea2e0: CConsoleCommandLine's copy constructor default-constructs the CBase base,
// installs its own vtable, deep-copies the current entry and copies the user-entry flag and cursor.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

static long g_baseCtors = 0;

struct CBaseClass {
    virtual long Kind() const { return 1; }
    CBaseClass() { ++g_baseCtors; }
};

struct CConsoleCommandLine : CBaseClass {
    CCharString CurrentEntry;
    bool        UserEntry;
    long        CursorPosition;
    virtual long Kind() const { return 2; }
    CConsoleCommandLine() : CurrentEntry("give gold 100"), UserEntry(false), CursorPosition(0) { }
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CBaseClass(), CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition)
    {
    }
};

int main()
{
    int ok = 1;
    CConsoleCommandLine source;
    source.UserEntry = true;
    source.CursorPosition = 7;
    g_baseCtors = 0;
    CConsoleCommandLine copy(source);
    if (g_baseCtors != 1 || copy.Kind() != 2)
        ok = 0;
    if (std::strcmp(copy.CurrentEntry.PStringData, "give gold 100") != 0 || copy.CurrentEntry.PStringData == source.CurrentEntry.PStringData)
        ok = 0;
    if (!copy.UserEntry || copy.CursorPosition != 7)
        ok = 0;
    if (!ok) {
        std::printf("009ea2e0_TEST FAIL\n");
        return 1;
    }
    std::printf("009ea2e0_TEST PASS\n");
    return 0;
}