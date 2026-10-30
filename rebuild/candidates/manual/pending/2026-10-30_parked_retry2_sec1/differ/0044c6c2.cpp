// CGameDefinitionManager::CGameDefinitionManager() -- the game's definition manager: CDefinitionManager over the "game" def set with this class's error callback, cached def slots cleared, 512 KB resource budget
// Grade: RELOCATION_MATCH with a LOCAL def-pointer stand-in (not landable as is) -- the four +0xD0..+0xDC members are
//   CDefPointer<const ...> handles: the retail destructor ~CGameDefinitionManager 0x00450BFF releases each one with the shared
//   CDefPointer::Release shape (`mov ecx,[m] ; test ; dec [ecx+4] ; jne ; call [eax+4] ; and [m],0`, +0xDC first), so the ctor
//   zeroes them through their null constructor, and member objects with an inline constructor give retail's scheduling
//   (vptr, D0, D4, D8, push 80000h / mov ecx,esi, DC). Blocked: engine/CDefPointer.h does not declare the FableWin.pdb null
//   constructor ??0..QAE@W4ENullDef@@@Z (the header lists it as unproven); with it, model the members as CDefPointer<const T>
//   (types unknown: Ego_r.pdb has CGameDefinitionManager only as a forward type) and land. Parked: 93v93 (plain void* members in
//   the init list; body assignments give the same 93v93).
// Leaf: ??0CGameDefinitionManager@@QAE@XZ
// Flags: /Zc:wchar_t
// Retail: Fable.exe @ 0x0044C6C2, 93 bytes (/Os TU: unaligned entry, ebp frame with `leave`), thiscall `ret`.
// Identity: the stored vtable 0x01232C24 is RTTI .?AVCGameDefinitionManager@@ (bases CDefinitionManager ->
// CResourceList); PDB name set `??0CGameDefinitionManager@@QAE@XZ`; caller 0x00418547 (the game component's start-up).
// Callees: CWideString::CWideString(const wchar_t*) 0x0099B6B0 on the literal L"game" 0x01232C30;
// CDefinitionManager::CDefinitionManager(const CWideString&, void (__fastcall*)(const CCharString&)) 0x009B0470 (PDB
// spelling `??0CDefinitionManager@@QAE@ABVCWideString@@P6IXABVCCharString@@@Z@Z`) with
// CGameDefinitionManager::ErrorCallback 0x0044C6AF (PDB `?ErrorCallback@CGameDefinitionManager@@KIXABVCCharString@@@Z`,
// protected static __fastcall); CWideString::~CWideString 0x0099B510 (the temporary); CResourceList::SetMaximumMemory
// 0x009FC520 (Ego_r `?SetMaximumMemory@CResourceList@@QAEXK@Z`) with 0x80000.
// Layout: CGameDefinitionManager's own four dwords +0xD0..+0xDC (retail CDefinitionManager is 0xD0 bytes); Ego_r.pdb has no
// layout for the derived class, so the four members carry descriptive names.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString;
class CWideString { public:
    CWideString(const wchar_t* text);                    // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
    void* PStringData;
};
class CResourceList { public:
    virtual ~CResourceList();
    void SetMaximumMemory(unsigned long maximum);        // 0x009FC520
    unsigned char _pad04[0x28];
};
class CDefinitionManager : public CResourceList { public:
    CDefinitionManager(const CWideString& defSetName, void (__fastcall* errorCallback)(const CCharString&));   // 0x009B0470
    unsigned char _pad2c[0xA4];
};
class CGameDefinitionManager : public CDefinitionManager { public:
    CGameDefinitionManager();
protected:
    static void __fastcall ErrorCallback(const CCharString& message);   // 0x0044C6AF
public:
    struct CCachedDef { void* Object; CCachedDef() : Object(0) { } };
    CCachedDef m_pCachedDef0;                            // +0xD0
    CCachedDef m_pCachedDef1;                            // +0xD4
    CCachedDef m_pCachedDef2;                            // +0xD8
    CCachedDef m_pCachedDef3;                            // +0xDC
};
CGameDefinitionManager::CGameDefinitionManager()
    : CDefinitionManager(L"game", ErrorCallback)
{
    SetMaximumMemory(0x80000);
}
