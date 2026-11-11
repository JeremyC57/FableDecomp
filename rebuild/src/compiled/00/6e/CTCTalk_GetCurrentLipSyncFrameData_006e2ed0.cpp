// CTCTalk::GetCurrentLipSyncFrameData(CLipSyncFrameData& frame_data) -- when the talker can be seen (a cut scene, or within 6 units of the LOD centre and rendered last frame) and a lip-sync bank is loaded, sample the lip-sync data of the current speech: from the playing sample's time when the attached emitter is playing it, else from the mime start while the mime window lasts
// Leaf: ?GetCurrentLipSyncFrameData@CTCTalk@@AAE_NAAVCLipSyncFrameData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2ED0, 673 bytes (688 with padding); thiscall, `ret 4`, returns bool.
// Identity: Ego_r.pdb / FableWin.pdb ?GetCurrentLipSyncFrameData@CTCTalk@@AAE_NAAVCLipSyncFrameData@@@Z (private); the one caller is the
//   talk update at 0x006E3C65 (`lea edx,[esp+20h] ; push edx ; mov ecx,ebx`, then the frame vector's (end - begin) / 12 is read). The
//   catalog's "CViewBase_ProcessLipSync" is a placeholder. The Ego_r / FableWin bodies are 16 / 32 bytes (the TLC body is new).
// Callees: CTCBase::GetWorld 0x00686A70 (twice), CGameScriptInterface::IsInMovieSequence (slot 395, +0x62C) through
//   CWorld::GetGameScriptInterface (inline, +0x38), CWorld::GetGameLODCentre (slot 8, +0x20), CThing::GetPos 0x004C73D0,
//   NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame 0x004BD200, CASoundManager::GetPLipSyncBank 0x009916F0 /
//   GetAnyPLipSyncBank 0x00991720 (the inline CTCTalk::GetPLipSyncBank, called out of line at 0x006E2860 at the later sites),
//   CTCBase::PeekSampleBank 0x00686D70 (inline CTCTalk::GetSpeechBank), GFGetTime 0x009E1BC0, CVectorMap::LowerBound 0x0040F020
//   (GetTC<CTCSoundPlayer>, interface 0x10), CBaseIntelligentPointer::GetPItem 0x00A01B50, CASoundEmitter slots 23 / 24
//   (IsSamplePlaying +0x5C, GetActiveSampleInfo +0x60), CLipSyncDataBank::PeekLipSyncInfo 0x00A42830 / GetLipSyncData 0x00A42FC0,
//   CLipSyncSampleData::GetFrameInterpolate 0x00A48F10, ~CCountedPointer<CLipSyncSampleData> 0x006E46F0, ~vector<CActiveSoundInfo>
//   0x006E4DD0.
// Globals: PSoundManager 0x013B8394, CMainGameComponent::ConstantFPS 0x01375550, 36.0f 0x0126007C, 0.0f 0x0122DEDC.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKSAMPLEBANK
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_LIPSYNC
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_TALKFRIEND
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_TALKFRIEND
#define FABLE_RETAIL_CASOUNDEMITTER_ACTIVESAMPLEINFO_ORDER
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CLIPSYNCDATABANK_LIPSYNCDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_WASRENDERED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCTalk.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CASoundEmitter.h"
#include "engine/CActiveSoundInfo.h"
#include "engine/CASoundManager.h"
#include "engine/CLipSyncDataBank.h"
#include "engine/CLipSyncSampleData.h"
#include "engine/CCountedPointer.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFGetTime.h"

// Same-TU body (the CTCTalk TU's; retail keeps the out-of-line copy at 0x006E2860 and expands the first call here).
const CLipSyncDataBank* CTCTalk::GetPLipSyncBank() const
{
    const CLipSyncDataBank* lip_sync_bank = 0;
    if (PSoundManager) {
        lip_sync_bank = PSoundManager->GetPLipSyncBank(GetSpeechBank());
        if (!lip_sync_bank)
            lip_sync_bank = PSoundManager->GetAnyPLipSyncBank();
    }
    return lip_sync_bank;
}

bool CTCTalk::GetCurrentLipSyncFrameData(CLipSyncFrameData& frame_data)
{
    if (!GetWorld().GetGameScriptInterface().IsInMovieSequence()
        && ((GetParentThing().GetPos() - GetWorld().GetGameLODCentre()).GetSquaredMagnitude() > 36.0f
            || !GetParentThing().GetTCGraphicAppearance() || !GetParentThing().GetTCGraphicAppearance()->WasRenderedLastFrame()))
        return false;
    const CLipSyncDataBank* lip_sync_bank = GetPLipSyncBank();
    if (!lip_sync_bank)
        return false;
    float now = (float)GFGetTime();
    CTCSoundPlayer* sound_player = 0;
    GetParentThing().GetTC(&sound_player);
    if (SpeechSample > 0) {
        if (sound_player->PAttachedEmitter.Get()) {
            CASoundEmitter* emitter = sound_player->PAttachedEmitter.Get()->PSoundEmitter;
            if (emitter && emitter->IsSamplePlaying(GetSpeechBank(), SpeechSample)
                && GetPLipSyncBank()->PeekLipSyncInfo(SpeechSample)) {
                std::vector<CActiveSoundInfo> active_samples;
                emitter->GetActiveSampleInfo(GetSpeechBank(), SpeechSample, active_samples);
                if (active_samples.empty())
                    return false;
                float time = (float)GFGetTime() - SampleLoadedTime;
                CCountedPointer<CLipSyncSampleData> sample_data = GetPLipSyncBank()->GetLipSyncData(SpeechSample);
                sample_data->GetFrameInterpolate(frame_data, time);
                return true;
            }
        } else if (MimeStartTime > 0.0f
                   && (float)SampleDurationFrames / CMainGameComponent::GetConstantFPS() + MimeStartTime > now
                   && lip_sync_bank->PeekLipSyncInfo(SpeechSample)) {
            float time = now - MimeStartTime;
            CCountedPointer<CLipSyncSampleData> sample_data = GetPLipSyncBank()->GetLipSyncData(SpeechSample);
            sample_data->GetFrameInterpolate(frame_data, time);
            return true;
        }
    }
    return false;
}