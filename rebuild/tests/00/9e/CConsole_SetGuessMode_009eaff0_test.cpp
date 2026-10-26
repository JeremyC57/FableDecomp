// CConsole::SetGuessMode() -- enter command-guess mode: look the lower-cased current line up in the command map and, when the found command starts with it, take that command's text as the guessed command line
// Leaf: SetGuessMode
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EAFF0, 221 bytes (224 with padding).
// Callees: CCharString copy ctor 0x0099EC30, CCharString::GetAsLowerCase 0x0099ED10 (result by hidden pointer; PDB ?GetAsLowerCase@CCharString@@QBE?AV1@XZ), ~CCharString
//   0x0099EAE0, the command map's _Lbound 0x009EEC10 (Dinkumware _Tree<CCharString, ...>), strncmp 0x00BFEAF8, the command's
//   virtual slot 2 (+8, its text by value), CConsoleCommandLine::CConsoleCommandLine(const CCharString&) 0x009EEA10,
//   CCharString::operator= 0x0099EFB0, CBase::~CBase 0x0099A2E0 (the command line's base); "" @ 0x0129AAF4.
// Layout: CConsole: GuessLine (CConsoleCommandLine: vptr, Text +4, flag +8, cursor +0xc) +0x1c, CurrentLine +0x30, the
//   command map +0x40 (header node first), GuessMode +0x65, GuessIterator +0x68; map node key +0x10, value +0x14.
// Shape: c_str()/Length() are the null-first inline accessors (merged null test). The lower-cased line comes from a
//   by-value helper (the CurrentLine copy is built before the hidden-result address, then destroyed right after the call);
//   the guess goes through a forced-inline SetGuessLine(const CCharString&) so the command-text temporary outlives the
//   named CConsoleCommandLine (destroyed text, ~CBase, then the temporary) while the ctor still takes the returned pointer.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

class CCharStringData { public:
    const char*   Text;                                     // +0x00
    unsigned long Length;                                   // +0x04
};

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    CCharString GetAsLowerCase() const;                           // 0x0099ED10
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->Text; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
};

struct CBase {
    virtual void v00();
    ~CBase();                                               // 0x0099A2E0
};

class CConsoleCommandLine : public CBase { public:
    CCharString Text;                                       // +0x04
    bool          Flag;                                     // +0x08
    long          Cursor;                                   // +0x0c
    CConsoleCommandLine(const CCharString& text);           // 0x009EEA10
};

struct CConsoleCommand {
    virtual void v00();
    virtual void v01();
    virtual CCharString GetCommandText();                   // slot 2 (+8)
};

struct SCommandNode {
    SCommandNode* _Left;
    SCommandNode* _Parent;
    SCommandNode* _Right;
    long          _Color;
    CCharString   first;                                    // +0x10
    CConsoleCommand* second;                                // +0x14
};

struct CCommandMap {                                        // Dinkumware map<CCharString, CConsoleCommand*>
    SCommandNode* _Myhead;                                  // +0x00
    unsigned long _Mysize;
    SCommandNode* _Lbound(const CCharString& key);          // 0x009EEC10
};

class CConsole { public:
    virtual void v00();
    unsigned char _pad04[0x1c - 4];
    CConsoleCommandLine GuessLine;                          // +0x1c
    unsigned char _pad2c[0x30 - 0x2c];
    CCharString CurrentLine;                                // +0x30
    unsigned char _pad34[0x40 - 0x34];
    CCommandMap Commands;                                   // +0x40
    unsigned char _pad48[0x65 - 0x48];
    bool GuessMode;                                         // +0x65
    SCommandNode* GuessIterator;                            // +0x68
    void SetGuessMode();
    __forceinline void SetGuessLine(const CCharString& text)
    {
        CConsoleCommandLine guess(text);
        GuessLine = guess;
    }
};

static CCharString GetLowerCaseCopy(CCharString text) { return text.GetAsLowerCase(); }

void CConsole::SetGuessMode()
{
    GuessMode = true;
    CCharString lowered(GetLowerCaseCopy(CurrentLine));
    GuessIterator = Commands._Lbound(lowered);
    if (GuessIterator != Commands._Myhead) {
        if (strncmp(GuessIterator->first.c_str(), lowered.c_str(), lowered.Length()) == 0) {
            SetGuessLine(GuessIterator->second->GetCommandText());
        }
    }
}

