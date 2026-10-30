# sub-wave asmre (hand): the asm re-author pass NAME_FIXES (rebuild/candidates/manual/batches/2026-10-30_asm_reauthor/NAME_FIXES.tsv)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub, recode
# ---- 0x0042F70B ?CompileDefs@CNewFrontendGameComponent@@SIXXZ (Ego_r + FableWin, static __fastcall): caller CGame::Play 0x00412F90
edit('00412f90', 'src', '''    CNewFrontendGameComponent(CGame* game, const CNewFrontendGameComponentInit& init);   // 0x0042EA8F
    unsigned char _body[0x144];
};
void __fastcall CompileDefs();                          // 0x0042F70B
''', '''    CNewFrontendGameComponent(CGame* game, const CNewFrontendGameComponentInit& init);   // 0x0042EA8F
    static void __fastcall CompileDefs();               // 0x0042F70B ?CompileDefs@CNewFrontendGameComponent@@SIXXZ (Ego_r / FableWin)
    unsigned char _body[0x144];
};
''')
edit('00412f90', 'src', '''        CompileDefs();
''', '''        CNewFrontendGameComponent::CompileDefs();
''')
# ---- 0x0043368D ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r 0x00441A6D + FableWin; static __fastcall returning a reference)
recode('0043368d', 'src', 'extern int g_value_013b8760;', '''// The singleton pointer at 0x013B8760 keeps its bound spelling (no PDB data name; CStartupDefinitionManager::Set stores it).
extern int g_value_013b8760;
class CStartupDefinitionManager {
public:
    static CStartupDefinitionManager& __fastcall Get();
};
CStartupDefinitionManager& __fastcall CStartupDefinitionManager::Get()
{
    return *(CStartupDefinitionManager*)g_value_013b8760;
}
''', line1='// CStartupDefinitionManager::Get() -- the startup definition manager singleton: `mov eax,[0x013B8760] ; ret` (static __fastcall, no `this`)',
     leaf='?Get@CStartupDefinitionManager@@SIAAV1@XZ',
     extra_comment='// Identity (naming wave 11): ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r.pdb 0x00441A6D + FableWin.pdb); was the free extern "C" GetGlobal.')
edit('004083e0', 'src', '''CDefinitionManager* __fastcall GetFrontendDefinitionManager(); //0043368D loads013B8760
''', '''class CStartupDefinitionManager : public CDefinitionManager { public: static CStartupDefinitionManager& __fastcall Get(); };   // 0x0043368D ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r / FableWin): loads 0x013B8760
''')
edit_all('004083e0', 'src', 'GetFrontendDefinitionManager()->', 'CStartupDefinitionManager::Get().', 2)
edit('0042e98f', 'src', '''class CDefinitionManager;
CDefinitionManager* GetDefinitionManager();              // 0x0043368D
''', '''class CDefinitionManager { };                           // only passed on here
class CStartupDefinitionManager : public CDefinitionManager { public:
    static CStartupDefinitionManager& __fastcall Get();  // 0x0043368D ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r / FableWin)
};
''')
edit('0042e98f', 'src', '''m_pInputManager, GetDefinitionManager());''', '''m_pInputManager, &CStartupDefinitionManager::Get());''')
# ---- 0x004300FB / 0x00430096: CDefinitionManager::GetDef<T> instantiations (FableWin.pdb, def BY POINTER, const)
for A, T, DEF, CALLER in (('004300fb', 'CFrontEndDef', '"FRONT_END"', 'CNewFrontendGameComponent::InitialiseDefs 0x0042F722'),
                          ('00430096', 'CEngineDef', '"ENGINE"', 'CNewFrontendGameComponent::InitialiseEngine 0x0042E204')):
    SYM = '??$GetDef@V%s@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBV%s@@@@@Z' % (T, T)
    recode(A, 'src', 'class CDefinitionManager { public:', '''class %s;
class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString& name, long index) const;   // 0x009ADE10 ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z (hidden result pointer)
    template <class T> bool GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const;
};

template <class T> bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> found = GetPDefFromClassIndex(name, index);
    if (found.m_p) {
        *(CDefPointer<const CDefClassBase>*)def = found;
        return true;
    }
    return false;
}
template bool CDefinitionManager::GetDef<%s>(const CCharString&, long, CDefPointer<const %s>*) const;
''' % (T, T, T),
        line1='// CDefinitionManager::GetDef<%s>(const CCharString&, long, CDefPointer<const %s>*) const -- GetPDefFromClassIndex @ 0x009ADE10 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer takes it' % (T, T),
        leaf=SYM,
        extra_comment='// Identity (naming wave 11): %s (FableWin.pdb GetDef<%s> instantiation; the %s def of %s, which passes the def BY POINTER).' % (SYM, T, DEF, CALLER))
