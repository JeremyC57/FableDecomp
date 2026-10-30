// Behaviour test for 0x00413c50: the candidate source below is embedded verbatim, followed by stand-ins for its callees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; printf("check failed line %d\n", __LINE__); } } while (0)
// ---- embedded candidate source ----
// InitialiseConsoleVariables() -- register the start-up console commands and variables, then run the user settings files
// Leaf: ?InitialiseConsoleVariables@@YIXXZ
// Retail: Fable.exe @ 0x00413C50, 4158 bytes, `ret`; called once from 0x004025A1. Landed name kept (fable_gfmain.h:
// `void FABLE_FASTCALL InitialiseConsoleVariables()`). Body: CConsole::Initialise('a', key 0x1E, no font bank), then 25
// CConsole::AddInput(new ...) registrations, each through the inlined CTBaseSingleton<CConsole>::Get() (create the
// console on first use): three CConsoleClasslessCommand (0x18 bytes; GConsoleSetLevel / GConsoleSetStartingHolySite take a
// string parameter, GConsoleEnableCodeSectionLoading a bool) and 22 CConsoleVariable (0x10 bytes: bool / long / float
// references). The console names are the retail strings (0x0122E6B4..0x0122E8B4). The CCharString name temporaries are
// conditional (built only when operator new succeeded): VC7.1 tracks them in a flag word (ebx bits 1..0x1000000).
// Finally default_userst.ini is run (CConsole::RunScript) when it exists (CAFile::PathExists) and userst.ini when the 0x01375444 flag is set;
// those last two Get() calls are out of line (VC7.1's inline budget for this function is spent).
// Names (Ego_r.pdb / FableWin.pdb): GConsoleSetLevel, GConsoleSetStartingHolySite, GConsoleEnableCodeSectionLoading (both
// PDBs list them as plain names next to the other GConsole* handlers); CConsoleInputBase / CConsoleCommandBase /
// CConsoleClasslessCommand / CConsoleVariable (vtables 0x0122E5B0 / 0x0122E638 / 0x0122E65C / 0x0122E5C8, ctors
// ??0CConsoleClasslessCommand@@QAE@ABVCCharString@@P6IXABV?$vector@VCGenericVar@@...@Z...ABW4EGenericVarType@@@Z and
// ??0CConsoleVariable@@QAE@ABVCCharString@@AA_N@Z / AAJ / AAM in Ego_r), CConsoleCommandParameters(EGenericVarType)
// 0x009EBB30, CConsole::AddInput 0x009EC5E0, CConsole::Initialise(char, EInputKey, const CFontBank*) 0x009ED190,
// CConsole::RunScript(const CWideString&) 0x009EC890, CConsole::CConsole 0x009ECD80,
// CCountedPointer<CConsole>::Reset 0x00413520, CTBaseSingleton<CConsole>::Get 0x00414C90 / Instance 0x013CAA40,
// CAFile::PathExists 0x00999230, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0,
// CWideString(const unsigned short*) 0x0099B6B0 / dtor 0x0099B510, operator new 0x00BFEA1A.
// EGenericVarType values (CGenericVarRef readers 0x00A63CC0 / 0x00A63D20): 1 long, 3 float, 4 bool, 5 string.
// Globals: the variable names are descriptive from their console names except the bound g_WADBuildEnabled 0x01375459,
// g_bLoadFromAnyDirectory 0x013B8646 and g_bRuntimeLevelFiles 0x013B8616; the ten pool sizes are the members of the
// default CEngineMemoryInit 0x0137549C (Ego_r.pdb CEngineMemoryInit member names, 0x28 bytes).
// Layout: CConsoleInputBase = vptr + Name (Ego_r 8 bytes); CConsoleCommandBase + Parameters +0x08 (retail 0x14: the
// shared header); CConsoleClasslessCommand + CommandFunc +0x14; CConsoleVariable + CGenericVarRef Var +0x08.
class CCharString
{
public:
    CCharString(const char* text, long length = -1);                                // 0x0099EBF0
    CCharString(const CCharString& other);                                          // 0x0099EC30
    ~CCharString();                                                                 // 0x0099EAE0
    void* PStringData;
};
class CWideString
{
public:
    CWideString(const unsigned short* text);                                        // 0x0099B6B0
    ~CWideString();                                                                 // 0x0099B510
    void* PStringData;
};
namespace std
{
    template <class T> class allocator;
    template <class T, class A = allocator<T> > class vector;
}
class CGenericVar;
class CFontBank;
class CCPPointerInfo;
enum EGenericVarType { };
enum EInputKey { };

