// NResourceDirectoryNames::GetHDMiscDir() -- the constant data folder L"Data\\Misc\\" as a CWideString
// Leaf: ?GetHDMiscDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A0C0, 17 bytes, plain `ret` (`__fastcall` free function: hidden result in ecx).
// Identity: Ego_r.pdb / FableWin.pdb ?GetHDMiscDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ.
// Order: Ego_r.pdb and FableWin.pdb list the NResourceDirectoryNames getters in one order (GetLevelDir, GetDefDir, GetMiscDir,
// GetHDMiscDir, GetBrushDir, GetGraphicBankDir, GetFontBankDir, GetCacheDir, GetDebugSaveDir, GetMeshBankDir,
// GetShaderBankDir, GetEngineCacheDir, GetWriteableEngineCacheDir, GetBoneConfigDir, GetSoundDir, GetDVDDialogueDir,
// GetHDDialogueDir, GetDVDSoundDir, GetHDSoundDir, GetHDMusicDir, GetDVDMusicDir, GetStaticMapDir, then (FableWin.pdb)
// GetMyDocumentsDir ... GetTattooUserDir, GetTattooDataDir; Ego_r.pdb moves GetTattooDataDir up after GetStaticMapDir);
// retail 0x0041A060-0x0041A6A0 has the same sequence of literals and body sizes, with the landed
// GetWriteableEngineCacheDir 0x0041A220, GetMyDocumentsDir 0x0041A430 and GetSaveDir 0x0041A540 at their places.
// This one: second of the two L"Data\\Misc\\" bodies (0x0041A0A0 is GetMiscDir, already labelled so).
// Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0. The old label (a propagated
// CUserProfileManager::GetAutoSaveFileName / MakeCWideString family name) described only the body.
#include "fable_string.h"

namespace NResourceDirectoryNames {
CWideString __fastcall GetHDMiscDir()
{
    return CWideString(L"Data\\Misc\\");
}
}
