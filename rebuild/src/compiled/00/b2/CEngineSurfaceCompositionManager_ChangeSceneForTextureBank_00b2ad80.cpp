// CEngineSurfaceCompositionManager::ChangeSceneForTextureBank() -- move the completed composite-texture bank to the current scene
// Leaf: ?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2AD80, 11 bytes: `add ecx,398h ; jmp 0x009FD0C0` (CompletedBankFile.ChangeScene(), tail call).
// Identity (two sources): (1) Ego_r.pdb names exactly one public `void ()` member of the class, ?ChangeSceneForTextureBank@
//   CEngineSurfaceCompositionManager@@QAEXXZ, and its members IntermediateBankFile / CompletedBankFile / CustomTextureBankFile are
//   CGraphicDataBank (retail 0x30C each from +0x8C, so +0x398 is CompletedBankFile); the target is the landed
//   ?ChangeScene@CGraphicDataBank@@QAEXXZ 0x009FD0C0; (2) the only caller, CEngineResourceManager::ResizeHiresTextureMemory
//   0x00B3B4F0, first runs ChangeScene on its own main graphic bank (+0x954) and then calls this on the surface manager: the scene
//   change for the texture bank. The landed caller spells it ?FlushCache@..@@QAEXXZ (NAME_FIXES.tsv); catalog "_Cons_val<..>".
// Callees: ?ChangeScene@CGraphicDataBank@@QAEXXZ 0x009FD0C0.
// Header: engine/CEngineSurfaceCompositionManager.h (FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER + _BANKS new),
//   engine/CGraphicDataBank.h (new), engine/CResourceBank.h, engine/CEngineComponent.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_BANKS
#include "fable_string.h"
#include "engine/CEngineSurfaceCompositionManager.h"
void CEngineSurfaceCompositionManager::ChangeSceneForTextureBank()
{
    CompletedBankFile.ChangeScene();
}