// ---- test harness ----
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
static int g_live = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static CCharStringData* MakeData(const char* text)
{
    if (!text) return 0;
    CCharStringData* d = (CCharStringData*)malloc(sizeof(CCharStringData));
    unsigned long n = (unsigned long)strlen(text);
    char* t = (char*)malloc(n + 1);
    memcpy(t, text, n + 1);
    d->Text = t; d->Length = n; ++g_live;
    return d;
}
static void FreeData(CCharStringData* d)
{
    if (d) { free((void*)d->Text); free(d); --g_live; }
}
CCharString::CCharString(const CCharString& other) : PStringData(MakeData(other.PStringData ? other.PStringData->Text : 0)) { }
CCharString::~CCharString() { FreeData(PStringData); PStringData = 0; }
CCharString& CCharString::operator=(const CCharString& other)
{
    if (this != &other) { CCharStringData* d = MakeData(other.PStringData ? other.PStringData->Text : 0); FreeData(PStringData); PStringData = d; }
    return *this;
}
static CCharString MakeString(const char* text)
{
    CCharString s(*(const CCharString*)"\0\0\0\0");   // copy of a null string
    s.PStringData = MakeData(text);
    return s;
}
CCharString CCharString::GetAsLowerCase() const
{
    CCharString r(*this);
    if (r.PStringData) { char* t = (char*)r.PStringData->Text; for (; *t; ++t) if (*t >= 'A' && *t <= 'Z') *t = (char)(*t + 32); }
    return r;
}
void CBase::v00() { }
CBase::~CBase() { }
static int g_lineCtors = 0;
CConsoleCommandLine::CConsoleCommandLine(const CCharString& text) : Text(text), Flag(true), Cursor(7) { ++g_lineCtors; }
void CConsoleCommand::v00() { }
void CConsoleCommand::v01() { }
CCharString CConsoleCommand::GetCommandText() { return MakeString("base"); }
struct CTestCommand : CConsoleCommand {
    const char* Name;
    virtual CCharString GetCommandText() { return MakeString(Name); }
};

static unsigned char g_nodeBuf[sizeof(SCommandNode) * 4];
#define g_head (((SCommandNode*)g_nodeBuf)[3])
#define g_nodes ((SCommandNode*)g_nodeBuf)
SCommandNode* CCommandMap::_Lbound(const CCharString& key)
{
    const char* k = key.c_str();
    for (int i = 0; i < 3; ++i)
        if (strcmp(g_nodes[i].first.c_str(), k) >= 0) return &g_nodes[i];
    return _Myhead;
}

int main()
{
    static CTestCommand cmds[3];
    const char* keys[3] = { "alpha", "beta", "betamax" };
    const char* texts[3] = { "alpha_cmd", "beta_cmd", "betamax_cmd" };
    for (int i = 0; i < 3; ++i) {
        cmds[i].Name = texts[i];
        g_nodes[i].first.PStringData = MakeData(keys[i]);
        g_nodes[i].second = &cmds[i];
    }
    static unsigned char buf[sizeof(CConsole)];
    memset(buf, 0, sizeof(buf));
    CConsole* con = (CConsole*)buf;
    con->Commands._Myhead = &g_head;
    int baseLive = g_live;

    con->CurrentLine.PStringData = MakeData("BeT");
    con->SetGuessMode();
    CHECK(con->GuessMode == true);
    CHECK(con->GuessIterator == &g_nodes[1]);
    CHECK(con->GuessLine.Text.PStringData && strcmp(con->GuessLine.Text.c_str(), "beta_cmd") == 0);
    CHECK(con->GuessLine.Flag == true && con->GuessLine.Cursor == 7);
    CHECK(g_live == baseLive + 2);                       // CurrentLine + GuessLine text only

    FreeData(con->CurrentLine.PStringData); con->CurrentLine.PStringData = MakeData("zz");
    con->GuessMode = false; con->GuessLine.Cursor = 0;
    con->SetGuessMode();
    CHECK(con->GuessMode == true && con->GuessIterator == &g_head);
    CHECK(strcmp(con->GuessLine.Text.c_str(), "beta_cmd") == 0 && con->GuessLine.Cursor == 0);

    FreeData(con->CurrentLine.PStringData); con->CurrentLine.PStringData = MakeData("ALQ");
    con->SetGuessMode();
    CHECK(con->GuessIterator == &g_nodes[1]);
    CHECK(strcmp(con->GuessLine.Text.c_str(), "beta_cmd") == 0 && con->GuessLine.Cursor == 0);

    FreeData(con->CurrentLine.PStringData); con->CurrentLine.PStringData = 0;
    con->SetGuessMode();
    CHECK(con->GuessIterator == &g_nodes[0]);
    CHECK(strcmp(con->GuessLine.Text.c_str(), "alpha_cmd") == 0 && con->GuessLine.Cursor == 7);
    CHECK(g_live == baseLive + 1);                       // GuessLine text only
    CHECK(g_lineCtors == 2);
    if (fails == 0) printf("009eaff0_TEST PASS\n");
    return fails;
}