# ---- 0x00B26340 ?GFAllocEngine@@YIPAVCIEngine@@XZ (Ego_r 0x009B0B40 + FableWin): `new CEngine` (0x178), ctor 0x00B260B0 = Ego_r ??0CEngine@@QAE@XZ
recode('00b26340', 'src', 'class CEngineInternalPrimitiveBase;', '''class CIEngine { };                                     // the engine interface (CEngine's base at offset 0)
class CEngine : public CIEngine {
public:
    CEngine();                                          // 0x00B260B0 ??0CEngine@@QAE@XZ (Ego_r.pdb 0x009B08C0)
    unsigned char _body[0x178];                         // retail new size 0x178
};
CIEngine* __fastcall GFAllocEngine()
{
    return new CEngine;
}
''', line1='// GFAllocEngine() -- the engine factory: new CEngine (0x178 bytes), null when the allocation fails',
     leaf='?GFAllocEngine@@YIPAVCIEngine@@XZ',
     extra_comment='// Identity (naming wave 11): ?GFAllocEngine@@YIPAVCIEngine@@XZ (Ego_r.pdb 0x009B0B40 + FableWin.pdb); the catalog label CEnginePrimitive2DViewportManager::CreateInternalPrimitive is propagated. Callee 0x00B260B0 is CEngine::CEngine (catalog + Ego_r).')
# ---- 0x0042F7C1 ??1CEngineInit@@QAE@XZ (Ego_r.pdb 0x0043CB3F): the out-of-line destructor of CEngineInit's five CWideString paths
recode('0042f7c1', 'src', 'struct CWideStringQuintet_0042f7c1 {', '''class CEngineInit {                                    // Ego_r.pdb CEngineInit (0x68; retail keeps the paths 4 bytes lower)
public:
    unsigned char _pad[0x20];               // +0x00..+0x1f (DefinitionManager, EngineDef, ... the bank pointers)
    CWideString MeshResourcePath;           // +0x20
    CWideString GraphicsResourcePath;       // +0x24
    CWideString ShaderResourcePath;         // +0x28
    CWideString EngineCacheResourcePath;    // +0x2c
    CWideString WriteableEngineCacheResourcePath;   // +0x30
    ~CEngineInit();
};
CEngineInit::~CEngineInit()
{
}
''', line1='// CEngineInit::~CEngineInit() -- implicit destructor of the five CWideString resource paths',
     leaf='??1CEngineInit@@QAE@XZ',
     extra_comment='// Identity (naming wave 11): ??1CEngineInit@@QAE@XZ (Ego_r.pdb 0x0043CB3F; Ego_r CEngineInit holds MeshResourcePath .. WriteableEngineCacheResourcePath as its five CWideString members); was CWideStringQuintet_0042f7c1.')
# ---- 0x0099B940 ??YCWideString@@QAEABV0@PBG@Z: the row already emits it (VC7.1's default wchar_t is the unsigned short typedef): catalog label only
# ---- 0x005BD931 ?GetMaximumNumberOfItems@CItem@NInventory@@QBEJXZ (Ego_r 0x00620834 + FableWin): NInventory::CItem over the shared
#      engine/CItem.h layout; its callee 0x005BD82E is Ego_r's private const ?GetInventoryItemDef@CItem@NInventory@@ABE?AV?$CDefPointer@$$CBVCInventoryItemDef@@@@XZ
edit('005bd931', 'src', '''struct CInventoryItemDef_Retail : CIVCountedPointeeBase {''', '''class CInventoryItemDef : public CIVCountedPointeeBase { public:''')
edit('005bd931', 'src', '''struct CIVCountedPointer_Def {                     // 4 bytes
    CInventoryItemDef_Retail* Object;
    __forceinline ~CIVCountedPointer_Def()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CItem_Methods : CItem {
    CIVCountedPointer_Def GetInventoryItemDef();   // 0x005BD82E (returns its hidden result pointer)
    long GetMaximumNumberOfItems();
};

long CItem_Methods::GetMaximumNumberOfItems()
{''', '''template <class T> class CDefPointer {             // 4 bytes: the counted def pointer
public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object)
            const_cast<CInventoryItemDef*>(Object)->ReleaseTemporary();
    }
};
namespace NInventory {
class CItem : public ::CItem {                     // NInventory::CItem (Ego_r.pdb / FableWin.pdb) over the shared engine/CItem.h layout
public:
    long GetMaximumNumberOfItems() const;
private:
    CDefPointer<const CInventoryItemDef> GetInventoryItemDef() const;   // 0x005BD82E (returns its hidden result pointer)
};
}

long NInventory::CItem::GetMaximumNumberOfItems() const
{''')
edit('005bd931', 'src', '''// CItem::GetMaximumNumberOfItems() -- zero''', '''// NInventory::CItem::GetMaximumNumberOfItems() const -- zero''')
edit('005bd931', 'src', '''// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD931, length 58 bytes.
''', '''// Leaf: ?GetMaximumNumberOfItems@CItem@NInventory@@QBEJXZ
// Identity (naming wave 11): Ego_r.pdb 0x00620834 + FableWin.pdb (public const, NInventory::CItem); the callee 0x005BD82E is Ego_r's
//   private const GetInventoryItemDef returning CDefPointer<const CInventoryItemDef>.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD931, length 58 bytes.
''')
# caller 0x005BDDE4 NInventory::CItem::AddInventoryItem (Ego_r ?AddInventoryItem@CItem@NInventory@@QAEXABV12@@Z)
edit('005bdde4', 'src', '''struct CItem {
    long                ItemType;                  // +0x00''', '''namespace NInventory {
class CItem {                                      // NInventory::CItem (Ego_r.pdb / FableWin.pdb)
public:
    long                ItemType;                  // +0x00''')