class CGenericVarRef
{
public:
    EGenericVarType Type;                                                           // +0x00
    void*           PVar;                                                           // +0x04 (union of the typed pointers)
    CGenericVarRef(bool& var) : Type((EGenericVarType)4), PVar(&var) { }
    CGenericVarRef(long& var) : Type((EGenericVarType)1), PVar(&var) { }
    CGenericVarRef(float& var) : Type((EGenericVarType)3), PVar(&var) { }
};

class CConsoleInputBase
{
public:
    CConsoleInputBase(const CCharString& name) : Name(name) { }
    virtual ~CConsoleInputBase();
    CCharString Name;                                                               // +0x04
};

class CConsoleCommandParameters
{
public:
    CConsoleCommandParameters(EGenericVarType type);                                // 0x009EBB30
    unsigned char ParamTypes[0xc];                                                  // vector<EGenericVarType>
};

class CConsoleCommandBase : public CConsoleInputBase
{
public:
    CConsoleCommandBase(const CCharString& name, EGenericVarType type) : CConsoleInputBase(name), Parameters(type) { }
    virtual void CallCommandFunction(const std::vector<CGenericVar>& args) const = 0;
    CConsoleCommandParameters Parameters;                                           // +0x08
};

class CConsoleClasslessCommand : public CConsoleCommandBase
{
public:
    typedef void (__fastcall* TCommandFunc)(const std::vector<CGenericVar>& args);
    CConsoleClasslessCommand(const CCharString& name, TCommandFunc func, const EGenericVarType& type)
        : CConsoleCommandBase(name, type), CommandFunc(func) { }
    virtual void CallCommandFunction(const std::vector<CGenericVar>& args) const;
    TCommandFunc CommandFunc;                                                       // +0x14
};

class CConsoleVariable : public CConsoleInputBase
{
public:
    CConsoleVariable(const CCharString& name, bool& var) : CConsoleInputBase(name), Var(var) { }
    CConsoleVariable(const CCharString& name, long& var) : CConsoleInputBase(name), Var(var) { }
    CConsoleVariable(const CCharString& name, float& var) : CConsoleInputBase(name), Var(var) { }
    virtual CCharString GetTypeAsString() const;
    CGenericVarRef Var;                                                             // +0x08
};

class CConsole
{
public:
    CConsole();                                                                     // 0x009ECD80
    void Initialise(char toggleKey, EInputKey key, const CFontBank* font);          // 0x009ED190
    void AddInput(CConsoleInputBase* input);                                        // 0x009EC5E0
    void RunScript(const CWideString& filename);                            // 0x009EC890
    unsigned char _body[0x9c];
};

template <class T> class CCountedPointer
{
public:
    T*              Data;
    CCPPointerInfo* Info;
    bool IsNull() const { return Data == 0; }
    void Reset(T* data);                                                            // 0x00413520 (CConsole)
};

template <class T> class CTBaseSingleton
{
public:
    static T& __fastcall Get()                                                      // 0x00414C90 (CConsole)
    {
        if (Instance.IsNull())
            Instance.Reset(new T);
        return *Instance.Data;
    }
    static CCountedPointer<T> Instance;                                             // 0x013CAA40 (CConsole)
};

class CAFile
{
public:
    static bool __fastcall PathExists(const CWideString& path);                     // 0x00999230
};

class CEngineMemoryInit
{
public:
    long  LandscapePoolSize;                                                        // +0x00
    float LandscapePhysicalMemoryRatio;                                             // +0x04
    long  LandscapeVirtualMemoryMinSize;                                            // +0x08
    long  MeshPoolSize;                                                             // +0x0c
    long  MeshStatsPoolSize;                                                        // +0x10
    float MeshPhysicalMemoryRatio;                                                  // +0x14
    long  HiresTextureMemory;                                                       // +0x18
    long  PhysicsMeshPoolSize;                                                      // +0x1c
    long  AnimationPoolSize;                                                        // +0x20
    long  ClothPoolSize;                                                            // +0x24
};
extern CEngineMemoryInit g_DefaultEngineMemoryInit;                                 // 0x0137549C
void __fastcall GConsoleSetLevel(const std::vector<CGenericVar>& args);             // 0x00413800
void __fastcall GConsoleSetStartingHolySite(const std::vector<CGenericVar>& args);  // 0x00413840
void __fastcall GConsoleEnableCodeSectionLoading(const std::vector<CGenericVar>& args);// 0x00413880
extern bool g_bUsePhysicalDVD;                                                      // 0x013B85F2
extern bool g_bLoadFromAnyDirectory;                                                // 0x013B8646 (bound name)
extern bool g_bRunFromDVD;                                                          // 0x013B8615
extern bool g_bRuntimeLevelFiles;                                                   // 0x013B8616 (bound name)
extern bool g_bUseCompiledAnimationEvents;                                          // 0x013B860A
extern bool g_bUseCompiledGlobalThings;                                             // 0x013B8609
extern bool g_bUseCompiledDefs;                                                     // 0x013B8617
extern bool g_bUseCompiledWorldFiles;                                               // 0x013B8618
extern bool g_bUseCompiledSoundSymbols;                                             // 0x013B8619
extern bool g_WADBuildEnabled;                                                      // 0x01375459 (bound name)
extern long g_InstallerBufferSize;                                                  // 0x01375494
extern long g_NoInstallBuffers;                                                     // 0x01375498
extern bool g_bExecuteUserSettingsFile;                                             // 0x01375444 (descriptive)

