// CScriptDefinitionManager::~CScriptDefinitionManager() -- own vtable store, ResetGlobalScriptDef(), then the base destructor
// Leaf: ??1CScriptDefinitionManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3EC0, length 22 bytes (tail-jumps to CDefinitionManager::~CDefinitionManager).
// Identity: stores vtable 0x012C2648, the vtable the landed CScriptDefinitionManager::CScriptDefinitionManager 0x00CD3F00 stores
//   (slot 0 = the landed ??_GCScriptDefinitionManager 0x00CD4330, which inlines this same body; slot 2 = InitAndCompile 0x00CD3F50);
//   Ego_r.pdb / FableWin.pdb ??1CScriptDefinitionManager@@UAE@XZ, first in the class's donor run (dtor, ErrorCallback, ctor,
//   InitAndCompile, ??_G) exactly as retail 0x00CD3EC0 / 0x00CD3EE0 / 0x00CD3F00 / 0x00CD3F50 / 0x00CD4330.
// Callees: ResetGlobalScriptDef 0x00CBEEB0 (PDB ?ResetGlobalScriptDef@@YIXXZ), CDefinitionManager::~CDefinitionManager 0x009B02E0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CScriptDefinitionManager.h"

void __fastcall ResetGlobalScriptDef();                 // 0x00CBEEB0

CScriptDefinitionManager::~CScriptDefinitionManager()
{
    ResetGlobalScriptDef();
}
