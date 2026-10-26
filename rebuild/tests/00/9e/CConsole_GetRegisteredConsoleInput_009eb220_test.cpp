// Behaviour test for 0x009EB220 CConsole::GetRegisteredConsoleInput: the name is lower-cased before the lookup and a
// missing name gives null.
#include <cstdio>
#include <cstring>
#include <cctype>

static int gLive = 0;

class CCharString
{ public:
    char Text[32];
    CCharString(const char* s) { std::strcpy(Text, s); ++gLive; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++gLive; }
    ~CCharString() { --gLive; }
    CCharString ToLower() const
    {
        CCharString r(Text);
        for (char* p = r.Text; *p; ++p) *p = (char)std::tolower((unsigned char)*p);
        return r;
    }
};

class CConsoleInputBase { public: int Id; };

struct SInputNode { const char* first; CConsoleInputBase* second; };

static CConsoleInputBase gHelp = { 1 }, gQuit = { 2 };
static SInputNode gNodes[3] = { { "help", &gHelp }, { "quit", &gQuit }, { 0, 0 } };   // gNodes[2] is end()

struct CInputMap
{
    SInputNode* _M_header;
    SInputNode* find(const CCharString& key)
    {
        for (int i = 0; i < 2; ++i) if (std::strcmp(gNodes[i].first, key.Text) == 0) return &gNodes[i];
        return _M_header;
    }
};

class CConsole
{ public:
    CInputMap RegisteredConsoleInputs;
    SInputNode* FindInput(const CCharString& key) { return RegisteredConsoleInputs.find(key); }
    CConsoleInputBase* GetRegisteredConsoleInput(const CCharString& name)
    {
        SInputNode* it = FindInput(name.ToLower());
        if (it != RegisteredConsoleInputs._M_header)
            return it->second;
        return 0;
    }
};

int main()
{
    CConsole console;
    console.RegisteredConsoleInputs._M_header = &gNodes[2];
    bool ok = console.GetRegisteredConsoleInput(CCharString("HeLp")) == &gHelp
           && console.GetRegisteredConsoleInput(CCharString("quit")) == &gQuit
           && console.GetRegisteredConsoleInput(CCharString("dance")) == 0;
    ok = ok && gLive == 0;
    if (!ok) { std::printf("009eb220_TEST FAIL\n"); return 1; }
    std::printf("009eb220_TEST PASS\n");
    return 0;
}