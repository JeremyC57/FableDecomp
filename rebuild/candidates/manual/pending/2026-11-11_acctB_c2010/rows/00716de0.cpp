// NTCScriptedControl::CActionTalkBase::Update() -- per-frame conversation update: make the speaker and the listener look at each other, start the line's timer once its speech is playing, end the line on a skip or when its time has run out (then the next line or the end of the conversation), and while it runs play the line's animation and attitude tags, random conversation gestures, and its camera tags (or the default conversation camera)
// Leaf: ?Update@CActionTalkBase@NTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00716DE0, 1514 bytes, thiscall, `ret`; ebp frame (`lea ebp,[esp-78h]`: the inline-asm ceiling).
//   Starts right after SetUpTextAndSpeech's int3 padding (0x00716B30 + 681 + 7); ends `ret` at 0x007173C9.
// Retail-asm: 0x007170DC `fistp dword ptr [ebp+74h]` and 0x00717189 `fistp dword ptr [ebp+74h]` -- the engine's inline-assembly
//   GFFloatToLongCeil (engine/GFFloatToLongCeil.h: `fld ; fadd half ; fistp ; fild ; fsub one ; fstp ; cmp ; jne ; dec`) expanded
//   twice, as in the landed sibling 0x00716B30 SetUpTextAndSpeech (its C++ attempt table: GOTCHAS rasm1 / ASM_RULES; `(long)x`
//   gives __ftol2, /QIfist `fistp qword`); the row reaches it only through the shared helper header.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Update@CActionTalkBase@NTCScriptedControl@@UAEXXZ (donor 0x0072B280 right after
//   SetUpTextAndSpeech 0x0072AFB0, as retail 0x00716DE0 follows 0x00716B30); called by CActionTalkToThing::Update 0x007173D0
//   (`call 0x00716DE0` twice) and CActionOverheadTalkToThing::Update 0x00717450; the member offsets are the landed sibling's.
// Callees: CScriptThing slots 11 GetPThing / 71 UpdateThingAttachment / 75 IsAlive / 77 IsNull (the target); CVectorMap LowerBound
//   0x0040F020 (GetTC TCI_LOOK 67, TCI_TALK 15); NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) 0x004C1060 (CThing
//   +0x64); CTCLook::IsOverriddenByParent 0x00751A70 / LookAtPos 0x00753E90 / IsPosInViewRange 0x007534B0; CTCTalk::
//   IsSpeechSamplePlaying 0x006E2D30 / IsSpeechSampleLoadedAndPlaying 0x006E2DC0 / StopCurrentSpeech 0x006E2E60 / inline SetAttitude;
//   GFGetTime 0x009E1BC0; the CPlayerGui singleton 0x013B8790 (SubtitleRenderer +0x18: inline IsInCutscene / SetAsUpdatingTimer,
//   IsWaitingForClickPast 0x0048DB46, Clear 0x00492BAB); CCreatureAction_PlayAnimationWithLookTurning(.., const CCharString&, ..)
//   0x00842DB0 / (.., const CAnimationEntry&, ..) 0x00842E40, CCreatureAction_PlayConversationAnimation 0x00842400,
//   ~CCreatureActionBase 0x00693EF0, CThingCreatureBase::SetCurrentAction 0x006644F0 / GetRandomAnimFromName 0x00662D80 /
//   GetAnimDuration 0x006627F0; NCreatureMode::IsAbleToUseConversationAnimations 0x0085F180; GFFloatRandom 0x004978A0 (seed
//   CMainGameComponent +0x1613C); the private siblings IsCameraToBeSetupDueToMovedSetupPositions 0x00713010, SetUpCamera 0x00715A70,
//   SetUpTextAndSpeech 0x00716B30. Data: CurrentCameraTagIndex 0x01380C34, DefaultCameraTags 0x013BAD18, ConstantFPS 0x01375550,
//   "CONV_GENERAL" 0x01265994, 0.5f 0x0122F59C.
// Shape (chunk 2010): the tag tests are `key * GameTurnDuration <= UpdateCounter` (product first, `test ah,1 ; jne`; the
//   `UpdateCounter >= ...` spelling loads the counter first and tests 0x41); the gesture's animation duration takes a named
//   creature reference and a named fps local ahead of the call (retail reads ConstantFPS before GetAnimDuration and keeps it in
//   the frame for the `fimul`, 0x007170B0 / 0x007170BA); the CSoundPair arguments are CSoundPair(0) (two zero stores).
// Header overlay: CTCTalk.h SPEECHSAMPLEPLAYING, CThingCreatureBase.h ANIMDURATION, CSubtitleRenderer.h CLICKPAST, the
//   CCreatureAction_PlayAnimation(WithLookTurning) IMPLICIT_DTOR and the CCreatureAction_PlayConversationAnimation retail model
//   (hdr/README.md). CActionTalkBase / CSubtitledMessage have no shared header yet: the row-local models of the landed sibling
//   0x00716B30 at the retail offsets (header debt, NOTES.md).
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
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTION_PLAYCONVERSATIONANIMATION
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
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

