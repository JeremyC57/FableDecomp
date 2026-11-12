// CTCCreatureModeManager::CModeBase::GetWorld() const -- the current thing manager's world: `mov eax,[0x013B8A1C] ; mov eax,[eax+24h] ; ret`
// Leaf: ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A680, 9 bytes.
// Naming (agentNF31, 2026-11-12): was the free function GetDisplayEngine() (CViewBase label; m_pDisplayEngine at +0x24) over a local
//   CThingManager model. Ego_r.pdb 0x00751030 (9 bytes / 3 instructions, egor_signatures) + FableWin.pdb 0x024C31E0
//   ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ; engine/CTCCreatureModeManager.h declares it at 0x0073A680
//   (FABLE_RETAIL_CMODEBASE_VIRTUALS) and the landed caller 0x008EACB0 binds it; its 40 retail call sites are all in CModeBase-derived
//   modes. Body: CThing::PCurrentThingManager (0x013B8A1C, inline static accessor) -> CThingManager::GetWorld (FableWin
//   ?GetWorld@CThingManager@@QAEAAVCWorld@@XZ, inline; World +0x24 Ego_r; header overlay opt-in GETWORLD).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_GETWORLD
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CTCCreatureModeManager.h"

CWorld& CTCCreatureModeManager::CModeBase::GetWorld() const
{
    return CThing::GetPCurrentThingManager()->GetWorld();
}
