// CWorld::Init() -- first-stage world set-up with loading-screen progress: create the world map, game clock, environment, navigation manager (A* and flyer navigator classes), factions, global console, combat manager, thing manager, message events, search tools, camera managers, bullet time, opinion reactions, script conversations, mesh bank, particle / animation-event / UI wiring, speech gain and the skeletal morph cache
// Leaf: ?Init@CWorld@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A6E30, 2259 bytes, plain `ret` (CWorld vtable slot 9; Ego_r.pdb / FableWin.pdb
//   ?Init@CWorld@@UAEXXZ). No EH frame. The TU is /Gr (every free function and free template is __fastcall: StartProfile /
//   DisplayProgress take the string in ecx, boost::checked_delete<T> takes the pointer in ecx, 0x004A6F4B).
// Shape: one block per subsystem, the PostInit 0x004A6550 pattern: NProfileTimer::StartProfile(name, 0) and/or
//   NProgressDisplay::DisplayProgress(name, -1.0f, false, false), then `P<Member>.reset(new T(...))` on the Ego_r.pdb
//   boost::scoped_ptr / CCountedPointer members (CWorld.h opt-in FABLE_RETAIL_CWORLD_TYPED_MEMBERS). scoped_ptr::reset is Boost 1.30's
//   `scoped_ptr(p).swap(*this)`, so the old pointer is deleted after the store: polymorphic managers through the deleting dtor in
//   place, CFactionManager / CThingSearchTools through their implicit destructors in place, CGameTimeManager / CBulletTimeManager /
//   CScriptConversationManager through the out-of-line checked_delete<T> copies 0x004ADD30 / 0x004AE080 / 0x004AE810 (their
//   implicit destructors expand a map). CCountedPointer<T>::Reset is out of line per T (0x004ADF80, 0x004ADAE0, 0x004A9A80).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CGAMETIMEMANAGER_IMPLICIT_DTOR
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_CTOR
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_VIRTUALS
#define FABLE_RETAIL_CFACTIONMANAGER_CTOR
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_CTOR
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_IMPLICIT_DTOR
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CTOR
#define FABLE_RETAIL_CGAMECAMERABASE_RENDER
#define FABLE_RETAIL_CGAMECAMERABASE_CTOR
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CBULLETTIMEMANAGER_CTOR
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_CTOR
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CCREATUREACTIONBASE_STATREGISTER
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "fable_profile.h"
#include "engine/NProgressDisplay.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/C2DBoxI.h"
#include "engine/CWorldMap.h"
#include "engine/CGameTimeManager.h"
#include "engine/CEnvironment.h"
#include "engine/CGameNavigatorManagerExternalInterface.h"
#include "engine/CNavigatorManager.h"
#include "engine/CClassTypeInit.h"
#include "engine/CFactionManager.h"
#include "engine/CCombatManager.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameCameraBase.h"
#include "engine/CBulletTimeManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CScriptConversationManager.h"
#include "engine/CSkeletalMorphResourceManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSystemManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NSpeechGainManager.h"

class CGraphicDataBank;
class CMeshDataBank;

namespace NGlobalConsole { void Initialise(); }                                       // 0x00419D90 ?Initialise@NGlobalConsole@@YIXXZ
namespace NParticleEngine {
void SetMeshBank(const CMeshDataBank& mesh_bank);                                       // 0x00AEAA90 ?SetMeshBank@NParticleEngine@@YIXABVCMeshDataBank@@@Z
void SetGraphicBank(const CGraphicDataBank& graphic_bank);                              // 0x00AEAA80 ?SetGraphicBank@NParticleEngine@@YIXABVCGraphicDataBank@@@Z
}
namespace NAnimationEvents {
void InitialiseManagers(const CMeshDataBank& mesh_bank);                                // 0x006FAA90 ?InitialiseManagers@NAnimationEvents@@YIXABVCMeshDataBank@@@Z
void LoadAnimationEvents(bool b);                                                       // 0x006FABF0 ?LoadAnimationEvents@NAnimationEvents@@YIX_N@Z
}
extern long g_NavigatorTypeDefault;                                                     // 0x0129CBA4 (A* navigator class type, 3)
extern long g_NavigatorTypeHover;                                                       // 0x0129CB44 (flyer navigator class type, 4)