edit('005bdde4', 'src', '''    long GetMaximumNumberOfItems();                // 0x005BD931
    void AddInventoryItem(const CItem& other);
};

void CItem::AddInventoryItem(const CItem& other)''', '''    long GetMaximumNumberOfItems() const;          // 0x005BD931 ?GetMaximumNumberOfItems@CItem@NInventory@@QBEJXZ
    void AddInventoryItem(const CItem& other);
};
}

void NInventory::CItem::AddInventoryItem(const CItem& other)''')
edit('005bdde4', 'src', '''// CItem::AddInventoryItem(const CItem& other) -- merge''', '''// NInventory::CItem::AddInventoryItem(const CItem& other) -- merge''')
# ---- 0x00416231 ?GetCurrentGameTime@CMainGameComponent@@IBENXZ (Ego_r 0x00415496 + FableWin; protected const, double)
recode('00416231', 'src', 'class CMainGameComponent { public:', '''class CMainGameComponent { public:
    char pad[0x60];
    double gameTimeBase;                    // +0x60
protected:
    double GetCurrentGameTime() const;
};
extern double __fastcall ReadCurrentGameTime(CMainGameComponent* self);
double CMainGameComponent::GetCurrentGameTime() const {
    return ReadCurrentGameTime(const_cast<CMainGameComponent*>(this)) - gameTimeBase;
}
''', line1='// CMainGameComponent::GetCurrentGameTime() const -- the game clock (0x009E1BC0 on this) less the game start time at +0x60',
     leaf='?GetCurrentGameTime@CMainGameComponent@@IBENXZ',
     extra_comment='// Identity (naming wave 11): Ego_r.pdb 0x00415496 + FableWin.pdb, protected const returning double; Ego_r order GetCurrentGameTime /\n//   ConvertGTToWF / GetRenderInterpolate / GetPredictedTimeSinceLastRenderFrame = retail 0x00416231 / 0x0041623E / 0x00416251 / 0x00416258.')
edit('004164e0', 'src', '''    double GetCurrentGameTime();                  // 0x00416231
''', '''protected:
    double GetCurrentGameTime() const;            // 0x00416231 ?GetCurrentGameTime@CMainGameComponent@@IBENXZ (Ego_r / FableWin)
public:
''')
# ---- 0x00416258 (its caller): ?GetPredictedTimeSinceLastRenderFrame@CMainGameComponent@@IBENXZ (Ego_r 0x004154B7 + FableWin, the
#      member after GetRenderInterpolate; body = GetCurrentGameTime() - LastRenderFrameStartTime +0x161A8, as ForceRender 0x004164E0 stores it)
recode('00416258', 'src', 'struct S; extern double __fastcall ggt_helper(S*);', '''class CMainGameComponent {
public:
    char   _pad[0x161a8];
    double LastRenderFrameStartTime;        // +0x161a8 (ForceRender 0x004164E0 stores GetCurrentGameTime() here)
protected:
    double GetCurrentGameTime() const;      // 0x00416231
    double GetPredictedTimeSinceLastRenderFrame() const;
};
double CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const
{
    return GetCurrentGameTime() - LastRenderFrameStartTime;
}
''', line1='// CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const -- the game time since the last render frame started',
     leaf='?GetPredictedTimeSinceLastRenderFrame@CMainGameComponent@@IBENXZ',
     extra_comment='// Identity (naming wave 11): Ego_r.pdb 0x004154B7 + FableWin.pdb (protected const, double); Ego_r member order GetCurrentGameTime /\n//   ConvertGTToWF / GetRenderInterpolate / GetPredictedTimeSinceLastRenderFrame = retail 0x00416231 / 0x0041623E / 0x00416251 / 0x00416258.\n// Retail: Fable.exe (TLC, VC7.1) @ 0x00416258, 16 bytes: push esi ; mov esi,ecx ; call 0x00416231 ; fsub [esi+0x161A8] ; pop esi ; ret.')
# ---- 0x0066FF20 ?SwapToHero@CTCCoopSpirit@@AAEXXZ (Ego_r + FableWin, private): caller CTCCoopSpirit::UpdateScore 0x00670710
edit('00670710', 'src', '''    void SwapToHero();                                      // 0x0066FF20
    void UpdateScore();
};''', '''    void UpdateScore();
private:
    void SwapToHero();                                      // 0x0066FF20 ?SwapToHero@CTCCoopSpirit@@AAEXXZ (Ego_r / FableWin, private)
};''')
# ---- 0x00686D40 ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ (Ego_r + FableWin, protected, returns a reference): caller
#      CTCTeleporter::FrameUpdate 0x007E00D0 (the vptr moves into the CTCBase base at +0x00; the member offsets stay)
edit('007e00d0', 'src', '''#pragma pack(push, 1)
class CTCTeleporter { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04''', '''#pragma pack(push, 1)
class CTCBase { public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();                 // 0x00686D40 ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ (Ego_r / FableWin)
};
class CTCTeleporter : public CTCBase { public:
    CThing* ParentThing;                                // +0x04''')
