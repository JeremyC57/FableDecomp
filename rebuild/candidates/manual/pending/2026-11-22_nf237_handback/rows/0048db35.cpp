// CUserProfileManager::IsSubtitlingEnabled() const -- the subtitling option of the active profile slot (the new-profile copy when UseNewProfileValues is set)
// Leaf: ?IsSubtitlingEnabled@CUserProfileManager@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DB35, 17 bytes, thiscall `ret` (size-optimised TU: `cmp byte [ecx+0Ch],0`).
// Name (NF237, land1122p27 CODEXP27): FableWin.pdb public ?IsSubtitlingEnabled@CUserProfileManager@@QBE_NXZ; Ego_r.pdb CUserProfileManager
//   records UseNewProfileValues +12 and SubtitlingEnabled +13 (retail +0x0C / +0x0D; the new-profile copy at retail +0xCD); both landed
//   binders (0x00548B50, 0x00548790 option 0x2F) bind this spelling. Was the free-function stand-in
//   ?CUserProfileManager_IsCameraResetting@@YIDPAVCUserProfileManager@@@Z (catalog CUserProfileManager::IsCameraResetting, a different
//   member: FableWin / Ego_r IsCameraResetting is the 0x00548690 getter). In-place respell of the HEAD local model; bytes unchanged.
#pragma optimize("s",on)
class CUserProfileManager { public:
    char pad0[0xc];
    bool UseNewProfileValues;           // +0x0c
    bool SubtitlingEnabled;             // +0x0d
    char pad1[0xcd - 0x0e];
    bool SubtitlingEnabledNewProfile;   // +0xcd
    bool IsSubtitlingEnabled() const;
};

bool CUserProfileManager::IsSubtitlingEnabled() const
{
    if (UseNewProfileValues)
        return SubtitlingEnabledNewProfile;
    return SubtitlingEnabled;
}
