// CGameScriptInterface::PlayAVIMovie(const CCharString& movie) const -- script command: play a full-screen movie, subtitled when the profile asks for it
// Leaf: ?PlayAVIMovie@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F890, 63 bytes.
// Identity: Ego_r.pdb 0x008CC5D0 + FableWin.pdb 0x02AAEF90 ?PlayAVIMovie@CGameScriptInterface@@UBEXABVCCharString@@@Z, slot 369 of
//   engine/CGameScriptInterface.h (vtable 0x01260F0C).
// Callees: CUserProfileManager::Get() 0x0040D2A0 and the inline FableWin.pdb IsSubtitlingEnabled() (UseNewProfileValues +0x0C selects
//   SubtitlingEnabledNewProfile +0xCD or SubtitlingEnabled +0x0D), then GFPlayFullScreenXBoxMovie 0x006286F0 (Ego_r.pdb 0x0054E5E0 +
//   FableWin.pdb ?GFPlayFullScreenXBoxMovie@@YI_NABVCCharString@@KKKKPBVCDataBank@NGameText@@PAVCIEngine@@_NMP6I_NXZPAVCXMVPlayerBuffers@@PAVCWideString@@@Z)
//   with flags 0x1B, no text bank / engine / callback / buffers / title, and a -1 duration.
// Name fix (agentNF55, 2026-11-17): re-authored on the shared headers. It was the free stand-in
//   CGameScriptInterface_PlayAVIMovie(void*, void*, void*) over a MidObj overlay, calling a made-up PlayMovie(void*, int, ...).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SUBTITLING
#include "engine/CGameScriptInterface.h"
#include "engine/CUserProfileManager.h"
namespace NGameText { class CDataBank; }                             // engine/NGameText_CDataBank.h (pointer parameter only)
class CIEngine;
class CXMVPlayerBuffers;
class CWideString;
bool __fastcall GFPlayFullScreenXBoxMovie(const CCharString& name, unsigned long flags, unsigned long a = 0, unsigned long b = 0,
                                          unsigned long c = 0, const NGameText::CDataBank* bank = 0, CIEngine* engine = 0,
                                          bool subtitles = true, float duration = -1.0f, bool (__fastcall* skip)() = 0,
                                          CXMVPlayerBuffers* buffers = 0, CWideString* title = 0);   // 0x006286F0

void CGameScriptInterface::PlayAVIMovie(const CCharString& movie) const
{
    GFPlayFullScreenXBoxMovie(movie, 0x1b, 0, 0, 0, 0, 0, CUserProfileManager::Get().IsSubtitlingEnabled(), -1.0f, 0, 0, 0);
}