edit('007e00d0', 'src', '''    CPlayerManager* GetPlayerManager();                 // 0x00686D40
''', '')
edit('007e00d0', 'src', '''GetPlayerManager()->GetMainPlayer()''', '''GetPlayerManager().GetMainPlayer()''')
# ---- 0x00414C90 ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ (FableWin.pdb; static __fastcall returning a reference,
#      as CNewFrontendGameComponent::Input 0x0042E3EE already binds it): callers 0x0042F75E and 0x00687FD0
edit('0042f75e', 'src', '''class CConsole {
public:
    static CConsole* Get();                                                  // 0x00414C90
    void Initialise(char toggleCharacter, EInputKey toggleKey, CFontBank* font);   // 0x009ED190
};''', '''class CConsole {
public:
    void Initialise(char toggleCharacter, EInputKey toggleKey, CFontBank* font);   // 0x009ED190
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                    // 0x00414C90 (CConsole) ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ (FableWin)
};''')
edit('0042f75e', 'src', '''    CConsole::Get()->Initialise(''', '''    CTBaseSingleton<CConsole>::Get().Initialise(''')
edit('00687fd0', 'src', '''    static CConsole* __fastcall Get();             // 0x00414C90
    bool ProcessInput(const CInputEvent& event);   // 0x009EA540
};''', '''    bool ProcessInput(const CInputEvent& event);   // 0x009EA540
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                    // 0x00414C90 (CConsole) ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ (FableWin)
};''')
edit('00687fd0', 'src', '''    if (CConsole::Get()->ProcessInput(event))''', '''    if (CTBaseSingleton<CConsole>::Get().ProcessInput(event))''')
# ---- 0x009EBB30 ??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z (Ego_r.pdb 0x00B3E750 + FableWin; InitialiseConsoleVariables
#      0x00413C50 already binds it): callers 0x006BBC30 and 0x00414F30 (the 4-byte enum by value, as the `long` model passed it)
edit('006bbc30', 'src', '''class CConsoleCommandParameters { public:                    // 12-byte vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters(long type);             // 0x009EBB30
};
enum EGenericVarType { GVT_FLOAT = 3 };''', '''enum EGenericVarType { GVT_FLOAT = 3 };
class CConsoleCommandParameters { public:                    // 12-byte vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters(EGenericVarType type);  // 0x009EBB30 ??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z (Ego_r / FableWin)
};''')
edit('00414f30', 'src', '''struct CConsoleCommandParameters_Retail {                       // 12 bytes: a retail std::vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters_Retail(long type);                // 0x009EBB30: one element
};''', '''enum EGenericVarType { };
class CConsoleCommandParameters { public:                       // 12 bytes: a retail std::vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters(EGenericVarType type);            // 0x009EBB30: one element ??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z (Ego_r / FableWin)
};''')
edit('00414f30', 'src', '''    CConsoleCommandParameters_Retail ParamTypes;                // +0x08''', '''    CConsoleCommandParameters ParamTypes;                       // +0x08''')
edit('00414f30', 'src', '''    , ParamTypes(type)''', '''    , ParamTypes((EGenericVarType)type)''')
# ---- 0x00595582 ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ (Ego_r.pdb 0x006043BE + FableWin 0x02010AF6; static, returns a
#      pointer): the defining row drops its extern "C" stand-in; callee 0x005953E2 = ??0CFrontEndManager@NUISystem@@QAE@XZ (Ego_r 0x006042D3,
#      the PDB member before GetInstance; landed row 0x005953E2 defines it); new(0xE0) = the retail sizeof (engine/CFrontEndManager.h).
#      The instance pointer 0x013B8B5C keeps its bound extern "C" spelling (no PDB data name).
recode('00595582', 'src', '#pragma optimize("s", on)', '''#pragma optimize("s", on)

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char _body[0xe0];                           // retail sizeof (engine/CFrontEndManager.h)
    CFrontEndManager();                                  // 0x005953E2 ??0CFrontEndManager@NUISystem@@QAE@XZ
    static CFrontEndManager* __fastcall GetInstance();
};
}
extern "C" NUISystem::CFrontEndManager* FableFrontEndManagerSingleton;   // 0x013B8B5C

NUISystem::CFrontEndManager* NUISystem::CFrontEndManager::GetInstance()
{
    if (FableFrontEndManagerSingleton == 0)
        FableFrontEndManagerSingleton = new CFrontEndManager;
    return FableFrontEndManagerSingleton;
}

#pragma optimize("s", off)
''', line1='// NUISystem::CFrontEndManager::GetInstance() @ 0x00595582 -- the front-end manager singleton, created (0xE0 bytes) on first use',
    leaf='?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ',
    extra_comment='// Identity (naming wave 11): Ego_r.pdb 0x006043BE + FableWin.pdb 0x02010AF6 ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ; the ctor 0x005953E2 precedes it as in Ego_r.')
