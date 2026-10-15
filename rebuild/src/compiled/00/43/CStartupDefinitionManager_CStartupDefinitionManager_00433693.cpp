// CStartupDefinitionManager::CStartupDefinitionManager(const CWideString&) -- builds the CDefinitionManager base over the given definitions file with the startup manager's (empty) error callback, then installs the derived vtable
// Leaf: ??0CStartupDefinitionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00433693, 29 bytes. Callees: CDefinitionManager::CDefinitionManager
// (CWideString*, void (*)(CCharString*)) @ 0x009B0470; the error callback pushed as an immediate is the empty
// `ret` function @ 0x0043368C. Derived vtable 0x0123117C.
// Layout: engine/CStartupDefinitionManager.h is the PDB's flattened 0xF0-byte view (`void* __vftable` + base bytes),
// which cannot emit a vtable store, so this file models the polymorphic CDefinitionManager base locally (size 0xF0).
// Shape: the file argument is forwarded straight from its slot (`push dword ptr [esp+0Ch]`) after the callback
// immediate (right-to-left), the base constructor runs on `this`, then the derived vtable store. The pass-through push
// marks a size-optimised TU: #pragma optimize("s", on).
#pragma optimize("s", on)
class CWideString;
class CCharString;

class CDefinitionManager {
public:
    typedef void (*TErrorCallback)(CCharString* message);
    CDefinitionManager(const CWideString& definitionsFile, TErrorCallback errorCallback);   // 0x009B0470
    virtual ~CDefinitionManager();
    unsigned char _body[0xEC];
};

class CStartupDefinitionManager : public CDefinitionManager {
public:
    static void StartupErrorCallback(CCharString* message);    // 0x0043368C (empty)
    CStartupDefinitionManager(const CWideString& definitionsFile);
    virtual ~CStartupDefinitionManager();
};

CStartupDefinitionManager::CStartupDefinitionManager(const CWideString& definitionsFile)
    : CDefinitionManager(definitionsFile, StartupErrorCallback)
{
}