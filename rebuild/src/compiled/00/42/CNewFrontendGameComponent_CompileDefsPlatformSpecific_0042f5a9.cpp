// CNewFrontendGameComponent::CompileDefsPlatformSpecific(bool xbox) -- build the startup definition manager over (x)frontend.bin, register the front-end def classes and compile them
// Leaf: ?CompileDefsPlatformSpecific@CNewFrontendGameComponent@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F5A9, 354 bytes, `ret`; FableWin.pdb
//   `?CompileDefsPlatformSpecific@CNewFrontendGameComponent@@SIX_N@Z` (static __fastcall, the flag in cl). Caller:
//   CNewFrontendGameComponent::CompileDefs 0x0042F70B (`(true)` when the create-compiled-defs flag 0x013B8648 is set,
//   then a `(false)` tail jump).
// Callees: CWideString() 0x0099AED0, CWideString::operator=(const unsigned short*) 0x0099B800, scalar operator new
//   0x00BFEA1A (0xD0 bytes), CStartupDefinitionManager(const CWideString&) 0x00433693, CStartupDefinitionManager::Set
//   0x004336B0 / Get 0x0043368D (the counted pointer 0x013B8760), CCharString(const char*, long) 0x0099EBF0, CCharString
//   copy 0x0099EC30, CDefinitionManager::AddDefClass(const CDefClassInit&) 0x009B0AC0, ~CCharString 0x0099EAE0,
//   CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool) 0x004336BC, ~CWideString 0x0099B510.
//   Def allocators: CFrontEndDef::Alloc 0x00433080, NUISystem::CUIMiscThingsDef::Alloc 0x00432C40,
//   NUISystem::CUIIconsDef::Alloc 0x00432E00, CControlsDef::Alloc 0x00432180 (Ego_r.pdb names `?Alloc@...@@SIPAVCDefClassBase@@XZ`).
// Layout: Ego_r.pdb CDefClassInit (Name +0, AllocFunc +4), built in the frame with its constructor inlined; the
//   startup manager is 0xD0 bytes in retail (the operator new size).
// Shape: frame-pointer size TU (`pop ecx` after new, `leave`): `#pragma optimize("s", on)` + `("y", off)`. The file name is
//   an if/else of two assignments (tail-merged into `push A ; jmp ; push B ; call`; a ternary selects into eax). The literal is an
//   IMPLICIT CCharString conversion (re-addressed with `lea` for the copy), the allocator stored after the copy, both
//   strings destroyed in reverse; the manager reference is fetched once into esi. The PC-only control scheme is
//   registered only when not building the Xbox set.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString {
public:
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
private:
    void* m_pData;
};

class CCharString {
public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
private:
    void* m_pData;
};

class CDefClassBase;
typedef CDefClassBase* (__fastcall* TDefClassAllocFunc)();

class CDefClassInit {
public:
    CCharString        Name;                             // +0x00
    TDefClassAllocFunc AllocFunc;                        // +0x04
    CDefClassInit(const CCharString& name, TDefClassAllocFunc allocFunc) : Name(name), AllocFunc(allocFunc) { }
};

class CDefinitionManager {
public:
    void AddDefClass(const CDefClassInit& init);         // 0x009B0AC0
};

class CStartupDefinitionManager : public CDefinitionManager {
public:
    CStartupDefinitionManager(const CWideString& definitionsFile);       // 0x00433693
    static void __fastcall Set(CStartupDefinitionManager* manager);     // 0x004336B0
    static CStartupDefinitionManager& __fastcall Get();                 // 0x0043368D
    void InitAndCompilePlatformSpecific(bool xbox);                     // 0x004336BC
    unsigned char _body[0xD0];
};

class CFrontEndDef { public: static CDefClassBase* __fastcall Alloc(); };              // 0x00433080
class CControlsDef { public: static CDefClassBase* __fastcall Alloc(); };              // 0x00432180
namespace NUISystem {
class CUIMiscThingsDef { public: static CDefClassBase* __fastcall Alloc(); };          // 0x00432C40
class CUIIconsDef { public: static CDefClassBase* __fastcall Alloc(); };               // 0x00432E00
}

class CNewFrontendGameComponent {
public:
    static void __fastcall CompileDefsPlatformSpecific(bool xbox);
};

void __fastcall CNewFrontendGameComponent::CompileDefsPlatformSpecific(bool xbox)
{
    CWideString definitionsFile;
    if (xbox)
        definitionsFile = L"xfrontend.bin";
    else
        definitionsFile = L"frontend.bin";
    CStartupDefinitionManager::Set(new CStartupDefinitionManager(definitionsFile));
    CStartupDefinitionManager& manager = CStartupDefinitionManager::Get();
    manager.AddDefClass(CDefClassInit("FRONT_END", CFrontEndDef::Alloc));
    manager.AddDefClass(CDefClassInit("UI_MISC_THINGS_DEF", NUISystem::CUIMiscThingsDef::Alloc));
    manager.AddDefClass(CDefClassInit("UI_ICONS_DEF", NUISystem::CUIIconsDef::Alloc));
    if (!xbox)
        manager.AddDefClass(CDefClassInit("CONTROL_SCHEME", CControlsDef::Alloc));
    manager.InitAndCompilePlatformSpecific(xbox);
}