recode('00595582', 'tst', '#include <cstdio>', '''#include <cstdio>

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char _body[0xe0];
    CFrontEndManager();
    static CFrontEndManager* __fastcall GetInstance();
};
}
using NUISystem::CFrontEndManager;
extern "C" CFrontEndManager* FableFrontEndManagerSingleton = 0;

static unsigned char g_storage[0xe0];
static CFrontEndManager* g_constructed = (CFrontEndManager*)g_storage;
static void* g_allocationResult;
static unsigned long g_allocatedBytes;
static int g_allocateCalls;
static void* g_constructorStorage;
static int g_constructorCalls;

void* __cdecl operator new(unsigned int bytes)
{
    ++g_allocateCalls;
    g_allocatedBytes = (unsigned long)bytes;
    return g_allocationResult;
}

CFrontEndManager::CFrontEndManager()
{
    ++g_constructorCalls;
    g_constructorStorage = this;
}

static void Reset()
{
    FableFrontEndManagerSingleton = 0;
    g_allocationResult = 0;
    g_allocatedBytes = 0;
    g_allocateCalls = 0;
    g_constructorStorage = 0;
    g_constructorCalls = 0;
}

static bool RunExisting()
{
    static unsigned char other[0xe0];
    Reset();
    FableFrontEndManagerSingleton = (CFrontEndManager*)other;
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == (CFrontEndManager*)other && g_allocateCalls == 0 && g_constructorCalls == 0;
}

static bool RunAllocationFailure()
{
    Reset();
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == 0 && FableFrontEndManagerSingleton == 0 && g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 && g_constructorCalls == 0;
}

static bool RunConstruction()
{
    Reset();
    g_allocationResult = g_storage;
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == g_constructed && FableFrontEndManagerSingleton == g_constructed && g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 && g_constructorCalls == 1 && g_constructorStorage == g_storage;
}

int main()
{
    const bool existing = RunExisting();
    const bool failed = RunAllocationFailure();
    const bool constructed = RunConstruction();
    if (!existing || !failed || !constructed)
    {
        std::printf("FSE2_00595582_TEST FAIL existing=%d failed=%d constructed=%d\\n",
            existing ? 1 : 0, failed ? 1 : 0, constructed ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595582_TEST PASS");
    return 0;
}
''')
edit('00595582', 'src', '''// The retail singleton accessor is compiled for size at this leaf.  The
// explicit failure branch preserves retail's `jmp`/`xor eax,eax` sequence,
// while the one-argument fastcall constructor declaration preserves the
// untouched EDX register in the constructor call.  No assembly is needed.''', '''// Shape: a size TU (`pop ecx` after the allocation call); VC7.1's null-tested new expression (`jmp` / `xor eax,eax`) stores
// the instance from eax on both paths and returns it.''')
# ---- 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r.pdb 0x00404100 + FableWin 0x018BD220; static __fastcall returning a reference;
#      already bound by CEditBox::SetActive 0x005401B0). Body `mov eax,0x013B7CD8 ; ret`: the 0x54-byte CIME object (Ego_r sizeof CIME = 84).
#      The object keeps its bound data spelling (no PDB data name). Member callees, by Ego_r / FableWin member order against retail:
#      FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0,
#      Render 0x00404C00, IsIMEActive 0x00404C50 (const); Shutdown 0x004051C0 (tests IsInitialised +8, calls the ImeUi uninit
#      helpers, then the inlined Reset zeroes EnableIME / WindowHandle / IsInitialised / Appearance[0x40] / PEditBox).
recode('00404a80', 'src', 'extern unsigned char g_primitiveVTableOrData[0x54];', '''namespace NUISystem {
class CIME {
public:
    unsigned char _body[0x54];                            // Ego_r.pdb sizeof CIME = 0x54
    static CIME& __fastcall Get();
};
}
extern unsigned char g_primitiveVTableOrData[0x54];   // the CIME object at 0x013B7CD8 (bound data spelling kept)

NUISystem::CIME& NUISystem::CIME::Get()
{
    return *(CIME*)g_primitiveVTableOrData;
}
''', line1='// NUISystem::CIME::Get() -- returns the IME singleton object at 0x013B7CD8',
    leaf='?Get@CIME@NUISystem@@SIAAV12@XZ',
    extra_comment='// Identity (naming wave 11): Ego_r.pdb 0x00404100 + FableWin.pdb 0x018BD220 ?Get@CIME@NUISystem@@SIAAV12@XZ (first CIME member in both retail and Ego_r); the object is 0x54 bytes = Ego_r sizeof CIME.')
