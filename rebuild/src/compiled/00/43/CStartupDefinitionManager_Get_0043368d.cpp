// CStartupDefinitionManager::Get() -- the startup definition manager singleton: `mov eax,[0x013B8760] ; ret` (static __fastcall, no `this`)
// Leaf: ?Get@CStartupDefinitionManager@@SIAAV1@XZ
// Identity (naming wave 11): ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r.pdb 0x00441A6D + FableWin.pdb); was the free extern "C" GetGlobal.
// Naming (agentNF31, 2026-11-12): the pointer at 0x013B8760 is the protected static CStartupDefinitionManager::PClass
//   (?PClass@CStartupDefinitionManager@@1V?$CCountedPointer@VCStartupDefinitionManager@@@@A: FableWin.pdb `dynamic initializer` /
//   `dynamic atexit destructor for 'CStartupDefinitionManager::PClass'`, Anniversary Test PDB publics), bound through the shared
//   engine/CStartupDefinitionManager.h (overlay opt-in PCLASS) in place of the stand-in `int g_value_013b8760` and the local class.
//   Get returns *PClass (CCountedPointer::operator*, Data at +0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#include "fable_string.h"
#include "engine/CStartupDefinitionManager.h"

CStartupDefinitionManager& __fastcall CStartupDefinitionManager::Get()
{
    return *PClass;
}
