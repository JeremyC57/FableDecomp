// NGlobalConsole::Initialise() -- register the global console commands; the retail build keeps only "ActivateQuest"
// Leaf: ?Initialise@NGlobalConsole@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00419D90, length 218 bytes, plain `ret` (`__fastcall`, no arguments).
// Name: Ego_r.pdb / FableWin.pdb `?Initialise@NGlobalConsole@@YIXXZ` -- the last function of the NGlobalConsole TU
// (donor 0x0041FB80, right after the console command bodies); in retail the TU holds only ConsoleActivateQuest
// (0x00419CE0, donor 0x00418A70) and this registration, which CWorld::Init (0x004A6E30) calls at 0x004A712B.
// The catalog's "StatRegisterClass" is a propagated label.
// Body: CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleClasslessCommand("ActivateQuest",
// ConsoleActivateQuest, CConsoleCommandParameters(type 5))) -- the name and the one-element parameter list are
// temporaries of the new-expression, so they are built only after the allocation succeeded and destroyed at the
// end of the full expression through the conditional-destruction flags in ebx (bit 0 the string, bit 1 the list).
// Shape: the name is an IMPLICIT const char* -> CCharString conversion (default length -1): VC7.1 builds implicit
// conversion temporaries before the explicit argument temporaries, which gives retail's string-first order; an
// explicit `CCharString("ActivateQuest", -1)` argument is evaluated right to left after the parameter list.
// Callees: operator new @ 0x00BFEA1A, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
//          CConsoleCommandParameters::CConsoleCommandParameters(long) @ 0x009EBB30 (zero the vector, push_back),
//          CCharString::CCharString(const CCharString&) @ 0x0099EC30, CConsoleCommandParameters copy ctor @ 0x00419F50,
//          CConsole::CConsole @ 0x009ECD80, CCountedPointer<CConsole>::Reset @ 0x00413520, CConsole::AddInput @ 0x009EC5E0,
//          free @ 0x00BFEA14 (the list's storage), CCharString::~CCharString @ 0x0099EAE0.
// Vtables: CConsoleInputBase 0x0122E5B0, CConsoleCommandBase 0x0122E638, CConsoleClasslessCommand 0x0122E65C
// (rtti-vtables.tsv); the inlined constructor chain stores them in that order, the command function pointer last.
// Layout (retail 0x18 bytes): vptr, CCharString Name +4, CConsoleCommandParameters ParamTypes +8 (a 12-byte STLport
// vector<EGenericVarType>), the `void (__fastcall*)(const vector<CGenericVar>&)` Function +0x14. CConsole singleton:
// CCountedPointer at 0x013CAA40 (as the landed CTBaseSingleton<CConsole>::Get 0x00414C90, inlined here).
// Model: local -- engine/CConsoleCommandBase.h exists, but the generated header is data-only (pack(1), no member functions, no
// virtuals: the vptr and counted pointers are flattened into raw words); the inlined constructor chain needs the real
// polymorphic hierarchy (three vtable stores), so it is local.
#include <new>
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14
class CCharString {
public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
private:
    void* PStringData;
};
enum EGenericVarType { GENERIC_VAR_TYPE_STRING = 5 };
class CGenericVar;
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CConsoleCommandParameters {                        // retail 12 bytes: STLport vector<EGenericVarType>
public:
    CConsoleCommandParameters(long type);                // 0x009EBB30
    CConsoleCommandParameters(const CConsoleCommandParameters& other);   // 0x00419F50
    ~CConsoleCommandParameters() { if (_M_start) free(_M_start); }
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
class CConsoleInputBase {                                // vtable 0x0122E5B0
public:
    CConsoleInputBase(const CCharString& name) : Name(name) {}
    virtual ~CConsoleInputBase();
    CCharString Name;                                    // +0x04
};
class CConsoleCommandBase : public CConsoleInputBase {   // vtable 0x0122E638
public:
    CConsoleCommandBase(const CCharString& name, const CConsoleCommandParameters& params)
        : CConsoleInputBase(name), ParamTypes(params) {}
    virtual ~CConsoleCommandBase();
    CConsoleCommandParameters ParamTypes;                // +0x08
};
typedef void (__fastcall *TConsoleFunction)(const std::vector<CGenericVar>& args);
class CConsoleClasslessCommand : public CConsoleCommandBase {   // vtable 0x0122E65C
public:
    CConsoleClasslessCommand(const CCharString& name, TConsoleFunction function, const CConsoleCommandParameters& params)
        : CConsoleCommandBase(name, params), Function(function) {}
    virtual ~CConsoleClasslessCommand();
    TConsoleFunction Function;                           // +0x14
};
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                 // 0x00413520 for CConsole
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
    static CCountedPointer<T> Instance;                  // 0x013CAA40 for CConsole
    static T& __fastcall Get()
    {
        if (!Instance.Data)
            Instance.Reset(new T);
        return *Instance.Data;
    }
};
class CConsole : public CTBaseSingleton<CConsole> {
public:
    CConsole();                                          // 0x009ECD80
    void AddInput(CConsoleInputBase* input);             // 0x009EC5E0
    unsigned char _body[0x9c - 4];
};

namespace NGlobalConsole {
void __fastcall ConsoleActivateQuest(const std::vector<CGenericVar>& args);   // 0x00419CE0

void __fastcall Initialise()
{
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleClasslessCommand("ActivateQuest",
        ConsoleActivateQuest, CConsoleCommandParameters(GENERIC_VAR_TYPE_STRING)));
}
}