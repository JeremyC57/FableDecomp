// Behaviour test for 0x009ECB70 CConsole::ConsoleRunScript: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#endif
#define protected public
extern char g_emptyString[];                            // 0x0129AAF4
struct CStringData { char* Text; long Length; };
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
class CCharString {
public:
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString ToLower() const;                        // 0x0099ED10
    const CCharString& operator+=(const char* text);          // 0x0099F100
    CWideString ToWideString() const;                   // 0x0099E480
    long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* c_str() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(c_str() + offset, suffix.c_str()) == 0;
        return false;
    }
};

class CGenericVar {                                     // Ego_r.pdb, 0x1C bytes, polymorphic
public:
    virtual ~CGenericVar();
    CCharString GetString() const { return ValString; }
    long          Type;                                 // +0x04 EGenericVarType
    CCharString   ValString;                            // +0x08
    long          ValSLONG;                             // +0x0C
    unsigned long ValULONG;                             // +0x10
    bool          ValBool;                              // +0x14
    float         ValFloat;                             // +0x18
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        typedef const _Tp& const_reference;
        typedef unsigned int size_type;
        const _Tp* begin() const { return this->_M_start; }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
}

class CConsole {
public:
    virtual ~CConsole();
    static void __fastcall ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args);
    void ExecuteConfigFile(const CWideString& fileName); // 0x009EC890
};

void __fastcall CConsole::ConsoleRunScript(CConsole& console, const std::vector<CGenericVar>& args)
{
    CCharString fileName = args[0].GetString();
    bool hasExtension;
    {
        CCharString extension(".ini");
        hasExtension = fileName.ToLower().EndsWith(extension);
    }
    if (!hasExtension)
        fileName += ".ini";
    console.ExecuteConfigFile(fileName.ToWideString());
}

char g_emptyString[1] = "";
static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static CStringData* Make(const char* text)
{
    CStringData* d = (CStringData*)malloc(sizeof(CStringData)); d->Length = (long)strlen(text);
    d->Text = (char*)malloc(d->Length + 1); strcpy(d->Text, text); return d;
}
CCharString::CCharString(const char* text, long) : PStringData(Make(text)) { }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData ? Make(other.PStringData->Text) : 0) { }
CCharString::~CCharString() { }
CCharString CCharString::ToLower() const
{
    CCharString r(c_str());
    for (char* p = r.PStringData->Text; *p; ++p) if (*p >= 'A' && *p <= 'Z') *p = (char)(*p - 'A' + 'a');
    return r;
}
const CCharString& CCharString::operator+=(const char* text)
{
    char buf[128]; strcpy(buf, c_str()); strcat(buf, text); PStringData = Make(buf); return *this;
}
CWideString CCharString::ToWideString() const { CWideString w; w.PStringData = (void*)c_str(); return w; }
CWideString::~CWideString() { }
CGenericVar::~CGenericVar() { }
CConsole::~CConsole() { }
void CConsole::ExecuteConfigFile(const CWideString& fileName) { char b[96]; sprintf(b, "exec(%s)", (const char*)fileName.PStringData); Log(b); }
static int Run(const char* arg, const char* expect)
{
    CGenericVar* v = (CGenericVar*)calloc(1, sizeof(CGenericVar)); v->ValString.PStringData = arg ? Make(arg) : 0;
    tstd::vector<CGenericVar>* args = (tstd::vector<CGenericVar>*)calloc(1, sizeof(tstd::vector<CGenericVar>));
    args->_M_start = v; args->_M_finish = v + 1;
    CConsole* c = (CConsole*)calloc(1, sizeof(CConsole));
    g_log[0] = 0;
    CConsole::ConsoleRunScript(*c, *args);
    int ok = strcmp(g_log, expect) == 0;
    if (!ok) printf("got %s want %s\n", g_log, expect);
    return ok;
}
int main()
{
    int ok = Run("autoexec", "exec(autoexec.ini);");
    ok &= Run("game.ini", "exec(game.ini);");
    ok &= Run("GAME.INI", "exec(GAME.INI);");
    ok &= Run("ini", "exec(ini.ini);");
    ok &= Run("a.ini.txt", "exec(a.ini.txt.ini);");
    ok &= Run(0, "exec(.ini);");
    if (ok) printf("009ecb70_TEST PASS\n");
    return ok ? 0 : 1;
}