edit('00404a80', 'src', '''// `mov eax, offset g_engineRoot; ret`. This is the "get the engine" accessor that GFInitialise and the
// shutdown path use to reach the engine root object (the one holding the display, primitive, texture and
// resource managers). The AddChildPrimitive label on this address is a propagated name and has nothing
// to do with what the function does; it is a global-object getter.
//
// Matching: returns the address of a global (absolute relocation), not its contents.''', '''// `mov eax, offset <object>; ret`: GFUninitialise 0x00401B80 calls CIME::Shutdown 0x004051C0 on the result, the front end
// IsIMEActive / FinaliseString / SetEditBoxCaretPos / ShouldEditBoxShowCaret. The AddChildPrimitive label was propagated.
//
// Matching: returns the address of a global (absolute relocation), not its contents.''')
recode('00404a80', 'tst', '#include <cstdio>', '''#include <cstdio>

namespace NUISystem {
class CIME {
public:
    unsigned char _body[0x54];
    static CIME& __fastcall Get();
};
}
unsigned char g_primitiveVTableOrData[0x54] = {};

int main()
{
    NUISystem::CIME& r = NUISystem::CIME::Get();
    if ((void*)&r == (void*)&g_primitiveVTableOrData[0]) {
        std::printf("CEngineInternalPrimitiveBase_00404a80_TEST PASS\\n");
        return 0;
    }
    std::printf("FAIL\\n");
    return 1;
}
''')
# callers
edit('00851860', 'src', '''struct CIMECalls {
    bool IsIMEActive();                     // 0x00404C50
    void FinaliseString(bool commit);       // 0x00404B50
};

extern CIMECalls* GFGetIME();               // 0x00404A80''', '''namespace NUISystem {
class CIME { public:
    static CIME& __fastcall Get();          // 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r / FableWin)
    void FinaliseString(bool commit);       // 0x00404B50 ?FinaliseString@CIME@NUISystem@@QAEX_N@Z
    bool IsIMEActive() const;               // 0x00404C50 ?IsIMEActive@CIME@NUISystem@@QBE_NXZ
};
}''')
edit('00851860', 'src', '''        if (GFGetIME()->IsIMEActive())
            GFGetIME()->FinaliseString(false);''', '''        if (NUISystem::CIME::Get().IsIMEActive())
            NUISystem::CIME::Get().FinaliseString(false);''')
edit('00540980', 'src', '''struct CIME {
    bool IsOn();                                            // 0x00404BA0
    void SetCompositionPosition(long x, long y);            // 0x00404B80
};
CIME* GFGetIME();                                           // 0x00404A80''', '''namespace NUISystem {
class CIME { public:
    static CIME& __fastcall Get();                          // 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r / FableWin)
    void SetEditBoxCaretPos(long x, long y);                // 0x00404B80 ?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z
    bool ShouldEditBoxShowCaret();                          // 0x00404BA0 ?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ
};
}''')
edit('00540980', 'src', '''    if (Active && GFGetIME()->IsOn()) {''', '''    if (Active && CIME::Get().ShouldEditBoxShowCaret()) {''')
edit('00540980', 'src', '''            GFGetIME()->SetCompositionPosition((long)(position.X + 3.0f), (long)(position.Y + 3.0f));''', '''            CIME::Get().SetEditBoxCaretPos((long)(position.X + 3.0f), (long)(position.Y + 3.0f));''')
edit('00540980', 'src', '''GFGetIME 0x00404A80, CIME 0x00404BA0 (IME on) / 0x00404B80 (composition position)''', '''CIME::Get 0x00404A80, CIME::ShouldEditBoxShowCaret 0x00404BA0 / CIME::SetEditBoxCaretPos 0x00404B80''')
edit('0054e280', 'src', '''class CIME {
public:
 static CIME* __fastcall GetInstance();       // 0x00404A80 (returns the static 0x013B7CD8)
 bool IsIMEActive();                          // 0x00404C50
};
namespace NUISystem {''', '''namespace NUISystem {
class CIME {
public:
 static CIME& __fastcall Get();               // 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r / FableWin; the static 0x013B7CD8)
 bool IsIMEActive() const;                    // 0x00404C50 ?IsIMEActive@CIME@NUISystem@@QBE_NXZ
};''')
edit('0054e280', 'src', '''!CIME::GetInstance()->IsIMEActive()''', '''!CIME::Get().IsIMEActive()''')
edit('0054e280', 'src', '''CIME::GetInstance 0x00404A80 +''', '''CIME::Get 0x00404A80 +''')
edit('00401b80', 'src', '''class CEnginePrimitiveManager { public:
    void Uninitialise();                          // 0x004051C0
};''', '''namespace NUISystem {
class CIME { public:
    static CIME& __fastcall Get();                // 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ (Ego_r / FableWin)
    void Shutdown();                              // 0x004051C0 ?Shutdown@CIME@NUISystem@@QAEXXZ (Ego_r / FableWin)
};
}''')
edit('00401b80', 'src', '''CEnginePrimitiveManager* __fastcall GetEnginePrimitiveManager();   // 0x00404A80
''', '')
edit('00401b80', 'src', '''    GetEnginePrimitiveManager()->Uninitialise();''', '''    NUISystem::CIME::Get().Shutdown();''')
edit('00401b80', 'src', '''// 0x00404A80 (`mov eax,0x013B7CD8 ; ret`) -> its Uninitialise 0x004051C0 (tail `jmp`).''', '''// NUISystem::CIME::Get 0x00404A80 (`mov eax,0x013B7CD8 ; ret`) -> CIME::Shutdown 0x004051C0 (tail `jmp`).''')
edit('00401b80', 'src', '''// call is sibling-call optimised to `jmp`. The manifest labels 0x00404A80 as
// CEngineInternalPrimitiveBase::AddChildPrimitive (propagated): its body only returns the
// address of the static object at 0x013B7CD8.''', '''// call is sibling-call optimised to `jmp`. 0x00404A80 = Ego_r ?Get@CIME@NUISystem@@SIAAV12@XZ (the manifest's
// CEngineInternalPrimitiveBase::AddChildPrimitive label was propagated).''')
# ---- the CIME member rows (Get's family): Ego_r.pdb + FableWin.pdb NUISystem::CIME members in retail address order (above); their ImeUi
#      callees by Ego_r.pdb name, arity and retail neighbour order (Ego_r SetCaretPosition < SetCompStringAppearance ... IsEnabled;
#      SetAppearance < GetCaretStatus < SetScreenDimension = retail 0x00CB2E20 < 0x00CB2EC0 < 0x00CB2EE0; 0x00CB2880 already defines
#      the Ego_r ImeUi_SetCompStringAppearance spelling). CIME layout = Ego_r: EnableIME +0, WindowHandle +4, IsInitialised +8,
#      Appearance +0xC (0x40), PEditBox +0x4C, CancelOnOKClick +0x50.
IDC = '// Identity (naming wave 11): Ego_r.pdb + FableWin.pdb %s (NUISystem::CIME members in retail = PDB order: Get 0x00404A80, FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0, Render 0x00404C00, IsIMEActive 0x00404C50).'
recode('00404b50', 'src', 'struct CIME {', '''namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08 the IME was set up at start-up
    unsigned char _pad09[0x47];                  // WindowHandle, Appearance, PEditBox
    bool          CancelOnOKClick;               // +0x50 profile option
    void FinaliseString(bool finalise);
};
}

void __fastcall ImeUi_FinalizeString(bool send);   // 0x00CB2C60 ?ImeUi_FinalizeString@@YIX_N@Z (Ego_r)

void NUISystem::CIME::FinaliseString(bool finalise)
{
    if (IsInitialised) {
        char v = finalise;
        // Finalise becomes cancel when the player asked for OK to cancel.
        if (finalise) {
            if (CancelOnOKClick)
                v = 0;
        }
        ImeUi_FinalizeString(v != 0);
    }
}
''', line1='// NUISystem::CIME::FinaliseString(bool finalise) -- tell the IME UI to commit or cancel the composition, honouring CancelOnOKClick',
    leaf='?FinaliseString@CIME@NUISystem@@QAEX_N@Z', extra_comment=IDC % '?FinaliseString@CIME@NUISystem@@QAEX_N@Z')
