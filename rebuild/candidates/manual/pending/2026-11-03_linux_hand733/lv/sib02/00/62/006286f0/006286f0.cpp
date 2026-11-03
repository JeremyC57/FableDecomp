#pragma optimize("t",on)
// GFPlayFullScreenXBoxMovie(const CCharString& file_name, unsigned long flags, unsigned long width, unsigned long height, unsigned long buffer_size, const NGameText::CDataBank* text_bank, CIEngine* engine, bool display_subtitles, float no_skip_duration, bool (__fastcall* pcallback)(), CXMVPlayerBuffers* pbuffers, CWideString* perror_message) -- play a full-screen movie through CVideoSys (PC): cancel the hero's bullet time, optionally fade the sound groups / pause two sound-manager flags, open the .wmv twin of the movie path, then copy each decoded frame into a texture and draw it letterboxed until the movie completes, a skip key is pressed or the system quits; restore the sound state
// Leaf: ?GFPlayFullScreenXBoxMovie@@YI_NABVCCharString@@KKKKPBVCDataBank@NGameText@@PAVCIEngine@@_NMP6I_NXZPAVCXMVPlayerBuffers@@PAVCWideString@@@Z
// Flags: default
// Retail: Fable.exe (TLC, VC7.1) @ 0x006286F0, 1864 bytes, __fastcall `ret 28h`, no EH frame (the /O2 default TU).
// Identity: Ego_r.pdb + FableWin.pdb ?GFPlayFullScreenXBoxMovie@@YI_N...@Z (Ego_r module release\movie_player_helper.obj,
//   whose local set names file_path, dimensions, time_start, renderTargetDim, event, clear_black, border, lparam1/2, dst_box,
//   outbox, whole_area, scan_info, event_code and v); callers 0x0041F933, 0x0042EDCD, 0x0042F1CB, 0x006D08CE and
//   CGameScriptInterface::PlayAVIMovie 0x0088F890. The TLC body is the PC CVideoSys path: width, height, buffer_size,
//   engine, display_subtitles, no_skip_duration, pcallback and pbuffers are not read.
// Headers (overlay, FABLE_RETAIL_* branches; see HEADER_NEEDS.md): CVideoSys / CMovieSubtitles / CTextureRenderer,
//   CSystemManager / CInputManager / CInputEvent / CInputEventScanInfo, CDisplayManager / CRenderManager(2D/Core),
//   CUserProfileManager, CASoundManager (slot 46 + the two bool setters; descriptive names), CTexture / CPixelFormat,
//   C2DBoxF / CUVBox / CRGBColour / C2DExtentsI, CAFile, CWideString, CMainGameComponent, the CThing chain + CTCBulletTime.
// Shape: the bullet-time cancel is CThing::GetTC<CTCBulletTime> (HasTC bit 0x92 at +0x30, then the 0x0040F020 lower bound);
//   the profile getters are expanded (UseNewProfileValues ? <X>NewProfile : <X>); `flags & 0x10` / `flags & 2` are the
//   compiler's CSE spills ([esp+28h] / [esp+20h]); the .xmv/.wmv literals convert implicitly into temporaries; the
//   Init-failure block sits after the success path (`if (hr >= 0) { ... return true; }`); a skip key leaves both loops
//   with a goto (Stop, then ~CInputEvent); GFGetSystemManager() is re-called for every manager access, as retail does.
// Probes: probe/006286f0.log (13 attempts).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CVIDEOSYS
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#include "fable_string.h"
#include "fable_game.h"
#include "fable_user_profile.h"
#include "engine/CAFile.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBulletTime.h"
#include "engine/CASoundManager.h"
#include "engine/CVideoSys.h"
#include "engine/CTextureRenderer.h"
#include "engine/CSystemManager.h"
#include "engine/CInputManager.h"
#include "engine/CInputEvent.h"
#include "engine/CInputEventScanInfo.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CRGBColour.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsI.h"
#include "engine/CUVBox.h"
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
#include "engine/ESurfacePool.h"

class CIEngine;
class CXMVPlayerBuffers;
double __fastcall GFGetTime();                                           // 0x009E1BC0 ?GFGetTime@@YINXZ
void __fastcall GFABoxToVBox(const C2DBoxF& abox, C2DBoxF& vbox);        // 0x009E2020 ?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z
extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
struct IDispatch : IUnknown {
    virtual long __stdcall GetTypeInfoCount(unsigned int*) = 0;
    virtual long __stdcall GetTypeInfo(unsigned int, unsigned long, void**) = 0;
    virtual long __stdcall GetIDsOfNames(const GUID&, unsigned short**, unsigned int, unsigned long, long*) = 0;
    virtual long __stdcall Invoke(long, const GUID&, unsigned long, unsigned short, void*, void*, void*, unsigned int*) = 0;
};
struct IMediaEvent : IDispatch {
    virtual long __stdcall GetEventHandle(long* hEvent) = 0;                                     // slot 7
    virtual long __stdcall GetEvent(long* lEventCode, long* lParam1, long* lParam2, long msTimeout) = 0;   // slot 8 (+0x20)
};
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long milliseconds, int alertable);  // 0x0143FE08
extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int key);                       // 0x014402CC

