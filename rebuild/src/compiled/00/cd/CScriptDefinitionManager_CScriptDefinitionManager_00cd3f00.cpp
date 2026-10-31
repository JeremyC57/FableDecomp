// CScriptDefinitionManager::CScriptDefinitionManager() -- base CDefinitionManager(CWideString(L"script.bin"), callback) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3F00, length 55 bytes.
// Leaf: ??0CScriptDefinitionManager
// Retail bytes: push ecx ; push esi ; mov esi,ecx ; push offset L"script.bin" ; lea ecx,[esp+8] ; call CWideString::CWideString ; push offset callback ;
//               lea eax,[esp+8] ; push eax ; mov ecx,esi ; call CDefinitionManager::CDefinitionManager ; lea ecx,[esp+4] ; call CWideString::~CWideString ;
//               mov dword ptr [esi],offset vftable(0x012C2648) ; mov eax,esi ; pop esi ; pop ecx ; ret
// Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, CDefinitionManager::CDefinitionManager(const CWideString&,
// void (__fastcall*)(const CCharString&)) @ 0x009B0470, CWideString::~CWideString @ 0x0099B510; the callback @ 0x00CD3EE0 is an empty
// function (`ret`); the literal at 0x012C2654 is L"script.bin". The vtable 0x012C2648 belongs to the no-RTTI script module.
// Shape: the file name comes from an inline helper returning the CWideString by value, so it is built in the one-word
// frame before the callback is pushed and its address re-derived for the call (a bare temporary pushes the trailing
// argument first); the temporary dies right after the base constructor, before the derived vtable store.
#include <stddef.h>
// Header debt: engine/CDefinitionManager.h declares no constructor, so the base is modelled locally as this constructor sees it.

class CCharString;
class CWideString { public:
    wchar_t* m_pData;
    CWideString(const wchar_t* text);          // 0x0099B6B0
    ~CWideString();                            // 0x0099B510
};

class CDefinitionManager {                     // CDefinitionManager as this constructor sees it
public:
    virtual void v00();
    CDefinitionManager(const CWideString& fileName, void (__fastcall* callback)(const CCharString&));   // 0x009B0470
};

static inline CWideString ScriptDefinitionFileName() { return CWideString(L"script.bin"); }

class CScriptDefinitionManager : public CDefinitionManager { public:   // vtable 0x012C2648
    virtual void v00();
protected:
    static void __fastcall ErrorCallback(const CCharString& message);   // 0x00CD3EE0 (empty; Ego_r ?ErrorCallback@CScriptDefinitionManager@@KIXABVCCharString@@@Z)
public:
    CScriptDefinitionManager();
};

CScriptDefinitionManager::CScriptDefinitionManager()
    : CDefinitionManager(ScriptDefinitionFileName(), &ErrorCallback)
{
}