// Behaviour test for CConsole::NextCommandGuess (0x009EB0D0): no step while the front line equals the user entry
// (case-insensitively), otherwise the next registered input with the entry as prefix becomes the front line, and
// running off the end (or a non-matching name) leaves guess mode.
#include <cstdio>
#include <cstring>
#include <cctype>

static int StrICmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = std::tolower((unsigned char)*a), cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (!ca) return 0;
    }
}
struct CCharString {
    char Buf[64];
    CCharString(const char* s = "") { std::strcpy(Buf, s); }
    const char* GetData() const { return Buf; }
    unsigned long GetLength() const { return (unsigned long)std::strlen(Buf); }
    CCharString ToLower() const { CCharString r(Buf); for (char* p = r.Buf; *p; ++p) *p = (char)std::tolower((unsigned char)*p); return r; }
    bool IsEqualNoCase(const CCharString& other) const { return StrICmp(GetData(), other.GetData()) == 0; }
};
struct CConsoleCommandLine {
    CCharString CurrentEntry; bool UserEntry; long CursorPosition;
    CConsoleCommandLine() : UserEntry(true), CursorPosition(0) {}
    CConsoleCommandLine(const CCharString& entry) : CurrentEntry(entry), UserEntry(false), CursorPosition((long)entry.GetLength()) {}
    CCharString GetEntry() const { return CurrentEntry; }
};
struct CConsoleInputBase { const char* Name; CCharString GetFormattedName() const { return CCharString(Name); } };
struct SInputNode { CCharString first; CConsoleInputBase* second; };
struct CConsole {
    CConsoleCommandLine FrontCommandLine, BackCommandLine;
    SInputNode* Inputs; int Count;          // the sorted map flattened; index Count is end()
    int GuessPos; bool GuessMode;
    void SetGuessMode() { GuessMode = false; }
    void NextCommandGuess();
};

void CConsole::NextCommandGuess()
{
    CCharString userEntry = BackCommandLine.GetEntry().ToLower();
    bool unchanged = FrontCommandLine.GetEntry().IsEqualNoCase(userEntry);
    if (unchanged)
        return;
    ++GuessPos;
    if (GuessPos != Count
        && std::strncmp(Inputs[GuessPos].first.GetData(), userEntry.GetData(), userEntry.GetLength()) == 0) {
        FrontCommandLine = Inputs[GuessPos].second->GetFormattedName();
    } else {
        SetGuessMode();
    }
}

int main()
{
    CConsoleInputBase a = { "SetSpeed" }, b = { "SetTime" }, c = { "Teleport" };
    SInputNode nodes[3] = { { CCharString("setspeed"), &a }, { CCharString("settime"), &b }, { CCharString("teleport"), &c } };
    CConsole con; con.Inputs = nodes; con.Count = 3; con.GuessPos = -1; con.GuessMode = true;
    con.BackCommandLine.CurrentEntry = CCharString("SET");
    con.FrontCommandLine.CurrentEntry = CCharString("set");          // equal ignoring case: no step
    con.NextCommandGuess();
    bool ok = con.GuessPos == -1 && con.GuessMode;
    con.FrontCommandLine.CurrentEntry = CCharString("x");
    con.NextCommandGuess();                                           // -> setspeed
    ok = ok && con.GuessPos == 0 && std::strcmp(con.FrontCommandLine.CurrentEntry.GetData(), "SetSpeed") == 0
        && !con.FrontCommandLine.UserEntry && con.FrontCommandLine.CursorPosition == 8;
    con.NextCommandGuess();                                           // -> settime
    ok = ok && con.GuessPos == 1 && std::strcmp(con.FrontCommandLine.CurrentEntry.GetData(), "SetTime") == 0;
    con.NextCommandGuess();                                           // teleport does not match: leave guess mode
    ok = ok && con.GuessPos == 2 && !con.GuessMode && std::strcmp(con.FrontCommandLine.CurrentEntry.GetData(), "SetTime") == 0;
    if (!ok) { std::printf("009eb0d0_TEST FAIL\n"); return 1; }
    std::printf("009eb0d0_TEST PASS\n");
    return 0;
}