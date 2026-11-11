// NTCScriptedControl::CActionTalkBase::SetUpTextAndSpeech() -- start the current conversation line: reset the line's tag cursors, remember the speaker / listener pair (a new pair resets the conversation camera), play the line's speech (or random lip movement for a silent line), time the line from the speech duration in game turns, pick the next gesture delay, take the first attitude tag, arm the animation tags, and pick the line's camera (its own tags, else a default camera tag), then let the action update
// Leaf: ?SetUpTextAndSpeech@CActionTalkBase@NTCScriptedControl@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00716B30, 681 bytes (+7 int3 to 0x00716DE0), thiscall, `ret`; ebp frame (the inline-asm ceiling).
// Retail-asm: 0x00716CA5 `fistp dword ptr [ebp-4]` and 0x00716D02 `fistp dword ptr [ebp-4]` -- the engine's inline-assembly
//   GFFloatToLongCeil (engine/GFFloatToLongCeil.h: `fld ; fadd half ; fistp ; fild ; fsub one ; fstp ; cmp ; jne ; dec`) expanded twice.
// Shape: the speech-duration / gesture-delay products are double (`fild ConstantFPS ; fmul st(1)`): the line length is the double
//   member, the gesture delay a double local; the narrator is passed through the inline GetNarrator() (Ego_r.pdb
//   ?GetNarrator@CActionTalkBase@NTCScriptedControl@@IBEABVCCharString@@XZ), which makes VC7.1 form the message address before the
//   SpeechIndex load (0x00716C3B `add eax,ecx`; a message reference / pointer alone folds it into `[eax+ebx+4]`).
// Identity: Ego_r.pdb ?SetUpTextAndSpeech@CActionTalkBase@NTCScriptedControl@@AAEXXZ (donor 0x0072AFB0, 720 bytes, before Update
//   0x0072B280 = retail 0x00716DE0); private, no stack arguments.
// Callees: CScriptThing slots 75 IsAlive / 11 GetPThing (the target), CVectorMap LowerBound 0x0040F020 (GetTC<CTCTalk>, TCI_TALK 15),
//   CTCTalk::PlaySpeech(CASoundBank&, long, bool, const CCharString&, EDialogueLayer, bool, bool) 0x006E3580, the silent-line call
//   0x006E2A50 (landed as CTCTalk::SetUpSpeechSample), GFGetTime 0x009E1BC0, CTCTalk::GetCurrentSpeechDuration 0x006E2840,
//   GFFloatRandom 0x004978A0 (seed CMainGameComponent +0x1613C), CTCBase::GetWorld 0x00686A70, the current game camera's slot 9
//   GetCameraManager, CGameCameraManager::IsUsingScriptedCameraMode 0x00697AE0, GetNextCameraTagIndex 0x00712E40,
//   SetUpCamera 0x00715A70, the action's slot 6 (Update tail).
// Data: static speaker / listener UIDs 0x013BAD08 / 0x013BAD10 (descriptive), last camera tag index 0x01380C34, camera side
//   0x013BAD00, DefaultCameraTags 0x013BAD18, ConstantFPS 0x01375550, 0.5-turn constant 0x0122F59C (the speech-duration pad).
// Header overlay: hdr/engine/CTCTalk.h (FABLE_RETAIL_CTCTALK_CONVERSATION). CActionTalkBase / CSubtitledMessage have no shared header
//   yet: row-local models at the retail offsets (header debt, NOTES.md).
#define FABLE_RETAIL_CCHARSTRING
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
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCBASE_TALKACTION_FRIEND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDMODE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCTalk.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CMainGameComponent.h"
#include "engine/CVectorMap.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFGetTime.h"

class CASoundBank;
enum EConversationAttitude;
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_CONVERSATION = 0 }; }   // enumerator name descriptive

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

