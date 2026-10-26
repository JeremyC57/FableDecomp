// Behaviour test for CConsole::SetHelpBarString (0x009EB2C0): an identifier naming a registered input, alone or
// followed by "(", shows that input's help; anything else clears the help bar.
#include <cstdio>
#include <cstring>
#include <cctype>

class CCharString { public:
    char Buf[64];
    CCharString(const char* s = "") { std::strcpy(Buf, s); }
    const char* GetData() const { return Buf; }
    bool operator==(char c) const { char text[2] = { c, 0 }; return std::strcmp(Buf, text) == 0; }
};
static CCharString g_OpenBracket("(");
class CParsedItem { public: long Type; CCharString Text; const CCharString& GetString() { return Text; } const CCharString& GetSymbol() { return Text; } };
class CStringParser { public:                       // a tiny tokeniser: identifiers (2) and single-character symbols (1)
    char storage[64];
    const char* p;
    CStringParser(const CCharString& text) { std::strcpy(storage, text.Buf); p = storage; }
    bool ReadNextItem(CParsedItem& item)
    {
        while (*p == ' ') ++p;
        if (!*p) return false;
        int n = 0;
        if (std::isalpha((unsigned char)*p)) { while (std::isalnum((unsigned char)*p)) item.Text.Buf[n++] = *p++; item.Type = 2; }
        else { item.Text.Buf[n++] = *p++; item.Type = 1; }
        item.Text.Buf[n] = 0;
        return true;
    }
};
class CConsoleInputBase { public: const char* Name; const char* Help; CCharString GetHelp() const { return CCharString(Help); } };
class CConsoleCommandLine { public: CCharString CurrentEntry; CCharString GetEntry() const { return CurrentEntry; } };
class CConsole { public:
    CConsoleCommandLine FrontCommandLine;
    CCharString HelpBarString;
    CConsoleInputBase* Inputs; int Count;
    CConsoleInputBase* FindInput(const CCharString& name)
    {
        for (int i = 0; i < Count; ++i) if (std::strcmp(Inputs[i].Name, name.Buf) == 0) return &Inputs[i];
        return 0;
    }
    void SetHelpBarString();
};

void CConsole::SetHelpBarString()
{
    CStringParser parser(FrontCommandLine.GetEntry());
    CParsedItem item;
    if (parser.ReadNextItem(item) && item.Type == 2) {
        CConsoleInputBase* input = FindInput(item.GetString());
        if (!(parser.ReadNextItem(item) && item.Type == 1) || item.GetSymbol() == g_OpenBracket.GetData()[0]) {
            if (input) {
                HelpBarString = input->GetHelp();
                return;
            }
        }
    }
    HelpBarString = "";
}

static const char* Run(CConsole& c, const char* line)
{
    c.FrontCommandLine.CurrentEntry = CCharString(line);
    c.HelpBarString = CCharString("stale");
    c.SetHelpBarString();
    return c.HelpBarString.Buf;
}

int main()
{
    CConsoleInputBase inputs[2] = { { "spawn", "spawn(name)" }, { "god", "god mode" } };
    CConsole c; c.Inputs = inputs; c.Count = 2;
    bool ok = std::strcmp(Run(c, "spawn"), "spawn(name)") == 0
        && std::strcmp(Run(c, "spawn(troll"), "spawn(name)") == 0
        && std::strcmp(Run(c, "god ="), "") == 0
        && std::strcmp(Run(c, "nothing"), "") == 0
        && std::strcmp(Run(c, "(god"), "") == 0
        && std::strcmp(Run(c, "god other"), "god mode") == 0;
    if (!ok) { std::printf("009eb2c0_TEST FAIL\n"); return 1; }
    std::printf("009eb2c0_TEST PASS\n");
    return 0;
}