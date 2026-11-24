// CScriptInfoManager::Init() -- install the script definition manager, register the script defs and load them, register every script class and sort the script table
// Leaf: ?Init@CScriptInfoManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5D80, length 182 bytes (`ret`).
// PDB: ?Init@CScriptInfoManager@@QAEXXZ.
// Callees: operator new @ 0x00BFEA1A (0xd0 bytes) + CScriptDefinitionManager::CScriptDefinitionManager @ 0x00CD3F00 (vtable
// 0x012C2648), CScriptDefinitionManager::Set @ 0x00CD3F40 (static `__fastcall`: resets the static CCountedPointer PClass @ 0x0143E920; manifest
// "operator_new" is propagated), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40 (`__fastcall`, the bool in dl),
// CScriptInfoManager::RegisterDefs @ 0x00F2A0F0, GFGetPDefinitionManager @ 0x00CD3EF0 + its virtual slot 2 (0x00CD3F50,
// loads the registered definitions), CScriptInfoManager::RegisterScripts @ 0x00CD52D0 (the quest-script allocator table),
// the SGI sort of the script table @ 0x00CB7780 (landed as Sort_SElement24_00cb7780; `__fastcall`, comparator by value).
// Layout: Ego_r.pdb CScriptInfoManager with retail's 16-byte CVectorMap: CurrentScriptID +0, Scripts +4 (vector +4..+0xc,
// comparator +0x10, Dirty +0x11), CodeSectionNames +0x14.
// Shape: the progress texts are implicit CCharString conversions (built before the trailing constants, in the one
// `push ecx` frame word); the table sort is CVectorMap's inline "sort if dirty".
// Naming (NF180, 2026-11-20, q4221): the local CCharString / CScriptInfoManager / CScriptMap (SScriptEntry blobs, an empty local
//   CKeyPairCompareLess, the Sort_SElement24_00cb7780 stand-in) replaced by the shared fable_string.h, engine/CScriptInfoManager.h,
//   engine/CVectorMap.h (FABLE_RETAIL_CVECTORMAP_SORT: Update = sort if Dirty, expanded) and engine/NProgressDisplay.h, so the table sort
//   binds the STLport std::sort<pair<CCharString, CScriptInfo>*, CKeyPairCompareLess<..> > spelling its self row 0x00CB7780 defines.
//   The definition-manager declarations are the landed row's, unchanged.
// Naming (NF371, 2026-11-24): the install call binds FableWin.pdb + Ego_r.pdb ?Set@CScriptDefinitionManager@@SIXPAV1@@Z (12-byte self row
//   0x00CD3F40, respelled in the same landing; was the made-up free ?GFSetPDefinitionManager@@YIXPAVCDefinitionManager@@@Z), declared as the
//   PDB static member in the local model; the argument is the same pointer (the base is at offset 0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/NProgressDisplay.h"
#include "engine/CScriptInfoManager.h"

void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

class CDefinitionManager {
public:
    virtual void v00(); virtual void v01();
    virtual void LoadDefinitions();                    // slot 2 (+0x08) 0x00CD3F50 in the script manager
};
class CScriptDefinitionManager : public CDefinitionManager {   // vtable 0x012C2648, 0xd0 bytes
public:
    CScriptDefinitionManager();                        // 0x00CD3F00
    static void __fastcall Set(CScriptDefinitionManager* manager);   // 0x00CD3F40
    unsigned char _body[0xcc];
};
CDefinitionManager* GFGetPDefinitionManager();                          // 0x00CD3EF0 (no arguments; landed callers declare it plain)

void CScriptInfoManager::Init()
{
    CScriptDefinitionManager::Set(new CScriptDefinitionManager());
    NProgressDisplay::DisplayProgress("Registering Script Defs", -1.0f, false, false);
    RegisterDefs();
    GFGetPDefinitionManager()->LoadDefinitions();
    NProgressDisplay::DisplayProgress("Registering Scripts", -1.0f, false, false);
    RegisterScripts();
    Scripts.Update();
}