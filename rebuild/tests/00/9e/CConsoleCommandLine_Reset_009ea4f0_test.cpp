#include <cstdio>
#include <cstring>
struct CCharString {
    void* PStringData;
    const char* assigned;
    const CCharString& operator=(const char* text) { assigned = text; return *this; }
};
struct CConsoleCommandLine {
    virtual void v00() { }
    CCharString CurrentEntry;
    bool        UserEntry;
    long        CursorPosition;
    void Reset();
};
void CConsoleCommandLine::Reset()
{
    CurrentEntry = "";
    UserEntry = false;
    CursorPosition = 0;
}
int main()
{
    CConsoleCommandLine line;
    line.CurrentEntry.assigned = "abc";
    line.UserEntry = true;
    line.CursorPosition = 7;
    line.Reset();
    if (std::strcmp(line.CurrentEntry.assigned, "") != 0 || line.UserEntry || line.CursorPosition != 0) { std::printf("009ea4f0_TEST FAIL\n"); return 1; }
    std::printf("009ea4f0_TEST PASS\n");
    return 0;
}