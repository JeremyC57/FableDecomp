// CPlayerGui::PeekGlobalDef() const -- look up the "GLOBAL" definition and return it without keeping a reference (a reference to the GLOBAL def)
// Leaf: ?PeekGlobalDef@CPlayerGui@@QBEABVCGlobalDef@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004384F0, length 123 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0
// (`__fastcall`, static), CDefinitionManager::GetPDefFromClassIndex @ 0x009ADE10 (a CDefPointer by value through the hidden
// pointer), ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// String 0x01231734 "GLOBAL".
// Shape: the result counted pointer starts null (esi); on a hit it takes a reference, the lookup temporary is
// released by hand and nulled; the name dies, then the result's reference is dropped and its raw pointer returned.
// The manager reference is a named local (Get() runs before the GetDef pushes); the hand release calls the def's own
// inline Release() member (`if (--m_count == 0) Destroy();`), which gives retail's `mov ecx,eax ; add eax,4 ; dec edx`
// form (the object copied to ecx for the virtual call, the count addressed through eax).
// Name (round 7, NF53): the self is Ego_r.pdb ?PeekGlobalDef@CPlayerGui@@QBEABVCGlobalDef@@XZ (a const member returning the GLOBAL
//   def by reference; the old leaf was a static returning a local CDefBase*); the callee 0x009ADE10 is
//   ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z (Ego_r.pdb 0x00B05EA0 /
//   FableWin.pdb 0x03061E20), reached through the inline by-class-index GetDef<CGlobalDef> (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_INLINE,
//   index 1). Was a local CCharString / CDefBase / CDefPointer / CDefinitionManager / CGameDefinitionManager / CPlayerGui model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_INLINE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CPLAYERGUI_PEEKGLOBALDEF
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CPlayerGui.h"

const CGlobalDef& CPlayerGui::PeekGlobalDef() const
{
    CDefPointer<const CGlobalDef> global_def;
    {
        CCharString name("GLOBAL");
        const CDefinitionManager& manager = CGameDefinitionManager::Get();
        manager.GetDef(name, 1, &global_def);
    }
    return *global_def;
}
