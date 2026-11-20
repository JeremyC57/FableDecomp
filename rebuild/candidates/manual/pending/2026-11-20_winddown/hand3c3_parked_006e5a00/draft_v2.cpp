// CScriptConversation::FrameUpdate()
// Leaf: ?FrameUpdate@CScriptConversation@@QAEXXZ
// Flags: none
// Retail-asm: 0x006E5E69 `fistp dword ptr [ebp-10h]` and 0x006E5F6E `fistp dword ptr [ebp-10h]`: GFFloatToLongCeil (engine/GFFloatToLongCeil.h) expanded twice
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CSUBTITLERENDERER_CLICKPAST
#define FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH
#define FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH_ACCESSORS
#define FABLE_RETAIL_CLIPSYNCDATABANK_LIPSYNCDATA
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_CONVERSATION
#define FABLE_RETAIL_CTCTALK_SPEECHSAMPLEPLAYING
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CSCRIPTCONVERSATION_CLASS
#define FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CArray.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundBank.h"
#include "engine/CLipSyncDataBank.h"
#include "engine/CLipSyncDataBankEntry.h"
#include "engine/CTCTalk.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCLook.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/CScriptConversation.h"
#include "engine/CScriptConversationManager.h"

void CScriptConversation::FrameUpdate()
{
    if (!IsValid())
    {
        Manager->RemoveConversation(ConversationIndex, false);
        return;
    }
    if (!PlayDuringCutScenes)
    {
        bool in_cutscene = CMainGameComponent::Get()->GetWorld().GetThingManager().IsNonScriptedEntitesPauseMode();
        if (in_cutscene)
        {
            if (CurrentLine != -1)
            {
                CThing* ptalker = GetCurrentSpeaker();
                if (IS_THING_ALIVE(ptalker))
                {
                    CTCTalk* ptc_talk = NULL;
                    if (ptalker->GetTC(&ptc_talk))
                        ptc_talk->StopCurrentSpeech();
                }
            }
            Manager->RemoveConversation(ConversationIndex, false);
            return;
        }
    }
    bool decrement_frame_counter = CurrentLine == -1 || StartedCurrentLineSpeech;
    if (!decrement_frame_counter)
    {
        CThing* ptalker = GetCurrentSpeaker();
        const CTCTalk* ptc_talk = NULL;
        if (ptalker->PeekTC(&ptc_talk))
        {
            StartedCurrentLineSpeech = !ptc_talk->IsSpeechSamplePlaying() || ptc_talk->IsSpeechSampleLoadedAndPlaying();
            decrement_frame_counter = StartedCurrentLineSpeech;
        }
    }
    if (decrement_frame_counter)
    {
        FramesLeftBeforeNewLine--;
        if (CurrentLine > -1 && Conversation[CurrentLine].ShowSubtitle)
            CPlayerGui::Get().GetSubtitleRenderer().SetAsUpdatingTimer(true);
    }
    bool ready_for_next_line = CurrentLine == -1;
    if (!ready_for_next_line)
    {
        CThing* ptalker = GetCurrentSpeaker();
        ready_for_next_line = FramesLeftBeforeNewLine <= 0;
        if (ready_for_next_line)
        {
            CTCTargetingBase* ptc_talker_targeting = NULL;
            if (ptalker->GetTC(&ptc_talker_targeting))
                ptc_talker_targeting->SetPScriptOverrideInterestingThing(NULL);
            CThing* ptalked_to = GetCurrentListener();
            if (IS_THING_ALIVE(ptalked_to))
            {
                CTCTargetingBase* ptc_talked_to_targeting = NULL;
                if (ptalked_to->GetTC(&ptc_talked_to_targeting))
                    ptc_talked_to_targeting->SetPScriptOverrideInterestingThing(NULL);
            }
        }
    }
    if (ready_for_next_line)
    {
        CurrentLine++;
        if ((unsigned long)CurrentLine < Conversation.size())
        {
            CThing* ptalker = GetCurrentSpeaker();
            StartedCurrentLineSpeech = true;
            float speech_duration = 2.5f;
            const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
            unsigned long text_index = text_bank.LookupValue(Conversation[CurrentLine].LineToSay.GetCRC());
            if (text_index > 0)
            {
                if (text_bank.GetEntryDataType(text_index) == 1)
                {
                    const CArray<long>& group_entries = text_bank.GetPGroupEntry(text_index, true)->GetTextEntries();
                    long random_index = GFRandom(group_entries.GetSize(), CMainGameComponent::Get()->GetWorld().GetWorldSeed());
                    text_index = group_entries[random_index];
                }
                CIVCountedPointer<const NGameText::CTextEntry> ptext_entry = text_bank.GetPTextEntry(text_index, true);
                if (ptext_entry)
                {
                    CASoundBank* psound_bank = CASoundManager::Get()->GetPSoundBank(ptext_entry->SpeechBank);
                    if (psound_bank)
                    {
                        CCharString symbol = "SND_" + ptext_entry->GetSoundName();
                        long sound_index = psound_bank->GetSoundIndexFromSymbol(symbol);
                        if (sound_index > 0)
                        {
                            const CLipSyncDataBank* plipsync_bank = CASoundManager::Get()->GetPLipSyncBank(*psound_bank);
                            if (plipsync_bank)
                            {
                                const CLipSyncDataBankEntry* pentry = plipsync_bank->PeekLipSyncInfo(sound_index);
                                if (pentry)
                                    speech_duration = pentry->GetDuration();
                            }
                            CTCTalk* ptc_talk = NULL;
                            if (ptalker->GetTC(&ptc_talk))
                            {
                                ptc_talk->PlaySpeech(*psound_bank, sound_index, false, ptext_entry->GetNarrator(),
                                                     (NSpeechGainManager::EDialogueLayer)1, SoundIn2D, true);
                                StartedCurrentLineSpeech = !ptc_talk->IsSpeechSamplePlaying() || ptc_talk->IsSpeechSampleLoadedAndPlaying();
                            }
                        }
                    }
                    if (Conversation[CurrentLine].ShowSubtitle)
                    {
                        CWideString text_of_message = ptext_entry->GetText();
                        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), text_of_message, 2, speech_duration, false, true);
                        CPlayerGui::Get().GetSubtitleRenderer().SetAsUpdatingTimer(false);
                    }
                }
                FramesLeftBeforeNewLine = GFFloatToLongCeil((speech_duration + 0.5f) * CMainGameComponent::GetConstantFPS());
            }
            else
            {
                CTCTalk* ptc_talk = NULL;
                if (ptalker->GetTC(&ptc_talk))
                    ptc_talk->PlayRandomLipMovement();
                if (Conversation[CurrentLine].ShowSubtitle)
                {
                    CWideString display_text = L"(HC)" + Conversation[CurrentLine].LineToSay.ToWideString();
                    CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), display_text, 2, speech_duration, false, true);
                }
                FramesLeftBeforeNewLine = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * speech_duration);
            }
            CThing* plistener = GetCurrentListener();
            if (IS_THING_ALIVE(plistener) && ptalker != plistener)
            {
                CTCTargetingBase* ptc_talker_targeting = NULL;
                if (ptalker->GetTC(&ptc_talker_targeting))
                    ptc_talker_targeting->SetPScriptOverrideInterestingThing(plistener);
                CTCTargetingBase* ptc_listener_targeting = NULL;
                if (plistener->GetTC(&ptc_listener_targeting))
                    ptc_listener_targeting->SetPScriptOverrideInterestingThing(ptalker);
            }
        }
        else
        {
            Manager->RemoveConversation(ConversationIndex, false);
            return;
        }
    }
    CThing* ptalker = GetCurrentSpeaker();
    CTCLook* ptc_talker_look = NULL;
    if (ptalker->GetTC(&ptc_talker_look))
        ptc_talker_look->TrackNearestInterestingThing(true);
    CThing* plistener = GetCurrentListener();
    if (IS_THING_ALIVE(plistener))
    {
        CTCLook* ptc_listener_look = NULL;
        if (plistener->GetTC(&ptc_listener_look))
            ptc_listener_look->TrackNearestInterestingThing(true);
    }
}