recode('00404b80', 'src', 'struct CIME {', '''namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08
    void SetEditBoxCaretPos(long x, long y);
};
}

void __fastcall ImeUi_SetCaretPosition(unsigned int x, unsigned int y);   // 0x00CB2860 ?ImeUi_SetCaretPosition@@YIXII@Z (Ego_r)

void NUISystem::CIME::SetEditBoxCaretPos(long x, long y)
{
    if (IsInitialised)
        ImeUi_SetCaretPosition(x, y);
}
''', line1='// NUISystem::CIME::SetEditBoxCaretPos(long x, long y) -- forward the edit box caret position to the IME UI when the IME is initialised',
    leaf='?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z', extra_comment=IDC % '?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z')
edit('00404b80', 'src', '''// arguments go straight to the UI helper. The FinaliseString name on this address is shared with
// 0x00404B50; the two bodies differ in signature, so this appears to be a sibling forwarding member.''', '''// arguments go straight to ImeUi_SetCaretPosition (the propagated FinaliseString label was 0x00404B50's).''')
recode('00404ba0', 'src', 'struct CNetworkClient {', '''namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08
    bool ShouldEditBoxShowCaret();
};
}

bool __fastcall ImeUi_GetCaretStatus();          // 0x00CB2EC0 ?ImeUi_GetCaretStatus@@YI_NXZ (Ego_r)

bool NUISystem::CIME::ShouldEditBoxShowCaret()
{
    if (IsInitialised)
        return ImeUi_GetCaretStatus();
    return true;
}
''', line1='// NUISystem::CIME::ShouldEditBoxShowCaret() -- with the IME up, ask the IME UI whether the caret shows; otherwise answer true',
    leaf='?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ', extra_comment=IDC % '?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ (the CNetworkClient label was propagated)')
