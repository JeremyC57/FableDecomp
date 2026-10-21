// Behaviour test for CConsole::RunTextCommand (0x009EC710): ';'-separated commands run one by one through the
// front command line; a ';' inside quotes is kept in the command.
#include <cstdio>
#include <cstring>

struct CCharString {
    char Buf[128];
    CCharString(const char* s = "") { std::strcpy(Buf, s); }
    unsigned long GetLength() const { return (unsigned long)std::strlen(Buf); }
    CCharString& operator+=(const CCharString& o) { std::strcat(Buf, o.Buf); return *this; }
    CCharString& operator+=(const char* t) { std::strcat(Buf, t); return *this; }
};
struct CCursorPos { bool InQuotes; };
struct CParsedItem { CCharString String; const CCharString& GetString() { return String; } };
struct CStringParser {                       // splits at ';', tracking whether the split point is inside quotes
    char Text[128]; const char* p; CCursorPos CursorPos;
    CStringParser(const CCharString& text) { std::strcpy(Text, text.Buf); p = Text; CursorPos.InQuotes = false; }
    bool ReadAsStringUntilString(const CCharString& until, CParsedItem& item)
    {
        if (!*p) return false;
        int n = 0;
        while (*p && *p != until.Buf[0]) { if (*p == '"') CursorPos.InQuotes = !CursorPos.InQuotes; item.String.Buf[n++] = *p++; }
        item.String.Buf[n] = 0;
        return true;
    }
    bool ReadNextItem(CParsedItem& item) { if (*p) { item.String.Buf[0] = *p++; item.String.Buf[1] = 0; return true; } return false; }
};
struct CConsoleCommandLine {
    CCharString CurrentEntry; bool UserEntry; long CursorPosition;
    void Reset() { CurrentEntry = ""; UserEntry = false; CursorPosition = 0; }
};
struct CConsole {
    CConsoleCommandLine FrontCommandLine;
    char Log[256]; int Runs; long LastCursor;
    void RunCommandLine() { ++Runs; LastCursor = FrontCommandLine.CursorPosition; std::strcat(Log, FrontCommandLine.CurrentEntry.Buf); std::strcat(Log, "|"); }
    void RunTextCommand(const CCharString& text);
};

void CConsole::RunTextCommand(const CCharString& text)
{
    CStringParser parser(text);
    CParsedItem item;
    CCharString command;
    while (parser.ReadAsStringUntilString(";", item)) {
        command += item.GetString();
        if (!parser.CursorPos.InQuotes) {
            FrontCommandLine.CurrentEntry = command;
            FrontCommandLine.CursorPosition = command.GetLength();
            RunCommandLine();
            FrontCommandLine.Reset();
            command = "";
        } else {
            command += ";";
        }
        parser.ReadNextItem(item);
    }
}

int main()
{
    CConsole c; c.Log[0] = 0; c.Runs = 0; c.LastCursor = -1; c.FrontCommandLine.UserEntry = true; c.FrontCommandLine.CursorPosition = 3;
    c.RunTextCommand(CCharString("a(1);say(\"x;y\");b"));
    bool ok = c.Runs == 3 && std::strcmp(c.Log, "a(1)|say(\"x;y\")|b|") == 0 && c.LastCursor == 1
        && c.FrontCommandLine.CurrentEntry.Buf[0] == 0 && !c.FrontCommandLine.UserEntry && c.FrontCommandLine.CursorPosition == 0;
    if (!ok) { std::printf("009ec710_TEST FAIL\n"); return 1; }
    std::printf("009ec710_TEST PASS\n");
    return 0;
}