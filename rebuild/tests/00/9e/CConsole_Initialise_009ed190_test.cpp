#include <cstdio>
#include <cstring>
#include <cstdlib>
// Behaviour test for CConsole::Initialise (retail 0x009ED190): the toggle key, key code and font are stored, the
// registered-input map is emptied, fourteen commands (with their parameter lists) and the ConsoleAlpha variable are
// registered in retail order, the history is cleared and its position reset, and one greeting is sent; every argument
// temporary (name string, parameter list) is destroyed again.
static int g_strings = 0, g_lists = 0;
class CCharString { public:
    char m_text[48];
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
enum EGenericVarType { GVT_NONE, GVT_SLONG, GVT_ULONG, GVT_FLOAT, GVT_BOOL, GVT_STRING };
class CConsoleCommandParameters { public:
    long m_types[3]; long m_count;
    CConsoleCommandParameters() : m_count(0) { ++g_lists; }
    CConsoleCommandParameters(EGenericVarType a) : m_count(1) { m_types[0] = a; ++g_lists; }
    CConsoleCommandParameters(EGenericVarType a, EGenericVarType b) : m_count(2) { m_types[0] = a; m_types[1] = b; ++g_lists; }
    CConsoleCommandParameters(EGenericVarType a, EGenericVarType b, EGenericVarType c) : m_count(3) { m_types[0] = a; m_types[1] = b; m_types[2] = c; ++g_lists; }
    CConsoleCommandParameters(const CConsoleCommandParameters& o) : m_count(o.m_count) { for (long i = 0; i < m_count; ++i) m_types[i] = o.m_types[i]; ++g_lists; }
    ~CConsoleCommandParameters() { --g_lists; }
};
class CConsoleInputBase { public: CCharString Name; CConsoleInputBase(const CCharString& n) : Name(n) {} virtual ~CConsoleInputBase() {} virtual bool IsVariable() const { return false; } };
class CConsoleCommandBase : public CConsoleInputBase { public:
    CConsoleCommandParameters ParamTypes;
    CConsoleCommandBase(const CCharString& n) : CConsoleInputBase(n) {}
    CConsoleCommandBase(const CCharString& n, const CConsoleCommandParameters& p) : CConsoleInputBase(n), ParamTypes(p) {}
    CConsoleCommandBase(const CCharString& n, const EGenericVarType& t) : CConsoleInputBase(n), ParamTypes(t) {}
};
class CConsole;
typedef void (*TFunction)(CConsole&, const void*);
class CConsoleCommand : public CConsoleCommandBase { public:
    CConsole* Object; TFunction Function;
    CConsoleCommand(const CCharString& n, CConsole& o, TFunction f) : CConsoleCommandBase(n), Object(&o), Function(f) {}
    CConsoleCommand(const CCharString& n, CConsole& o, TFunction f, const EGenericVarType& t) : CConsoleCommandBase(n, t), Object(&o), Function(f) {}
    CConsoleCommand(const CCharString& n, CConsole& o, TFunction f, const CConsoleCommandParameters& p) : CConsoleCommandBase(n, p), Object(&o), Function(f) {}
};
class CConsoleVariable : public CConsoleInputBase { public:
    EGenericVarType Type; long* PValSLONG;
    CConsoleVariable(const CCharString& n, long& v) : CConsoleInputBase(n), Type(GVT_SLONG), PValSLONG(&v) {}
    virtual bool IsVariable() const { return true; }
};
class CConsole { public:
    CConsoleInputBase* m_inputs[32]; int m_count;
    int m_history; int m_historyPos; int m_messages;
    char ToggleChar; long ToggleKey; const void* PFont; long ConsoleAlpha;
    CConsole() : m_count(3), m_history(5), m_historyPos(4), m_messages(0), ConsoleAlpha(255) {}
    void ClearInputs() { m_count = 0; }
    void AddInput(CConsoleInputBase* input) { m_inputs[m_count++] = input; }
    void SendMessageToUser(const CCharString& text) { if (std::strcmp(text.m_text, "Try and shut up and get on with your work.") == 0) ++m_messages; }
    void Initialise(char toggle_char, long toggle_key, const void* font);
};
static void FnBindString(CConsole&, const void*) {} static void FnRunBound(CConsole&, const void*) {}
static void FnBindKey(CConsole&, const void*) {} static void FnRunScript(CConsole&, const void*) {}
static void FnList(CConsole&, const void*) {} static void FnCmdList(CConsole&, const void*) {}
static void FnColour(CConsole&, const void*) {}
void CConsole::Initialise(char toggle_char, long toggle_key, const void* font)
{
    ToggleChar = toggle_char; ToggleKey = toggle_key; PFont = font;
    ClearInputs();
    AddInput(new CConsoleCommand("BindString", *this, FnBindString, CConsoleCommandParameters(GVT_STRING, GVT_STRING)));
    AddInput(new CConsoleCommand("RunBoundString", *this, FnRunBound, CConsoleCommandParameters(GVT_STRING)));
    AddInput(new CConsoleCommand("BindKey", *this, FnBindKey, CConsoleCommandParameters(GVT_STRING, GVT_STRING)));
    AddInput(new CConsoleCommand("RunScript", *this, FnRunScript, GVT_STRING));
    AddInput(new CConsoleCommand("ConsoleListContaining", *this, FnList, GVT_STRING));
    AddInput(new CConsoleCommand("CommandList", *this, FnCmdList));
    AddInput(new CConsoleCommand("CommandListContaining", *this, FnList, GVT_STRING));
    AddInput(new CConsoleCommand("VarList", *this, FnCmdList));
    AddInput(new CConsoleCommand("VarListContaining", *this, FnList, GVT_STRING));
    AddInput(new CConsoleCommand("SetConsoleTextColour", *this, FnColour, CConsoleCommandParameters(GVT_ULONG, GVT_ULONG, GVT_ULONG)));
    AddInput(new CConsoleCommand("SetConsoleHistoryWindowColour", *this, FnColour, CConsoleCommandParameters(GVT_ULONG, GVT_ULONG, GVT_ULONG)));
    AddInput(new CConsoleCommand("SetConsoleClientWindowColour", *this, FnColour, CConsoleCommandParameters(GVT_ULONG, GVT_ULONG, GVT_ULONG)));
    AddInput(new CConsoleCommand("SetConsoleHelpWindowColour", *this, FnColour, CConsoleCommandParameters(GVT_ULONG, GVT_ULONG, GVT_ULONG)));
    AddInput(new CConsoleCommand("SetConsoleScrollBarColour", *this, FnColour, CConsoleCommandParameters(GVT_ULONG, GVT_ULONG, GVT_ULONG)));
    AddInput(new CConsoleVariable("ConsoleAlpha", ConsoleAlpha));
    m_history = 0;
    m_historyPos = 0;
    SendMessageToUser("Try and shut up and get on with your work.");
}
int main()
{
    static const char* names[15] = { "BindString", "RunBoundString", "BindKey", "RunScript", "ConsoleListContaining", "CommandList",
        "CommandListContaining", "VarList", "VarListContaining", "SetConsoleTextColour", "SetConsoleHistoryWindowColour",
        "SetConsoleClientWindowColour", "SetConsoleHelpWindowColour", "SetConsoleScrollBarColour", "ConsoleAlpha" };
    static const long counts[14] = { 2, 1, 2, 1, 1, 0, 1, 0, 1, 3, 3, 3, 3, 3 };
    CConsole console;
    int font = 0;
    console.Initialise('`', 41, &font);
    int ok = console.ToggleChar == '`' && console.ToggleKey == 41 && console.PFont == &font && console.m_count == 15
          && console.m_history == 0 && console.m_historyPos == 0 && console.m_messages == 1;
    for (int i = 0; ok && i < 15; ++i)
        ok = std::strcmp(console.m_inputs[i]->Name.m_text, names[i]) == 0 && console.m_inputs[i]->IsVariable() == (i == 14);
    for (int i = 0; ok && i < 14; ++i) {
        CConsoleCommand* c = (CConsoleCommand*)console.m_inputs[i];
        ok = c->Object == &console && c->ParamTypes.m_count == counts[i];
        for (long k = 0; ok && k < c->ParamTypes.m_count; ++k)
            ok = c->ParamTypes.m_types[k] == (i >= 9 ? GVT_ULONG : GVT_STRING);
    }
    if (ok) {
        CConsoleVariable* v = (CConsoleVariable*)console.m_inputs[14];
        ok = v->Type == GVT_SLONG && v->PValSLONG == &console.ConsoleAlpha && *v->PValSLONG == 255;
    }
    ok = ok && g_strings == 15 && g_lists == 14;      // only the registered copies survive
    console.Initialise('~', 7, 0);                    // the map is emptied before re-registering
    ok = ok && console.m_count == 15 && console.ToggleChar == '~' && console.PFont == 0 && console.m_messages == 2;
    if (!ok) { std::printf("009ed190_TEST FAIL\n"); return 1; }
    std::printf("009ed190_TEST PASS\n");
    return 0;
}