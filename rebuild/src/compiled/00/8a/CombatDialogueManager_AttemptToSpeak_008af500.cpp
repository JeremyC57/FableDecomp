// CombatDialogueManager::AttemptToSpeak() -- when the current talker is alive, has a pending text group and a CTCTalk interface, play the AI speech; started -> stamp the group's NextAllowedFrame (frame + fps * MinTimeBetweenSpeechRepeat) and clear the pending group, final failure -> clear it
// Leaf: ?AttemptToSpeak@CombatDialogueManager@@AAEXXZ
// Flags: none
// Retail: Fable.exe @ 0x008AF500, 235 bytes, `__fastcall`, plain `ret` (Ego_r.pdb ?AttemptToSpeak@CombatDialogueManager@@AAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CVectorMap lower_bound 0x0040F020 (GetTC<CTCTalk>, TCI_TALK 0xF),
//   CTCTalk::PlayAISpeech 0x006E3B60, the map's _Rb_tree::find 0x008AEED0, CMainGameComponent::GetCurrentWorldFrame() const
//   0x004167C8 on CMainGameComponent::Get() (0x013B86A0), CMainGameComponent::GetConstantFPS() (0x01375550), _ftol 0x00BFEA70.
// Layout: the shared CombatDialogueManager (SpeechGroupInfoMap +0x00, PDialogueDef +0x0C, PCurrentTalker +0x10,
//   CurrentTalkerTextGroupIndex +0x18); CCombatDialogueDef MinTimeBetweenSpeechRepeat +0x40 (Ego_r +0x44).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDefClassBase.h"
#include "engine/CTCTalk.h"
#include "engine/CombatDialogueManager.h"
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 }; }
class CCombatDialogueDef : public CDefClassBase {                   // no shared header (retail offsets: Ego_r minus 4)
public:
    unsigned char _pad[0x3c - sizeof(CDefClassBase)];
    float MinTimeBetweenSpeech;                                     // +0x3C
    float MinTimeBetweenSpeechRepeat;                               // +0x40
    float MinDelayAfterBattleCry;                                   // +0x44
};
template<class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() != 0 && !p.Get()->IsDead(); }

void CombatDialogueManager::AttemptToSpeak()
{
    if (!IS_THING_ALIVE(PCurrentTalker))
        return;
    if (CurrentTalkerTextGroupIndex == 0)
        return;
    CTCTalk* talk;
    if (!PCurrentTalker.Get()->GetTC(&talk))
        return;
    CTCTalk::ESpeechPlaybackResult result = talk->PlayAISpeech(CurrentTalkerTextGroupIndex, false, NSpeechGainManager::DIALOGUE_LAYER_1, false, false);
    if (result == CTCTalk::SPEECH_PLAYBACK_STARTED) {
        std::map<unsigned long, TSpeechGroupInfo>::iterator it = SpeechGroupInfoMap.find(CurrentTalkerTextGroupIndex);
        it->second.NextAllowedFrame =
            (long)(CMainGameComponent::Get()->GetCurrentWorldFrame() + CMainGameComponent::GetConstantFPS() * PDialogueDef->MinTimeBetweenSpeechRepeat);
        CurrentTalkerTextGroupIndex = 0;
    }
    else if (result == CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL)
        CurrentTalkerTextGroupIndex = 0;
}