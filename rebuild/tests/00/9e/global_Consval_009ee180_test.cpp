// behaviour test for 009ee180: placement-copy-construct a CConsoleCommandLine into raw storage: the CBase
// base is default-constructed, the derived vtable installed, the entry deep-copied, flag and cursor copied.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

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
    CConsoleCommandLine() : CurrentEntry("help"), UserEntry(false), CursorPosition(0) { }
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CBaseClass(), CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition)
    {
    }
};

static void Cons_val(CConsoleCommandLine* ptr, const CConsoleCommandLine& value)
{
    new ((void*)ptr) CConsoleCommandLine(value);
}

int main()
{
    int ok = 1;
    CConsoleCommandLine source;
    void* raw = ::operator new(sizeof(CConsoleCommandLine));
    CConsoleCommandLine* dest = (CConsoleCommandLine*)raw;
    source.UserEntry = true;
    source.CursorPosition = 3;
    g_baseCtors = 0;
    Cons_val(dest, source);
    if (g_baseCtors != 1 || dest->Kind() != 2 || !dest->UserEntry || dest->CursorPosition != 3)
        ok = 0;
    if (std::strcmp(dest->CurrentEntry.PStringData, "help") != 0 || dest->CurrentEntry.PStringData == source.CurrentEntry.PStringData)
        ok = 0;
    dest->~CConsoleCommandLine();
    ::operator delete(raw);
    if (!ok) {
        std::printf("009ee180_TEST FAIL\n");
        return 1;
    }
    std::printf("009ee180_TEST PASS\n");
    return 0;
}