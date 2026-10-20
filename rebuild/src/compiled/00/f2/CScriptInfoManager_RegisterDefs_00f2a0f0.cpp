// CScriptInfoManager::RegisterDefs() -- register the three script definition classes (CScriptDef, CCutsceneDef, CRegionScriptDef) with the global definition manager
// Leaf: RegisterDefs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F2A0F0, length 220 bytes (`ret`).
// PDB: ?RegisterDefs@CScriptInfoManager@@IAEXXZ.
// Callees: GFGetPDefinitionManager @ 0x00CD3EF0, CCharString(const char*, long) @ 0x0099EBF0, CCharString(const CCharString&)
// @ 0x0099EC30, CDefinitionManager::AddDefClass(const CDefClassInit&) @ 0x009B0AC0, ~CCharString @ 0x0099EAE0; the def
// allocators @ 0x00F29F40 (CScriptDef), 0x00F2A0D0 (CCutsceneDef), 0x00F29FA0 (CRegionScriptDef) (unlabelled in the
// manifest; named after the class they are registered for).
// Layout: Ego_r.pdb CDefClassInit (8 bytes): CCharString Name +0, CDefClassBase* (*AllocFunc)() +4.
// Shape: the manager is fetched once into esi; each registration is one full expression -- an IMPLICIT CCharString
// conversion of the literal (re-addressed with `lea` for the copy; an explicit temporary reuses eax), a CDefClassInit temporary copying it (copy constructor out of line, the allocator stored after it),
// AddDefClass by reference, then the two strings destroyed in reverse order of construction.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CDefClassBase;
struct CDefClassInit {
    CCharString     Name;                              // +0x00
    CDefClassBase* (*AllocFunc)();                     // +0x04
    CDefClassInit(const CCharString& name, CDefClassBase* (*allocFunc)()) : Name(name), AllocFunc(allocFunc) { }
};
class CDefinitionManager {
public:
    void AddDefClass(const CDefClassInit& init);       // 0x009B0AC0
};
CDefinitionManager* GFGetPDefinitionManager();         // 0x00CD3EF0

CDefClassBase* AllocCScriptDef();                      // 0x00F29F40
CDefClassBase* AllocCCutsceneDef();                    // 0x00F2A0D0
CDefClassBase* AllocCRegionScriptDef();                // 0x00F29FA0

class CScriptInfoManager {
public:
    void RegisterDefs();
};

void CScriptInfoManager::RegisterDefs()
{
    CDefinitionManager* manager = GFGetPDefinitionManager();
    manager->AddDefClass(CDefClassInit("CScriptDef", AllocCScriptDef));
    manager->AddDefClass(CDefClassInit("CCutsceneDef", AllocCCutsceneDef));
    manager->AddDefClass(CDefClassInit("CRegionScriptDef", AllocCRegionScriptDef));
}