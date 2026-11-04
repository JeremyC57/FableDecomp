// CAIStateGroupBase::PeekDefinitionManager() const -- the game definition manager, through the brain's main game component
// Leaf: ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCFB0, 11 bytes: `mov eax,[ecx+4] ; mov ecx,[eax+14h] ; jmp 0x00415FBC`
//   (PBrain->Component->PeekDefinitionManager(), tail call).
// Identity (two sources): Ego_r.pdb ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ (protected const; donor
//   0x00847E00 right before ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ 0x00847E20, as retail 0x008FCFB0 before the landed 0x008FCFD0
//   which reads the same PBrain->Component word); the tail target is the only spelling bound at 0x00415FBC,
//   ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ. Its 11 callers are CAIStateGroup_* Update / action
//   bodies. The catalog's "GetPDefinitionManager" is in neither PDB.
// Callees: ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ 0x00415FBC.
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _PEEKDEFINITIONMANAGER new), engine/CAIBrain.h (Component
//   +0x14), engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
const CGameDefinitionManager& CAIStateGroupBase::PeekDefinitionManager() const
{
    return PBrain->Component->PeekDefinitionManager();
}