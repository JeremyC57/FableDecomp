// CTCInventoryExperience::CloseInventory() -- close the experience screen: 3D scene rendering back on and its start frame stamped, the sound group volumes restored and the tutorial sounds stopped, the exit sound played, the screen closed (CloseInventoryInternal), the cutscene preload cleared, the close message posted and the open-menu count dropped
// Leaf: ?CloseInventory@CTCInventoryExperience@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056F30B, 396 bytes, thiscall, plain `ret`; size TU with a frame pointer
//   (`#pragma optimize("y", off)` + `("s", on)`, as the TU-mate OpenInventory 0x0056FAD7).
//   Identity: Ego_r.pdb / FableWin.pdb ?CloseInventory@CTCInventoryExperience@@UAEXXZ; vtable 0x0124F11C slot 31 (+0x7C) = 0x0056F30B.
// Callees: CIEngine::Enable3DSceneRendering (slot 76) / CutscenePreload (slot 56) on CMainGameComponent::Get()->GetDisplayEngine()
//   .Get3DEngine() (PDisplayEngine +0x28, Engine3D +0x2C); CTCBase::GetWorld 0x00686A70; CWorld::SetAsStarting3DRendering (inline:
//   CWorld::GetFrame 0x0049D870 into +0x8C); CUserProfileManager::Get 0x0040D2A0 + inline GetSoundVolume; PSoundManager 0x013B8394
//   SetGainFader (slot 46) for groups 0x10 and 0x80 and Get2DEmitter (slot 9) -> CASoundEmitter::StopSpecificSample (its slot 9)
//   per tutorial sound handle (slot names: engine/CASoundManager.h, engine/CASoundEmitter.h) (STLport map
//   increment 0x004292C0); NUISystem::CManager::GetInstance 0x0041E5F2 (twice), GetMiscThingsDef (slot 42, def pointer by hidden
//   result) and PlaySoundW (slot 40) with SoundExit +0x1A8; CloseInventoryInternal (this slot 89); CWorld::GetMessageEventManager
//   (inline, +0x60) and CMessageEventManager::AddMessage 0x00687540 (type 0x4A, 10 seconds of frames from
//   CMainGameComponent::GetConstantFPS 0x01375550); NUISystem::CManager::OnCloseMenus 0x0041D165.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CWorld.h"
#include "engine/C3DVector.h"
#include "engine/CUserProfileManager.h"
#include "engine/CASoundManager.h"

enum EMessageEventType { };
class CMessageEvent;
class CMessageEventManager { public:                       // no shared header (as the landed 0x0096DBC0)
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long flags, const C3DVector& pos, CThing* thing1, CThing* thing2,
                              float a, float b, long priority);   // 0x00687540 Ego_r ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z
};

void CTCInventoryExperience::CloseInventory()
{
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    GetWorld().SetAsStarting3DRendering();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        for (std::map<CCharString, unsigned long>::iterator it = TutorialSoundHandles.begin(); it != TutorialSoundHandles.end(); ++it)
            PSoundManager->Get2DEmitter().StopSpecificSample(it->second);
    }
    NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
    CloseInventoryInternal(true);
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
    C3DVector origin(0.0f, 0.0f, 0.0f);
    GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)0x4A, CMainGameComponent::GetConstantFPS() * 10, origin, 0, 0,
                                                   0.0f, 0.0f, 0);
    NUISystem::CManager::OnCloseMenus();
}