bool __fastcall GFPlayFullScreenXBoxMovie(const CCharString& file_name, unsigned long flags, unsigned long width,
    unsigned long height, unsigned long buffer_size, const NGameText::CDataBank* text_bank, CIEngine* engine,
    bool display_subtitles, float no_skip_duration, bool (__fastcall* pcallback)(), CXMVPlayerBuffers* pbuffers,
    CWideString* perror_message)
{
    if (CMainGameComponent::Get()) {
        CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead()) {
            CTCBulletTime* bullet_time;
            if (hero->GetTC(&bullet_time))
                bullet_time->ImmediateCancel();
        }
    }

    if ((flags & 0x10) && PSoundManager) {
        PSoundManager->SetGroupVolume(8, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
        PSoundManager->SetGroupVolume(0x10, 0.0f, 500.0f);
        PSoundManager->SetGroupVolume(0x40, 0.0f, 500.0f);
        PSoundManager->SetGroupVolume(0x80, 0.0f, 500.0f);
    }
    if (flags & 2) {
        PSoundManager->SetEnabled25_009915e0(false);
        PSoundManager->SetEnabled26_00991620(false);
    }

    CWideString file_path = CAFile::ConvertPathToFullPath(file_name.ToWideString());
    file_path.Replace(L".xmv", L".wmv");

    CVideoSys* video = new CVideoSys;
    if (!text_bank && (flags & 8))
        text_bank = &CMainGameComponent::Get()->GetTextBank();
    if (text_bank)
        video->SetTextBank(text_bank);

    long hr = video->Init(file_path, 0, true);
    if (hr >= 0) {

    video->SetSubtitlesEnabled(CUserProfileManager::Get().IsSubtitlingEnabled());
    video->SetVolume(CUserProfileManager::Get().GetMusicVolume());
    video->Play();
    double time_start = GFGetTime();
    C2DExtentsI dimensions(video->TexRenderer->Width, video->TexRenderer->Height);
    CTexture* texture = new CTexture;
    texture->Initialise(dimensions, 1, CPixelFormat(8, 8, 8, 8), 0, SURFACE_POOL_MANAGED, false, false);

    if (GFGetSystemManager()->Update() != 2) {
        do {
            long event_code = 0;
            long lparam1 = 0;
            long lparam2 = 0;
            if (video->MEvent) {
                video->MEvent->GetEvent(&event_code, &lparam1, &lparam2, 0);
                if (event_code == 1)
                    break;
            }
            CInputManager* input_manager = GFGetSystemManager()->GetInputManager();
            input_manager->Update();
            CInputEventScanInfo scan_info;
            CInputEvent event;
            if (GFGetSystemManager()->GetInputManager()->EventScanStart(scan_info, event) == true) {
                do {
                    if (event.GetType() == 1) {
                        if (event.GetKey() == 1 || event.GetKey() == 0x39 || event.GetKey() == 0x1c ||
                            (event.GetKey() == 0x3e && GetAsyncKeyState(0x12) < 0)) {
                            video->Stop();
                            goto finished;
                        }
                    }
                } while (input_manager->EventScanNext(scan_info, event) == true);
            }

            unsigned long wait = WaitForSingleObjectEx(video->GetRenderEvent(), 0x21, 1);
            CRGBColour clear_black(0, 0, 0, 0xff);
            GFGetSystemManager()->GetDisplayManager()->ClearRenderTarget(clear_black, 7);
            GFGetSystemManager()->GetDisplayManager()->BeginRender();
            C2DBoxF whole_area(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
            GFGetSystemManager()->GetDisplayManager()->GetRenderManager().SetVWindow(whole_area);
            CUVBox v;
            v.U[0] = 0.0f; v.V[0] = 1.0f;
            v.U[1] = 1.0f; v.V[1] = 1.0f;
            v.U[2] = 1.0f; v.V[2] = 0.0f;
            v.U[3] = 0.0f; v.V[3] = 0.0f;
            C2DExtentsI renderTargetDim = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
            float border = ((float)renderTargetDim.Y - (float)(video->GetHeight() * renderTargetDim.X / video->GetWidth())) * 0.5f;
            C2DBoxF dst_box(0.0f, border, (float)renderTargetDim.X, (float)renderTargetDim.Y - border);
            C2DBoxF outbox;
            GFABoxToVBox(dst_box, outbox);
            if (wait == 0) {
                EnterCriticalSection(&video->GetCriticalSection());
                texture->CopyFromTexture(*video->GetPRenderTexture(), 0xffffffff);
                LeaveCriticalSection(&video->GetCriticalSection());
            }
            GFGetSystemManager()->GetDisplayManager()->GetRenderManager().Draw2DTexture(outbox, v, CRGBColour(0xff, 0xff, 0xff, 0xff), *texture, 0);
            GFGetSystemManager()->GetDisplayManager()->GetRenderManager().DrawBufferedPolys();
            video->GetMovieSubtitles().Play((float)(GFGetTime() - time_start), video->GetSubtitlesEnabled());
            GFGetSystemManager()->GetDisplayManager()->GetRenderManager().Render2DDrawList(true);
            GFGetSystemManager()->GetDisplayManager()->EndRender();
            GFGetSystemManager()->GetDisplayManager()->SwapScreens();
        } while (GFGetSystemManager()->Update() != 2);
    }
finished:
    delete texture;
    video->Close();
    delete video;

    if ((flags & 0x10) && PSoundManager) {
        PSoundManager->SetGroupVolume(8, 0.0f, 0.0f);
        PSoundManager->SetGroupVolume(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGroupVolume(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
        PSoundManager->SetGroupVolume(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
    }
    if (flags & 2) {
        PSoundManager->SetEnabled25_009915e0(true);
        PSoundManager->SetEnabled26_00991620(true);
    }
    return true;
    }
    if (perror_message)
        perror_message->Sprintf(L"0x%08X", hr);
    video->Close();
    delete video;
    return false;
}