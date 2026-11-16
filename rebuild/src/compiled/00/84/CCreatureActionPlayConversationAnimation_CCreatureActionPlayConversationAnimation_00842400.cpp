// CCreatureAction_PlayConversationAnimation::CCreatureAction_PlayConversationAnimation(CThingCreatureBase& creature, EConversationAttitude attitude,
// const CSoundPair& sound, long p4, long priority) -- base CCreatureActionBase(creature, -1.0, 1.0, a random loaded animation of the attitude's
// animation name, sound, p4, priority), own vtable (0x012737CC), UseAnimationMovement (+0xA8) cleared
// Leaf: ??0CCreatureAction_PlayConversationAnimation@@QAE@AAVCThingCreatureBase@@W4EConversationAttitude@@ABVCSoundPair@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842400, length 81 bytes, `ret 14h`.
// Callees: CConversationAttitude::GetAnimName(EConversationAttitude) @ 0x004CD9B0 (static __fastcall, landed), CThingCreatureBase::
//   GetRandomLoadedAnimFromName(const CCharString&, bool) const @ 0x00662FA0, CCreatureActionBase::CCreatureActionBase(...) @ 0x00693B30.
// Shape: arguments right to left -- priority, p4, the sound reference, then the animation (its own `true` pushed before the name lookup), 1.0f,
//   -1.0f, the creature.
// NF39 (2026-11-16): moved off its local CCreatureActionBase / CThingCreatureBase / CSoundPair / class models (pointer + long arguments, the callee
//   0x004CD9B0 as a free `GetAnimGroupName_004cd9b0`) onto the shared engine/CCreatureAction_PlayConversationAnimation.h (opt-in
//   FABLE_RETAIL_CCREATUREACTION_PLAYCONVERSATIONANIMATION, which names 0x00842400), engine/CCreatureActionBase.h (ANIMCTOR),
//   engine/CThingCreatureBase.h (ANIMQUERY) and engine/CSoundPair.h. Evidence: Ego_r.pdb + FableWin.pdb
//   ??0CCreatureAction_PlayConversationAnimation@@QAE@AAVCThingCreatureBase@@W4EConversationAttitude@@ABVCSoundPair@@JJ@Z; the landed caller
//   CActionTalkBase::Update 0x00716DE0 already binds it. CConversationAttitude has no shared header: its static GetAnimName is declared here with
//   the landed 0x004CD9B0 spelling (Ego_r.pdb donor run GetAnimName(EAttitude) / GetAnimName(EConversationAttitude) / GetAnimGroupName).
// Defines: the landed 0x00716DE0 CActionTalkBase::Update preamble + ANIMCTOR + ANIMQUERY.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_CONVERSATION
#define FABLE_RETAIL_CTCTALK_SPEECHSAMPLEPLAYING
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_CTCBASE_TALKACTION_FRIEND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDMODE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTION_PLAYCONVERSATIONANIMATION
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMDURATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CSUBTITLERENDERER_CLICKPAST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCTalk.h"
#include "engine/CTCLook.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CMainGameComponent.h"
#include "engine/CVectorMap.h"
#include "engine/CArray.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CAnimationEntry.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimationWithLookTurning.h"
#include "engine/CCreatureAction_PlayConversationAnimation.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CPlayerGui.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFGetTime.h"

class CConversationAttitude {
public:
    static const CCharString& __fastcall GetAnimName(EConversationAttitude attitude);   // 0x004CD9B0
};

CCreatureAction_PlayConversationAnimation::CCreatureAction_PlayConversationAnimation(CThingCreatureBase& creature, EConversationAttitude attitude,
    const CSoundPair& sound, long p4, long priority)
    : CCreatureActionBase(creature, -1.0f, 1.0f, *creature.GetRandomLoadedAnimFromName(CConversationAttitude::GetAnimName(attitude), true), sound, p4, priority)
    , UseAnimationMovement(false)
{
}