void CWorld::Init()
{
    NProfileTimer::StartProfile("Init World Map", 0);
    NProgressDisplay::DisplayProgress("Init World Map", -1.0f, false, false);
    PWorldMap.reset(new CWorldMap(*Component, *this, C2DBoxI(0, 0, 0x2000, 0x2000), 5));

    NProfileTimer::StartProfile("Init Environment", 0);
    NProgressDisplay::DisplayProgress("Init Environment", -1.0f, false, false);
    PGameTimeManager.reset(new CGameTimeManager(*Component, *this));
    PEnvironment.reset(new CEnvironment(*DefinitionManager));

    NProfileTimer::StartProfile("Init Navigation Manager", 0);
    NProgressDisplay::DisplayProgress("Init Navigation Manager", -1.0f, false, false);
    PNavigationManager.reset(new CNavigatorManager(4, Component->GetWorldSeed(), CGameNavigatorManagerExternalInterface(*this)));
    PNavigationManager->AddNewClassType(CClassTypeInit(g_NavigatorTypeDefault, 0, false, "Navigator A Star"));
    PNavigationManager->AddNewClassType(CClassTypeInit(g_NavigatorTypeHover, 0, false, "Navigator flyer"));

    PFactionManager.reset(new CFactionManager(*this));

    NProgressDisplay::DisplayProgress("Init Global Console", -1.0f, false, false);
    NGlobalConsole::Initialise();
    NProgressDisplay::DisplayProgress("Adding Console Commands", -1.0f, false, false);
    ConsolePauseAtFrameNumber = 0;

    NProfileTimer::StartProfile("Init Combat Manager", 0);
    NProgressDisplay::DisplayProgress("Init Combat Manager", -1.0f, false, false);
    PThingCombatManager.reset(new CCombatManager(*this));
    PThingCombatManager->Init();

    NProfileTimer::StartProfile("Init Thing Manager", 0);
    NProgressDisplay::DisplayProgress("Init Thing Manager", -1.0f, false, false);
    InitThingManager();

    NProgressDisplay::DisplayProgress("Init Event Manager", -1.0f, false, false);
    PMessageEventManager.Reset(new CMessageEventManager(*this));

    NProgressDisplay::DisplayProgress("Init Search Tools", -1.0f, false, false);
    PThingSearchTools.reset(new CThingSearchTools(*PThingManager, *PWorldMap));

    NProfileTimer::StartProfile("Init Game Camera Manager", 0);
    NProgressDisplay::DisplayProgress("Init Game Camera Manager", -1.0f, false, false);
    PGameCameraManager.reset(new CGameCameraManager(*this));

    NProgressDisplay::DisplayProgress("Init Bullet Time Manager", -1.0f, false, false);
    PBulletTimeManager.reset(new CBulletTimeManager(*this, *PlayerManager));

    NProgressDisplay::DisplayProgress("Init Opinion Reaction Manager", -1.0f, false, false);
    POpinionReactionManager.Reset(new COpinionReactionManager(*this));

    NProgressDisplay::DisplayProgress("Init Script Conversation Manager", -1.0f, false, false);
    PScriptConversationManager.reset(new CScriptConversationManager);

    NProgressDisplay::DisplayProgress("Init Game Camera", -1.0f, false, false);
    PGameCamera.reset(new CGameCameraBase(*this));
    PCurrentGameCamera = PGameCameraManager.get();

    NProfileTimer::StartProfile("Init Mesh Bank Manager", 0);
    NProgressDisplay::DisplayProgress("Init Mesh Bank", -1.0f, false, false);
    InitMeshBank();

    NProgressDisplay::DisplayProgress("Setting Particle Engine Mesh Bank", -1.0f, false, false);
    NParticleEngine::SetMeshBank(*PMainMeshBank);
    NProgressDisplay::DisplayProgress("Setting Particle Engine Graphic Bank", -1.0f, false, false);
    NParticleEngine::SetGraphicBank(Component->PeekEngineGraphics());

    NProgressDisplay::DisplayProgress("Init Animation Event Managers", -1.0f, false, false);
    NAnimationEvents::InitialiseManagers(*PMainMeshBank);
    NProfileTimer::StartProfile("Init Animation Events", 0);
    NProgressDisplay::DisplayProgress("Init Animation Events", -1.0f, false, false);
    NAnimationEvents::LoadAnimationEvents(true);
    CCreatureActionBase::StatRegisterClass();

    NProfileTimer::StartProfile("Init UI Manager", 0);
    NProgressDisplay::DisplayProgress("Init UI Manager", -1.0f, false, false);
    NUISystem::CManager::GetInstance()->SetMeshBank(PMainMeshBank);
    NUISystem::CManager::GetInstance()->SetInputManager(GFGetSystemManager()->GetInputManager());
    NUISystem::CManager::GetInstance()->SetInput(NUISystem::CManager::UI_INPUT_XBOX);

    NProgressDisplay::DisplayProgress("Init Speech Gain Manager", -1.0f, false, false);
    NSpeechGainManager::Initialise(*this);

    CSkeletalMorphResourceManager::Get().PreloadBoneConfigs();
}