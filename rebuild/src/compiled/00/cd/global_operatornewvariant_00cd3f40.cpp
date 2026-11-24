// CScriptDefinitionManager::Set(CScriptDefinitionManager* manager) -- install the script definition manager: Reset the static counted pointer PClass to it
// Leaf: ?Set@CScriptDefinitionManager@@SIXPAV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD3F40, 12 bytes, __fastcall (manager in ecx): `push ecx; mov ecx, offset PClass 0x0143E920; call Reset 0x00CD43A0; ret`.
// Names (NF371, 2026-11-24; was ?operator_new_variant@@YIPAXI@Z calling a made-up ?Allocate@CFixedAllocator@@QAEPAXI@Z on 0x0143E920, catalog
//   _global::operator_new_variant): FableWin.pdb + Ego_r.pdb ?Set@CScriptDefinitionManager@@SIXPAV1@@Z (Ego_r script_definition_manager.obj Set is
//   12 bytes, as retail), declared by engine/CScriptDefinitionManager.h; the callee 0x00CD43A0 stores DeleteData<CScriptDefinitionManager>
//   0x00CD42E0, so it is FableWin.pdb ?Reset@?$CCountedPointer@VCScriptDefinitionManager@@@@QAEXPAVCScriptDefinitionManager@@@Z; the only caller is
//   CScriptInfoManager::Init 0x00CB5D80 with a fresh CScriptDefinitionManager.
// PClass: FableWin.pdb + Ego_r.pdb name `dynamic initializer / atexit destructor for 'CScriptDefinitionManager::PClass'`
//   (script_definition_manager.obj; retail atexit destructor 0x0122AEB0); no data-symbol spelling is on the lane, so the static stays at its
//   address, as the landed row modelled it (no relocation either way). Reset is declaration only here
//   (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL): retail calls it out of line.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"

#include "fable_string.h"
#include "engine/CScriptDefinitionManager.h"

#define CScriptDefinitionManager_PClass (*(CCountedPointer<CScriptDefinitionManager>*)0x0143e920)   // static CScriptDefinitionManager::PClass

void __fastcall CScriptDefinitionManager::Set(CScriptDefinitionManager* manager)
{
    CScriptDefinitionManager_PClass.Reset(manager);
}