edit('00404ba0', 'src', '''// A guarded forwarder: the byte at +8 says whether this object is active, and only then does the
// question go to the real implementation. An inactive object answers "yes" so callers that loop on
// "is the package set / is it done" fall straight through instead of waiting on something that will
// never happen. The CNetworkClient name is what the symbol tables attach to this address; the shape is
// generic and the PDB's CNetworkClient has its embedded server object at +8, so the attribution is
// uncertain.''', '''// A guarded forwarder: IsInitialised (+8) gates the ImeUi query; without the IME the edit box always shows its caret.
// Caller: CEditBox::UpdateDisplayText 0x00540980 (draws the caret and sets SetEditBoxCaretPos).''')
recode('00404c50', 'src', 'struct CIME {', '''namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                   // +0x08
    bool IsIMEActive() const;
};
}

bool __fastcall ImeUi_IsEnabled();                 // 0x00CB28E0 ?ImeUi_IsEnabled@@YI_NXZ (Ego_r; `mov al,[0x0143E890] ; ret`)

bool NUISystem::CIME::IsIMEActive() const
{
    if (IsInitialised && ImeUi_IsEnabled())
        return true;
    return false;
}
''', line1='// NUISystem::CIME::IsIMEActive() const -- is the IME initialised and currently switched on by the player?',
    leaf='?IsIMEActive@CIME@NUISystem@@QBE_NXZ', extra_comment=IDC % '?IsIMEActive@CIME@NUISystem@@QBE_NXZ')
edit('00404c50', 'src', '''// Matching: the callee at 0x00CB28E0 carries the propagated label "CAIStateGroup_Freeze::IsRunnable";''', '''// Matching: the callee 0x00CB28E0 is ImeUi_IsEnabled (the label "CAIStateGroup_Freeze::IsRunnable" was propagated);''')
edit('00404bb0', 'src', '''struct SRect_00404bb0 { long left, top, right, bottom; };
// IME UI: position and style the composition string (font, colour, rectangle).
void __fastcall ImeUi_SetCompStringAppearance(void* font, long colour, const SRect_00404bb0* rect);   // 0x00CB2880
struct CIME {
    unsigned char _pad[8];
    bool          m_active;                        // IsInitialised
    unsigned char _pad09[0x13];
    void*         m_pFont;                         // Appearance.font: the font the composition string is drawn in
    void SetEditBoxRegion(long x, long y, long width, long height);
};

void CIME::SetEditBoxRegion(long x, long y, long width, long height)
{
    if (m_active) {
        // Edit box rectangle in screen space.
        SRect_00404bb0 rect;''', '''struct tagRECT { long left, top, right, bottom; };
// IME UI: position and style the composition string (font, colour, rectangle).
class CImeUiFont_Base;
void __fastcall ImeUi_SetCompStringAppearance(CImeUiFont_Base* font, unsigned long colour, const tagRECT* rect);   // 0x00CB2880 (Ego_r spelling, as row 0x00CB2880 defines it)
namespace NUISystem {
class CIME {
public:
    unsigned char    _pad00[0x8];
    bool             IsInitialised;                // +0x08
    unsigned char    _pad09[0x13];
    CImeUiFont_Base* m_pFont;                      // +0x1C Appearance.font: the font the composition string is drawn in
    void SetEditBoxRegion(long x, long y, long width, long height);
};
}

void NUISystem::CIME::SetEditBoxRegion(long x, long y, long width, long height)
{
    if (IsInitialised) {
        // Edit box rectangle in screen space.
        tagRECT rect;''')
edit('00404bb0', 'src', '''// CIME::SetEditBoxRegion(long x, long y, long width, long height)''', '''// Leaf: ?SetEditBoxRegion@CIME@NUISystem@@QAEXJJJJ@Z
// NUISystem::CIME::SetEditBoxRegion(long x, long y, long width, long height)''')
edit('00404c00', 'src', '''void __fastcall ImeUI_RenderCompositionString(bool a, bool b);''', '''void __fastcall ImeUi_RenderUI(bool drawCompAttr, bool drawOtherUi);   // 0x00CB38E0 ?ImeUi_RenderUI@@YIX_N0@Z (Ego_r; the only (bool, bool) ImeUi entry)''')
edit('00404c00', 'src', '''struct CIME {
    unsigned char             _pad[8];
    bool                      m_active;            // IsInitialised
    unsigned char             _pad09[0x43];
    CThemeProvider_00404c00*  m_pTheme;            // PEditBox: the edit box currently taking IME input
    void Render();
};

void CIME::Render()
{
    if (m_active) {''', '''namespace NUISystem {
class CIME {
public:
    unsigned char             _pad00[0x8];
    bool                      IsInitialised;       // +0x08
    unsigned char             _pad09[0x43];
    CThemeProvider_00404c00*  m_pTheme;            // +0x4C PEditBox: the edit box currently taking IME input
    void Render();
};
}

void NUISystem::CIME::Render()
{
    if (IsInitialised) {''')
edit('00404c00', 'src', '''        ImeUI_RenderCompositionString(flags, flags);''', '''        ImeUi_RenderUI(flags, flags);''')
edit('00404c00', 'src', '''// CIME::Render() -- draw''', '''// Leaf: ?Render@CIME@NUISystem@@QAEXXZ
// NUISystem::CIME::Render() -- draw''')
# NUISystem::CManager::GetInstance 0x0041E5F2 is static __fastcall in Ego_r 0x00438C20 + FableWin 0x01AE5507 (SI): every caller binds SI
edit('0041e5f2', 'src', '''    static CManager* GetInstance();''', '''    static CManager* __fastcall GetInstance();          // ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (Ego_r / FableWin)''')