void __fastcall InitialiseConsoleVariables()
{
    CTBaseSingleton<CConsole>::Get().Initialise('a', (EInputKey)0x1e, 0);
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleClasslessCommand("SetLevel", GConsoleSetLevel, (EGenericVarType)5));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleClasslessCommand("SetStartingHolySite", GConsoleSetStartingHolySite, (EGenericVarType)5));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleClasslessCommand("EnableCodeSectionLoading", GConsoleEnableCodeSectionLoading, (EGenericVarType)4));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UsePhysicalDVD", g_bUsePhysicalDVD));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseRetailSaveGameSystem", g_bLoadFromAnyDirectory));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("RunFromDVD", g_bRunFromDVD));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseRetailBanks", g_bRuntimeLevelFiles));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseCompiledAnimationEvents", g_bUseCompiledAnimationEvents));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseCompiledGlobalThings", g_bUseCompiledGlobalThings));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseCompiledDefs", g_bUseCompiledDefs));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseCompiledWorldFiles", g_bUseCompiledWorldFiles));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("UseCompiledSoundSymbols", g_bUseCompiledSoundSymbols));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("AllowDataGeneration", g_WADBuildEnabled));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("InstallerBufferSize", g_InstallerBufferSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("NoInstallBuffers", g_NoInstallBuffers));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("LandscapePoolSize", g_DefaultEngineMemoryInit.LandscapePoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("LandscapePhysicalMemoryRatio", g_DefaultEngineMemoryInit.LandscapePhysicalMemoryRatio));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("LandscapeVirtualMemoryMinSize", g_DefaultEngineMemoryInit.LandscapeVirtualMemoryMinSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("MeshPoolSize", g_DefaultEngineMemoryInit.MeshPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("MeshStatsPoolSize", g_DefaultEngineMemoryInit.MeshStatsPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("MeshPhysicalMemoryRatio", g_DefaultEngineMemoryInit.MeshPhysicalMemoryRatio));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("HiresTextureMemory", g_DefaultEngineMemoryInit.HiresTextureMemory));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("PhysicsMeshPoolSize", g_DefaultEngineMemoryInit.PhysicsMeshPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("AnimationPoolSize", g_DefaultEngineMemoryInit.AnimationPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(new CConsoleVariable("ClothPoolSize", g_DefaultEngineMemoryInit.ClothPoolSize));
    if (CAFile::PathExists(L"default_userst.ini"))
        CTBaseSingleton<CConsole>::Get().RunScript(L"default_userst.ini");
    if (g_bExecuteUserSettingsFile)
        CTBaseSingleton<CConsole>::Get().RunScript(L"userst.ini");
}

