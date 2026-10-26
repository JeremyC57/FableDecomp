// CScriptInfoManager::Init() -- install the script definition manager, register the script defs and load them, register every script class and sort the script table
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5D80, length 182 bytes (`ret`).
// PDB: ?Init@CScriptInfoManager@@QAEXXZ.
// Callees: operator new @ 0x00BFEA1A (0xd0 bytes) + CScriptDefinitionManager::CScriptDefinitionManager @ 0x00CD3F00 (vtable
// 0x012C2648), GFSetPDefinitionManager @ 0x00CD3F40 (`__fastcall`: resets the global CCountedPointer @ 0x0143E920; manifest
// "operator_new" is propagated), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40 (`__fastcall`, the bool in dl),
// CScriptInfoManager::RegisterDefs @ 0x00F2A0F0, GFGetPDefinitionManager @ 0x00CD3EF0 + its virtual slot 2 (0x00CD3F50,
// loads the registered definitions), CScriptInfoManager::RegisterScripts @ 0x00CD52D0 (the quest-script allocator table),
// the SGI sort of the script table @ 0x00CB7780 (landed as Sort_SElement24_00cb7780; `__fastcall`, comparator by value).
// Layout: Ego_r.pdb CScriptInfoManager with retail's 16-byte CVectorMap: CurrentScriptID +0, Scripts +4 (vector +4..+0xc,
// comparator +0x10, Dirty +0x11), CodeSectionNames +0x14.
// Shape: the progress texts are implicit CCharString conversions (built before the trailing constants, in the one
// `push ecx` frame word); the table sort is CVectorMap's inline "sort if dirty".
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

class CDefinitionManager {
public:
    virtual void v00(); virtual void v01();
    virtual void LoadDefinitions();                    // slot 2 (+0x08) 0x00CD3F50 in the script manager
};
class CScriptDefinitionManager : public CDefinitionManager {   // vtable 0x012C2648, 0xd0 bytes
public:
    CScriptDefinitionManager();                        // 0x00CD3F00
    unsigned char _body[0xcc];
};
void __fastcall GFSetPDefinitionManager(CDefinitionManager* manager);   // 0x00CD3F40
CDefinitionManager* GFGetPDefinitionManager();                          // 0x00CD3EF0 (no arguments; landed callers declare it plain)

struct SScriptEntry { unsigned char _body[0x18]; };    // pair<CCharString, CScriptInfo>
struct CKeyPairCompareLess { };                        // empty comparator, passed by value
void __fastcall Sort_SElement24_00cb7780(SScriptEntry* first, SScriptEntry* last, CKeyPairCompareLess comp);   // 0x00CB7780

struct CScriptMap {                                    // CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<..>>
    SScriptEntry*       _M_start;                      // +0x00
    SScriptEntry*       _M_finish;                     // +0x04
    SScriptEntry*       _M_end_of_storage;             // +0x08
    CKeyPairCompareLess Compare;                       // +0x0c
    bool                Dirty;                         // +0x0d
    void Sort()
    {
        if (Dirty) {
            Sort_SElement24_00cb7780(_M_start, _M_finish, Compare);
            Dirty = false;
        }
    }
};

class CScriptInfoManager {
public:
    long          CurrentScriptID;                     // +0x00
    CScriptMap    Scripts;                             // +0x04
    unsigned char CodeSectionNames[0x0c];              // +0x14 vector<CCharString>
    void Init();
    void RegisterDefs();                               // 0x00F2A0F0
    void RegisterScripts();                            // 0x00CD52D0
};

void CScriptInfoManager::Init()
{
    GFSetPDefinitionManager(new CScriptDefinitionManager());
    NProgressDisplay::DisplayProgress("Registering Script Defs", -1.0f, false, false);
    RegisterDefs();
    GFGetPDefinitionManager()->LoadDefinitions();
    NProgressDisplay::DisplayProgress("Registering Scripts", -1.0f, false, false);
    RegisterScripts();
    Scripts.Sort();
}