namespace NTCScriptedControl {
class CScriptConversation;
class CActionTalkBase : public CTCScriptedControl::CActionBase {               // retail = Ego_r.pdb - 4 after Messages (12-byte CArray)
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
    static unsigned __int64 SpeakerUniqueID;                                    // 0x013BAD08 (descriptive)
    static unsigned __int64 ListenerUniqueID;                                   // 0x013BAD10 (descriptive)
    static long CurrentCameraTagIndex;                                          // 0x01380C34 (descriptive; -1 initially)
    static long ConversationCameraSide;                                         // 0x013BAD00 (descriptive)
    static std::vector<CSubtitledMessage::CCameraTag> DefaultCameraTags;        // 0x013BAD18
private:
    CGameCameraManager& GetCameraManager() const                                // Ego_r ?GetCameraManager@..@@ABEAAVCGameCameraManager@@XZ
    {
        CGameCameraManager* manager;
        PTCScript->GetWorld().GetGameCamera()->GetCameraManager(&manager);
        return *manager;
    }
protected:
    const CCharString& GetNarrator() const { return Messages[CurrentMessageIndex].Narrator; }   // Ego_r ?GetNarrator@..@@IBEABVCCharString@@XZ
    long GetNextCameraTagIndex(long current_index);                             // 0x00712E40
    void SetUpCamera(const CSubtitledMessage::CCameraTag& tag);                 // 0x00715A70
private:
    void SetUpTextAndSpeech();
};

void CActionTalkBase::SetUpTextAndSpeech()
{
    NextAttitudeTagIndex = -1;
    NextAnimationTagIndex = -1;
    NextCameraTagIndex = -1;
    SoundStartedPlaying = false;
    SkippedCurrentLine = false;
    unsigned __int64 speaker = GetParentThing().GetUniqueID();
    unsigned __int64 listener = 0;
    if (PTargetThing.IsAlive())
        listener = PTargetThing.GetPThing()->GetUniqueID();
    if (!((SpeakerUniqueID == speaker || SpeakerUniqueID == listener)
          && (ListenerUniqueID == speaker || ListenerUniqueID == listener))) {
        CurrentCameraTagIndex = -1;
        ConversationCameraSide = 0;
    }
    SpeakerUniqueID = speaker;
    ListenerUniqueID = listener;
    CTCTalk* talk;
    if (GetParentThing().GetTC(&talk)) {
        const CSubtitledMessage& message = Messages[CurrentMessageIndex];
        if (message.SpeechIndex > 0) {
            talk->PlaySpeech(*message.PSpeechBank, message.SpeechIndex, false, GetNarrator(),
                             NSpeechGainManager::DIALOGUE_LAYER_CONVERSATION, SoundIn2D, true);
        } else {
            talk->PlayRandomLipMovement();
            SoundStartedPlaying = true;
            TimeCurrentLineStarted = GFGetTime();
        }
        TimeCurrentLineLasts = talk->GetCurrentSpeechDuration() + 0.5f;
        GameTurnDuration = GFFloatToLongCeil((float)(CMainGameComponent::GetConstantFPS() * TimeCurrentLineLasts));
        UpdateCounter = 0;
        double gesture_delay = GFFloatRandom(1.7f, CMainGameComponent::Get()->GetWorldSeed()) + 0.5f;
        FramesUntilNextGesture = GFFloatToLongCeil((float)(CMainGameComponent::GetConstantFPS() * gesture_delay));
        if (!Messages[CurrentMessageIndex].AttitudeTags.empty())
            talk->SetAttitude(Messages[CurrentMessageIndex].AttitudeTags.begin()->second);
        if (!Messages[CurrentMessageIndex].AnimationTags.empty()) {
            NextAnimationTagIndex = 0;
            FramesUntilNextGesture = -1;
        }
        if (UseCameraTags) {
            if (!Messages[CurrentMessageIndex].CameraTags.empty()) {
                NextCameraTagIndex = 0;
            } else {
                if (!GetCameraManager().IsUsingScriptedCameraMode())
                    CameraTagsWereUsed = true;
                long index = GetNextCameraTagIndex(CurrentCameraTagIndex);
                if (index != CurrentCameraTagIndex) {
                    CurrentCameraTagIndex = index;
                    SetUpCamera(DefaultCameraTags[index]);
                }
            }
        }
    }
    DisplayText();
}
}