// ---- end of embedded source ----
// ---- stand-ins ----
static int g_liveCharStrings = 0, g_liveWideStrings = 0, g_consoles = 0, g_resets = 0, g_initialised = 0;
static int g_inputCount = 0, g_executed = 0;
static CConsoleInputBase* g_inputs[64];
static char g_executedNames[4][32];
static char g_initKey = 0;
static long g_initInputKey = 0;
CCharString::CCharString(const char* text, long length) { CHECK(length == -1); PStringData = (void*)text; ++g_liveCharStrings; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_liveCharStrings; }
CCharString::~CCharString() { --g_liveCharStrings; }
CWideString::CWideString(const unsigned short* text) { PStringData = (void*)text; ++g_liveWideStrings; }
CWideString::~CWideString() { --g_liveWideStrings; }
CConsoleInputBase::~CConsoleInputBase() { }
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType type) { memset(ParamTypes, 0, sizeof(ParamTypes)); *(long*)ParamTypes = (long)type; }
void CConsoleClasslessCommand::CallCommandFunction(const std::vector<CGenericVar>& args) const { CommandFunc(args); }
CCharString CConsoleVariable::GetTypeAsString() const { return CCharString("?"); }
CConsole::CConsole() { ++g_consoles; }
void CConsole::Initialise(char toggleKey, EInputKey key, const CFontBank* font) { ++g_initialised; g_initKey = toggleKey; g_initInputKey = (long)key; CHECK(font == 0); }
void CConsole::AddInput(CConsoleInputBase* input) { g_inputs[g_inputCount++] = input; }
static void Narrow(const CWideString& name, char* out)
{
    const unsigned short* w = (const unsigned short*)name.PStringData;
    int i = 0;
    for (; w[i] && i < 31; ++i) out[i] = (char)w[i];
    out[i] = 0;
}
void CConsole::RunScript(const CWideString& filename) { Narrow(filename, g_executedNames[g_executed++]); }
template <> void CCountedPointer<CConsole>::Reset(CConsole* data) { ++g_resets; Data = data; }
template <> CCountedPointer<CConsole> CTBaseSingleton<CConsole>::Instance = { 0, 0 };
static int g_pathChecks = 0;
bool __fastcall CAFile::PathExists(const CWideString& path)
{
    char name[32]; Narrow(path, name); ++g_pathChecks;
    return strcmp(name, "default_userst.ini") == 0;
}
void __fastcall GConsoleSetLevel(const std::vector<CGenericVar>&) { }
void __fastcall GConsoleSetStartingHolySite(const std::vector<CGenericVar>&) { }
void __fastcall GConsoleEnableCodeSectionLoading(const std::vector<CGenericVar>&) { }
bool g_bUsePhysicalDVD, g_bLoadFromAnyDirectory, g_bRunFromDVD, g_bRuntimeLevelFiles, g_bUseCompiledAnimationEvents;
bool g_bUseCompiledGlobalThings, g_bUseCompiledDefs, g_bUseCompiledWorldFiles, g_bUseCompiledSoundSymbols, g_WADBuildEnabled;
long g_InstallerBufferSize, g_NoInstallBuffers;
CEngineMemoryInit g_DefaultEngineMemoryInit;
bool g_bExecuteUserSettingsFile = true;
static const char* NameOf(CConsoleInputBase* input) { return (const char*)input->Name.PStringData; }
int main()
{
    InitialiseConsoleVariables();
    CHECK(g_consoles == 1 && g_resets == 1);                      // the console is created once, on first use
    CHECK(g_initialised == 1 && g_initKey == 'a' && g_initInputKey == 0x1e);
    CHECK(g_inputCount == 25);
    CHECK(g_liveCharStrings == 25 && g_liveWideStrings == 0);      // only the inputs' own name copies are left
    CHECK(strcmp(NameOf(g_inputs[0]), "SetLevel") == 0);
    CConsoleClasslessCommand* setLevel = (CConsoleClasslessCommand*)g_inputs[0];
    CHECK(setLevel->CommandFunc == GConsoleSetLevel && *(long*)setLevel->Parameters.ParamTypes == 5);
    CConsoleClasslessCommand* codeSections = (CConsoleClasslessCommand*)g_inputs[2];
    CHECK(strcmp(NameOf(g_inputs[2]), "EnableCodeSectionLoading") == 0);
    CHECK(codeSections->CommandFunc == GConsoleEnableCodeSectionLoading && *(long*)codeSections->Parameters.ParamTypes == 4);
    CConsoleVariable* dvd = (CConsoleVariable*)g_inputs[3];
    CHECK(strcmp(NameOf(dvd), "UsePhysicalDVD") == 0 && dvd->Var.Type == 4 && dvd->Var.PVar == &g_bUsePhysicalDVD);
    CConsoleVariable* wad = (CConsoleVariable*)g_inputs[12];
    CHECK(strcmp(NameOf(wad), "AllowDataGeneration") == 0 && wad->Var.PVar == &g_WADBuildEnabled);
    CConsoleVariable* buffers = (CConsoleVariable*)g_inputs[13];
    CHECK(strcmp(NameOf(buffers), "InstallerBufferSize") == 0 && buffers->Var.Type == 1 && buffers->Var.PVar == &g_InstallerBufferSize);
    CConsoleVariable* ratio = (CConsoleVariable*)g_inputs[16];
    CHECK(strcmp(NameOf(ratio), "LandscapePhysicalMemoryRatio") == 0 && ratio->Var.Type == 3);
    CHECK(ratio->Var.PVar == &g_DefaultEngineMemoryInit.LandscapePhysicalMemoryRatio);
    CConsoleVariable* cloth = (CConsoleVariable*)g_inputs[24];
    CHECK(strcmp(NameOf(cloth), "ClothPoolSize") == 0 && cloth->Var.Type == 1 && cloth->Var.PVar == &g_DefaultEngineMemoryInit.ClothPoolSize);
    CHECK(g_pathChecks == 1 && g_executed == 2);
    CHECK(strcmp(g_executedNames[0], "default_userst.ini") == 0 && strcmp(g_executedNames[1], "userst.ini") == 0);
    if (g_ok) printf("00413c50_TEST PASS\n");
    return g_ok ? 0 : 1;
}