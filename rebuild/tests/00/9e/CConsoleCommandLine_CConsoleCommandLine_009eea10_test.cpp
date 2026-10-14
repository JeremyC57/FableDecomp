#include <cstdio>
struct CCharStringData { char* PData; unsigned long StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { }
    long GetLength() const
    {
        if (PStringData == 0)
            return 0;
        return PStringData->StringLength;
    }
};
struct CBase { virtual void v00() { } CBase() { } };
struct CConsoleCommandLine : CBase {
    virtual void v00() { }
    CCharString CurrentEntry;
    bool        UserEntry;
    long        CursorPosition;
    CConsoleCommandLine(const CCharString& entry);
};
CConsoleCommandLine::CConsoleCommandLine(const CCharString& entry)
    : CBase(), CurrentEntry(entry), UserEntry(false), CursorPosition(entry.GetLength())
{
}
int main()
{
    char text[] = "hello";
    CCharStringData data = { text, 5 };
    CCharString s; s.PStringData = &data;
    CConsoleCommandLine a(s);
    CCharString empty;
    CConsoleCommandLine b(empty);
    if (a.CursorPosition != 5 || a.UserEntry || a.CurrentEntry.PStringData != &data) { std::printf("009eea10_TEST FAIL\n"); return 1; }
    if (b.CursorPosition != 0 || b.CurrentEntry.PStringData != 0) { std::printf("009eea10_TEST FAIL\n"); return 1; }
    std::printf("009eea10_TEST PASS\n");
    return 0;
}