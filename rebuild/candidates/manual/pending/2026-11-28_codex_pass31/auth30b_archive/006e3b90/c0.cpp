// 006e3b90 CTCTalk::Update
// Leaf: ?Update@CTCTalk@@QAEXAAVCAnimateThing@@ABVCAnimateThingUpdate@@@Z
// Flags: none
#define FABLE_RETAIL_TALK_UPDATE_PASS29
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_UPDATE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CTCTALK_LIPSYNC
#define FABLE_RETAIL_CTCBASE_PEEKSAMPLEBANK
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CANIMINIT_STRAFE_CALLED_PASS24
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMATETHING_REMOVE_COMBINATION_CALL_PASS24
#define FABLE_RETAIL_CLIPSYNCPHONEME_GETSYMBOL_INLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CThing.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCTalk.h"
#include "engine/CLipSyncFrameData.h"
#include "engine/CAnimateThing.h"
#include "engine/GFGetTime.h"

void CTCTalk::Update(CAnimateThing& anim, const CAnimateThingUpdate& anim_update)
{
    if (SpeechSample <= 0)
        return;
    CLipSyncFrameData frame_data;
    if (SampleLoadedTime < 0.0f) {
        const CTCSoundPlayer* sound_player;
        if (!GetParentThing().PeekTC(&sound_player) || !(SampleID > 0) ||
            sound_player->IsAttachedSoundLoadedAndPlaying(SampleID) ||
            CMainGameComponent::Get()->GetCurrentServerFrame() - SampleStartedFrame >
                CMainGameComponent::GetConstantFPS() * 10000.0f)
            SampleLoadedTime = (float)GFGetTime();
    } else if (UseLipSynching && GetCurrentLipSyncFrameData(frame_data)) {
        std::vector<CAnimInit> phoneme_anims;
        phoneme_anims.reserve(frame_data.GetPhonemeCount());
        for (unsigned long i = 0; i < frame_data.GetPhonemeCount(); ++i) {
            const CLipSyncPhoneme& phoneme = frame_data.GetPhoneme(i);
            const long index = GetPhonemeAnim(phoneme.GetSymbol());
            if (index > 0) {
                CAnimInit init;
                init.Init(19, 0, index, ANIM_UPDATE_NONE, false, 0, 102);
                init.LastAlpha = init.Alpha = phoneme.GetWeight();
                init.Frame = CAnimFrameInfo(1.0f);
                CAnimSet* old_anim = anim.GetCombinationAnimFromTypeAndIndex(19,index);
                if (old_anim) init.LastAlpha = old_anim->GetLastAlpha();
                phoneme_anims.push_back(init);
            }
        }
        anim.RemoveCombinationAnimsOfType(19);
        const unsigned long count = phoneme_anims.size();
        for (unsigned long i=0; i<count; ++i) {
            LipSynching = true;
            anim.AddCombinationAnim(phoneme_anims[i],0,0,true,false);
        }
    } else if (LipSynching) {
        std::vector<CAnimSet*> remaining;
        anim.GetAllCombinationAnimsFromType(19,remaining);
        unsigned long count=remaining.size();
        for (unsigned long i=0;i<remaining.size();++i) {
            if (remaining[i]->GetAlpha() == 0.0f) {
                anim.RemoveCombinationAnim(remaining[i]);
                --count;
            } else remaining[i]->SetAlpha(0.0f);
        }
        if (count==0) {
            LipSynching=false;
            SpeechSample=0;
            MimeStartTime=0.0f;
        }
    }
}
