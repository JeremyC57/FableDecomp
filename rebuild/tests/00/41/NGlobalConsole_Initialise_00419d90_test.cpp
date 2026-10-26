#include <cstdio>
#include <cstring>
#include <cstdlib>
// Behaviour test for NGlobalConsole::Initialise (retail 0x00419D90): the console singleton is created on first use
// and receives one CConsoleClasslessCommand named "ActivateQuest" with a single parameter of type 5 bound to
// ConsoleActivateQuest; the argument temporaries (name string, parameter list) are destroyed afterwards.
static int g_strings = 0, g_lists = 0;
class CCharString { public:
    char m_text[32];
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CConsoleCommandParameters { public:
    long* _M_start; long* _M_finish;
    CConsoleCommandParameters(long type) { _M_start = (long*)std::malloc(sizeof(long)); *_M_start = type; _M_finish = _M_start + 1; ++g_lists; }
    CConsoleCommandParameters(const CConsoleCommandParameters& o)
    { long n = (long)(o._M_finish - o._M_start); _M_start = (long*)std::malloc(n * sizeof(long)); for (long i = 0; i < n; ++i) _M_start[i] = o._M_start[i]; _M_finish = _M_start + n; ++g_lists; }
    ~CConsoleCommandParameters() { if (_M_start) std::free(_M_start); --g_lists; }
};
class CConsoleInputBase { public: CCharString Name; CConsoleInputBase(const CCharString& n) : Name(n) {} virtual ~CConsoleInputBase() {} };
struct CConsoleCommandBase : CConsoleInputBase {
    CConsoleCommandParameters ParamTypes;
    CConsoleCommandBase(const CCharString& n, const CConsoleCommandParameters& p) : CConsoleInputBase(n), ParamTypes(p) {}
};
typedef void (*TConsoleFunction)(const void* args);
struct CConsoleClasslessCommand : CConsoleCommandBase {
    TConsoleFunction Function;
    CConsoleClasslessCommand(const CCharString& n, TConsoleFunction f, const CConsoleCommandParameters& p) : CConsoleCommandBase(n, p), Function(f) {}
};
static int g_consolesBuilt = 0;
class CConsole { public:
    CConsoleInputBase* m_inputs[4]; int m_count;
    CConsole() : m_count(0) { ++g_consolesBuilt; }
    void AddInput(CConsoleInputBase* input) { m_inputs[m_count++] = input; }
};
static CConsole* g_instance = 0;
static CConsole& GetConsole() { if (!g_instance) g_instance = new CConsole; return *g_instance; }
static void ConsoleActivateQuest(const void*) {}
static void Initialise()
{
    GetConsole().AddInput(new CConsoleClasslessCommand("ActivateQuest", ConsoleActivateQuest, CConsoleCommandParameters(5)));
}
int main()
{
    Initialise();
    int ok = g_instance != 0 && g_consolesBuilt == 1 && g_instance->m_count == 1 && g_strings == 1 && g_lists == 1;
    if (ok) {
        CConsoleClasslessCommand* c = (CConsoleClasslessCommand*)g_instance->m_inputs[0];
        ok = std::strcmp(c->Name.m_text, "ActivateQuest") == 0 && c->ParamTypes._M_finish - c->ParamTypes._M_start == 1
          && c->ParamTypes._M_start[0] == 5 && c->Function == ConsoleActivateQuest;
    }
    Initialise();                       // the singleton is reused
    ok = ok && g_consolesBuilt == 1 && g_instance->m_count == 2 && g_strings == 2 && g_lists == 2;
    if (!ok) { std::printf("00419d90_TEST FAIL\n"); return 1; }
    std::printf("00419d90_TEST PASS\n");
    return 0;
}