class CASoundBank;
namespace NCreatureMode { bool FABLE_FASTCALL IsAbleToUseConversationAnimations(const CThing& thing); }   // 0x0085F180

class CSubtitledMessage {                                                       // Ego_r.pdb members, retail 0x44 bytes
public:
    class CCameraTag { public: long CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView; };
    CASoundBank* PSpeechBank;                                                   // +0x00
    long SpeechIndex;                                                           // +0x04
    bool Speech3D;                                                              // +0x08
    CWideString Text;                                                           // +0x0C
    CCharString Narrator;                                                       // +0x10
    CVectorMap<float, EConversationAttitude> AttitudeTags;                      // +0x14
    CVectorMap<float, CCharString> AnimationTags;                               // +0x24
    CVectorMap<float, CCameraTag> CameraTags;                                   // +0x34
};

inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t = static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated = true; }

namespace NTCScriptedControl {
class CScriptConversation;
class CActionTalkBase : public CTCScriptedControl::CActionBase {               // retail = Ego_r.pdb - 4 after Messages (12-byte CArray)
public:
    virtual void Update();                                                      // slot 1
protected:
    virtual void DisplayText();                                                 // slot 6 (FableWin ?DisplayText@..@@MAEXXZ)
    CScriptThing PTargetThing;                                                  // +0x0C
    long UpdateCounter;                                                         // +0x18
    double TimeCurrentLineStarted;                                              // +0x20
    double TimeCurrentLineLasts;                                                // +0x28
    CArray<CSubtitledMessage> Messages;                                         // +0x30
    long CurrentMessageIndex;                                                   // +0x3C
    CCountedPointer<CScriptConversation> Conversation;                          // +0x40
    long GameTurnDuration;                                                      // +0x48
    long NextAttitudeTagIndex;                                                  // +0x4C
    long NextAnimationTagIndex;                                                 // +0x50
    long NextCameraTagIndex;                                                    // +0x54
    long LastActionPriority;                                                    // +0x58
    long FramesUntilNextGesture;                                                // +0x5C
    long LastGestureAnimIndex;                                                  // +0x60
    long CameraToBeMovedDueToMovedSetupPositionsCount;                          // +0x64
    long PreviousConversationAttitude;                                          // +0x68 EConversationAttitude
    bool AddedConversationMode, FarEnoughAwayForFacingCamera, MakeTargetListen, UseCameraTags;   // +0x6C
    bool CameraTagsWereUsed, LookingAtListener, SoundIn2D, SoundStartedPlaying; // +0x70
    float ThingLookingFromPos[3], ThingLookingAtPos[3];                         // +0x74 C3DVector x2
    bool ThingLookingPosSet, ParentIsDemonDoor, GettingUp, SkippedCurrentLine;  // +0x8C
    static long CurrentCameraTagIndex;                                          // 0x01380C34 (descriptive; -1 initially)
    static std::vector<CSubtitledMessage::CCameraTag> DefaultCameraTags;        // 0x013BAD18
private:
    bool IsCameraToBeSetupDueToMovedSetupPositions(const CSubtitledMessage::CCameraTag& tag) const;   // 0x00713010
    void SetUpCamera(const CSubtitledMessage::CCameraTag& tag);                 // 0x00715A70
    void SetUpTextAndSpeech();                                                  // 0x00716B30
};

void CActionTalkBase::Update()
{
    LookingAtListener = true;
    if (PTargetThing.GetPThing() && PTargetThing.GetPThing() != &GetParentThing()) {
        CTCLook* look;
        if (GetParentThing().GetTC(TCI_LOOK, &look)) {
            C3DVector pos = PTargetThing.GetPThing()->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            if (!look->IsOverriddenByParent())
                look->LookAtPos(pos, 0xa3);
            LookingAtListener = look->IsPosInViewRange(pos, 1.0f / 6.0f, false, false);
        }
        CTCLook* target_look;
        if (PTargetThing.GetPThing()->GetTC(TCI_LOOK, &target_look)) {
            C3DVector pos = GetParentThing().GetTCGraphicAppearance()->GetFocalPos(1.0f);
            if (!target_look->IsOverriddenByParent())
                target_look->LookAtPos(pos, 0xa3);
        }
    }
    if (!PTargetThing.IsNull())
        PTargetThing.UpdateThingAttachment();
    CTCTalk* talk = 0;
    GetParentThing().GetTC(&talk);
    if (!SoundStartedPlaying) {
        if (!talk || !talk->IsSpeechSamplePlaying() || talk->IsSpeechSampleLoadedAndPlaying()) {
            TimeCurrentLineStarted = GFGetTime();
            CSubtitleRenderer& subtitles = CPlayerGui::Get().GetSubtitleRenderer();
            if (!subtitles.IsInCutscene())
                subtitles.SetAsUpdatingTimer(true);
            SoundStartedPlaying = true;
        }
    }
    if (SoundStartedPlaying)
        ++UpdateCounter;
    if (SkippedCurrentLine || (SoundStartedPlaying && GFGetTime() - TimeCurrentLineStarted > TimeCurrentLineLasts)) {
        if (!CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene() && CPlayerGui::Get().GetSubtitleRenderer().IsWaitingForClickPast())
            return;
        CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
        if (talk)
            talk->StopCurrentSpeech();
        if ((unsigned long)++CurrentMessageIndex < Messages.size()) {
            SetUpTextAndSpeech();
            return;
        }
        Terminate();
        return;
    }
    if (!PTargetThing.IsAlive()) {
        Terminate();
        return;
    }
    if (NextAnimationTagIndex != -1) {
        CVectorMap<float, CCharString>& tags = Messages[CurrentMessageIndex].AnimationTags;
        if (tags[NextAnimationTagIndex].first * GameTurnDuration <= UpdateCounter) {
            GetParentCreature().SetCurrentAction(CCreatureAction_PlayAnimationWithLookTurning(GetParentCreature(),
                tags[NextAnimationTagIndex].second, CSoundPair(0), false, false, ++LastActionPriority, 0));
            if ((unsigned long)++NextAnimationTagIndex >= tags.size())
                NextAnimationTagIndex = -1;
        }
    }
    if (FramesUntilNextGesture > 0)
        --FramesUntilNextGesture;
    if (FramesUntilNextGesture == 0 && LookingAtListener) {
        const CAnimationEntry& anim = GetParentCreature().GetRandomAnimFromName("CONV_GENERAL", LastGestureAnimIndex);
        CThingCreatureBase& creature = GetParentCreature();
        long fps = CMainGameComponent::GetConstantFPS();
        long duration = GFFloatToLongCeil(creature.GetAnimDuration(anim.BankIndex) * fps);
        if (duration <= GameTurnDuration - UpdateCounter && NCreatureMode::IsAbleToUseConversationAnimations(GetParentThing())) {
            GetParentCreature().SetCurrentAction(CCreatureAction_PlayAnimationWithLookTurning(GetParentCreature(),
                anim, CSoundPair(0), false, false, LastActionPriority, 0));
        }
        LastGestureAnimIndex = anim.BankIndex;
        double gesture_delay = GFFloatRandom(1.7f, CMainGameComponent::Get()->GetWorldSeed()) + 0.5f;
        FramesUntilNextGesture = GFFloatToLongCeil((float)(CMainGameComponent::GetConstantFPS() * gesture_delay));
    }
    if (NextAttitudeTagIndex != -1) {
        CVectorMap<float, EConversationAttitude>& tags = Messages[CurrentMessageIndex].AttitudeTags;
        if (tags[NextAttitudeTagIndex].first * GameTurnDuration <= UpdateCounter) {
            if (talk) {
                talk->SetAttitude(tags[NextAttitudeTagIndex].second);
            } else {
                ++LastActionPriority;
                if (NCreatureMode::IsAbleToUseConversationAnimations(GetParentThing()))
                    GetParentCreature().SetCurrentAction(CCreatureAction_PlayConversationAnimation(GetParentCreature(),
                        tags[NextAttitudeTagIndex].second, CSoundPair(0), 0, LastActionPriority));
            }
            if ((unsigned long)++NextAttitudeTagIndex >= tags.size())
                NextAttitudeTagIndex = -1;
        }
    }
    bool camera_tag_set = false;
    if (NextCameraTagIndex != -1) {
        CVectorMap<float, CSubtitledMessage::CCameraTag>& tags = Messages[CurrentMessageIndex].CameraTags;
        CameraTagsWereUsed = true;
        if (tags[NextCameraTagIndex].first * GameTurnDuration <= UpdateCounter) {
            SetUpCamera(tags[NextCameraTagIndex].second);
            camera_tag_set = true;
            if ((unsigned long)++NextCameraTagIndex >= tags.size())
                NextCameraTagIndex = -1;
        }
    }
    const CSubtitledMessage::CCameraTag* tag;
    if (NextCameraTagIndex > 0 && CameraTagsWereUsed && !camera_tag_set) {
        tag = &Messages[CurrentMessageIndex].CameraTags[NextCameraTagIndex - 1].second;
    } else {
        if (CurrentCameraTagIndex == -1)
            return;
        tag = &DefaultCameraTags[CurrentCameraTagIndex];
    }
    if (tag) {
        if (CameraToBeMovedDueToMovedSetupPositionsCount) {
            if (++CameraToBeMovedDueToMovedSetupPositionsCount > 10) {
                CameraToBeMovedDueToMovedSetupPositionsCount = 0;
                SetUpCamera(*tag);
            }
        } else if (IsCameraToBeSetupDueToMovedSetupPositions(*tag)) {
            ++CameraToBeMovedDueToMovedSetupPositionsCount;
        }
    }
}
}
