#ifndef FABLE_ENGINE_CUSERPROFILEMANAGER_H
#define FABLE_ENGINE_CUSERPROFILEMANAGER_H
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME
#include "fable_string.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#include "fable_string.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#include "fable_string.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
#include "engine/CActionInputControl.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES
#include "engine/CDisplayModeDesc.h"
#include "stlport/fable_containers.h"
#endif
// CUserProfileManager (new, chunk 765): only the two methods row 0x006C9FD0 calls are modelled. Ego_r.pdb and FableWin.pdb
// both spell the singleton accessor ?Get@CUserProfileManager@@SIAAV1@XZ (static __fastcall, returns a reference; retail
// 0x0040D2A0 lazily news 0x140 bytes into 0x013B7D4C); ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ = retail 0x0040BB30.
// Landed rows (e.g. NUISystemCSlider_GetValueFromProfile_0054bbb0.cpp) declare the same Get locally.
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();                         // 0x0040D2A0
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VIRTUAL_DTOR
    // Retail opt-in (agentFAM10, 2026-11-12, row 0x0040D830 CCountedPointer<CUserProfileManager>::DeleteData): the class is polymorphic
    // (RTTI vtable .?AVCUserProfileManager@@ stored by the ctor 0x0040CEC0) and vtable slot 0 is its scalar deleting destructor (the
    // deleter's `push 1; call [eax]`). Declaration only, for a row that models no members: the VOLUME / SCOREBOARD layout opt-ins
    // already count the vptr in their leading pad, so do not combine them with this one.
    virtual ~CUserProfileManager();
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_UIBACKGROUND
    // Retail opt-in (Codex pass19): both PDBs and donor01885a90/native004075c0.
    void RenderUIBackground(float);
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME
    // agentFAM09 opt-in (row 0x01219EC0, its dynamic initializer): the profile file name, Ego_r.pdb / FableWin.pdb `dynamic
    // initializer for 'CUserProfileManager::m_strProfileName'', L"Profile.bin" at 0x013B7D54; private static per Anniv360 Test PDB
    // (third source) ?m_strProfileName@CUserProfileManager@@0VCWideString@@A. Declaration only: no layout change.
private:
    static CWideString m_strProfileName;                                  // 0x013B7D54
public:
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
    // Retail opt-in (chunk F2, row 0x00406AE0): Ego_r.pdb / FableWin.pdb ?GetManualSaveFileName@CUserProfileManager@@QBE?AVCWideString@@J@Z
    // (0x00406610) and ?GetManualSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@J@Z (0x00406AE0). Declarations only.
    CWideString GetManualSaveFileName(long slot) const;                  // 0x00406610
    CWideString GetManualSaveDisplayName(long slot) const;               // 0x00406AE0
#endif
    void ProcessPendingTutorials();                                      // 0x0040BB30
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
    // Nested opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): the option setters the UI actions call, out of line in
    // retail, FableWin.pdb / Ego_r.pdb spellings (?SetGamma@CUserProfileManager@@QAEXM_N@Z ...; the second bool is "use the new
    // profile's copy", false from every UI action). Declarations only.
    void SetSubtitlingEnabled(bool, bool);                               // 0x0040AF90
    void SetVibrationEnabled(bool, bool);                                // 0x0040AFD0
    void SetCameraXYInverted(bool, bool);                                // 0x0040B010
    void SetCameraYZInverted(bool, bool);                                // 0x0040B050
    void SetTutorialSystemEnabled(bool, bool);                           // 0x0040B210
    void SetGuildMasterEnabled(bool, bool);                              // 0x0040B250
    void SetHUDTooltipEnabled(bool, bool);                               // 0x0040B290
    void SetGamma(float, bool);                                          // 0x0040B2D0
    void SetHUDAlpha(float, bool);                                       // 0x0040B310
    void SetMusicVolume(float, bool);                                    // 0x0040B350
    void SetSoundVolume(float, bool);                                    // 0x0040B390
    void SetDialogueVolume(float, bool);                                 // 0x0040B3D0
    void SetUsingHeadPhones(bool, bool);                                 // 0x0040B410
    void SetShowingBuddyNames(bool, bool);                               // 0x0040B450
    void SetShowTargetingStatus(bool, bool);                             // 0x0040B490
    void SetAntialiasing(unsigned long, bool);                           // 0x0040B4D0
    void SetCameraResetting(bool, bool);                                 // 0x0040B510
    void SetCameraSensitivity(float, bool);                              // 0x0040B550
    void SetShadowDetail(float, bool);                                   // 0x0040B590
    void SetTextureDetail(float, bool);                                  // 0x0040B5D0
    void SetMeshDetail(float, bool);                                     // 0x0040B610
    void SetEffectsDetail(float, bool);                                  // 0x0040B650
    void SetResolution(unsigned long, bool);                             // 0x0040B690
    void SetRefreshRate(unsigned long, bool);                            // 0x0040B6D0
    void SetScreenAspectRatio(unsigned long, bool);                      // 0x0040B710
    void SetVerticalSyncEnabled(bool, bool);                             // 0x0040B750
    void SetCameraBowYZInverted(bool, bool);                             // 0x0040D2F0
    void Save();                                                         // 0x0040A740
    void Apply();                                                        // 0x0040BC80
    void SetDefaultValuesGameplay();                                     // 0x00408360 (RESTORE_DEFAULTS_GAMEPLAY 0x004210DF)
    void SetDefaultValuesAudio();                                        // 0x004083B0 (RESTORE_DEFAULTS_AUDIO 0x0042116A)
    // FableWin.pdb ?SetNewProfileToUseWASD@CUserProfileManager@@QAEX_N@Z and ?WereVideoChangesAccepted@CUserProfileManager@@QBE_NXZ,
    // inline in retail (Action writes the byte +0x78 in place, 0x004212B3, and tests the byte +0x13C, 0x0042151E). The members
    // (Ego_r.pdb NewProfileToUseWASD +0x80 / VideoChangesAccepted +0x140, retail -8 / -4 after the 12-byte STLport vectors) stay
    // opaque in this opt-in, so the accessors read their bytes.
    void SetNewProfileToUseWASD(bool use) { reinterpret_cast<bool*>(this)[0x78] = use; }
    bool WereVideoChangesAccepted() const { return reinterpret_cast<const bool*>(this)[0x13c]; }
    void SetDefaultValuesVideo();                                        // 0x004093E0
    void CaptureBackBufferForUIBackground();                             // 0x00406CC0
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYDIALOG
    // agentQ7 (2026-11-11, row 0x0047E9AE CTCInGameMenu::GlobalUpdate): FableWin.pdb ?SetDisplayDialogOnOptionsChanged@CUserProfileManager@@QAEX_N@Z,
    // inline in retail (the row writes the byte +0x34 in place twice around Apply / Save). The member (Ego_r.pdb DisplayDialogOnOptionsChanged
    // +0x34, before the first STLport vector, so the same offset in retail) stays opaque here, as in the SETTERS block's accessors.
    void SetDisplayDialogOnOptionsChanged(bool display) { reinterpret_cast<bool*>(this)[0x34] = display; }
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES
    // Opt-in (chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): Ego_r.pdb
    // ?GetDisplayModes@CUserProfileManager@@QBEABV?$vector@VCDisplayModeDesc@@V?$allocator@VCDisplayModeDesc@@@std@@@std@@XZ, out of
    // line in retail at 0x00406880 (`lea eax,[ecx+80h] ; ret`: DisplayModes, Ego_r.pdb +0x88 less two 4-byte-smaller STLport vectors).
    const std::vector<CDisplayModeDesc>& GetDisplayModes() const;        // 0x00406880
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
    // Retail opt-in (chunk 795, row 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate): the profile volumes and their Ego_r.pdb accessors
    // ?GetSoundVolume@CUserProfileManager@@QBEMXZ / ?GetDialogueVolume@CUserProfileManager@@QBEMXZ (out of line at 0x0047CFC2 /
    // 0x0047CFD3 for the slider TU, expanded in place here: `cmp byte [eax+0Ch] ; mov [eax+0E4h] / [eax+24h]`, 0x005E7980).
    // Retail offsets = Ego_r.pdb before the first vector (UseNewProfileValues +0x0C, SoundVolume +0x24, MusicVolume +0x28,
    // DialogueVolume +0x2C) and Ego_r.pdb - 4 after it (SoundVolumeNewProfile +0xE4, MusicVolumeNewProfile +0xE8,
    // DialogueVolumeNewProfile +0xEC). Combinable with FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD.
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }
    unsigned char _pad_vol_0x00[0xc];                                    // vptr, CBaseClass, ProfileName
    bool UseNewProfileValues;                                            // +0x0c
    unsigned char _pad_vol_0x0d[0x24 - 0xd];                             // SubtitlingEnabled .. HUDAlpha
    float SoundVolume;                                                   // +0x24
    float MusicVolume;                                                   // +0x28
    float DialogueVolume;                                                // +0x2c
    unsigned char _pad_vol_0x30[0xe4 - 0x30];                            // UsingHeadPhones .. HUDAlphaNewProfile
    float SoundVolumeNewProfile;                                         // +0xe4
    float MusicVolumeNewProfile;                                         // +0xe8
    float DialogueVolumeNewProfile;                                      // +0xec
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SETSOUNDASMUTED
    // agentNF29 name-fix opt-in (row 0x00408340 itself): Ego_r.pdb 0x004062F0 + FableWin.pdb 0x0187E2D0
    // ?SetSoundAsMuted@CUserProfileManager@@QAEX_N@Z (public, bool). Declaration only: no layout change.
    void SetSoundAsMuted(bool muted);                                    // 0x00408340
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
    // Retail opt-in (chunk F1, row 0x00407CD0): FableWin.pdb / Ego_r.pdb ?RedefineControlPassive@CUserProfileManager@@QAEXABVCActionInputControl@@0J@Z
    // (0x00407CD0), ?ApplyValues@CUserProfileManager@@IAEXXZ (0x00407370, landed; PROTECTED in both PDBs) and the Ego_r.pdb member AssignedInputsPassive
    // (vector<CActionInputControl>, +0x54 in both; retail reads its start / finish at [this+54h] / [this+58h]). Not combinable with the
    // other layout blocks (own pad).
protected:
    void ApplyValues();                                                  // 0x00407370 (IAE: protected)
public:
    void RedefineControlPassive(const CActionInputControl& new_control,const CActionInputControl& old_control,long index);   // 0x00407CD0
    unsigned char _pad_pi_0x00[0x54];                                    // vptr, CBaseClass, ProfileName .. the option values
    std::vector<CActionInputControl> AssignedInputsPassive;              // +0x54
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_TARGETINGSTATUS
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the targeting-status pair the row
    // selects by UseNewProfileValues (`cmp byte [eax+0Ch],0 ; mov al,[eax+134h] / [eax+6Ch]`): Ego_r.pdb ShowTargetingStatus (+0x74
    // there, after two 16-byte vectors that are 12 in retail: +0x6C) and ShowTargetingStatusNewProfile (+0x138 there; retail +0x134,
    // the VOLUME block's -4 rule for the new-profile members). Not combinable with the VOLUME layout block above (own pads).
    unsigned char _pad_ts_0x00[0xc];                                     // vptr, CBaseClass, ProfileName
    bool UseNewProfileValues;                                            // +0x0c
    unsigned char _pad_ts_0x0d[0x6c - 0xd];                              // SubtitlingEnabled .. AssignedInputsAggressive
    bool ShowTargetingStatus;                                            // +0x6c
    unsigned char _pad_ts_0x6d[0x134 - 0x6d];                            // EffectsDetail .. CameraSensitivityNewProfile
    bool ShowTargetingStatusNewProfile;                                  // +0x134
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_AUDIODEFAULTS
    // agentNF28 repair opt-in (row 0x004083B0 ?SetDefaultValuesAudio@CUserProfileManager@@QAEXXZ, Ego_r.pdb 0x00406350 / FableWin.pdb
    // 0x01881DC0; declared in the SETTERS block): the audio option members the row writes, Ego_r.pdb names and offsets (before the
    // first STLport vector, so the same offsets in retail: SoundVolume +0x24 .. DialogueVolume +0x2C, UsingHeadPhones +0x30,
    // SoundsMuted +0x31; retail 0x004083B2..0x004083C9), and the protected ?ApplyValues@CUserProfileManager@@IAEXXZ (0x00407370)
    // the row tail-calls. Not combinable with the other layout blocks (own pad).
#ifndef FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS
protected:
    void ApplyValues();                                                  // 0x00407370 (IAE: protected)
public:
#endif
    unsigned char _pad_ad_0x00[0x24];                                    // vptr, CBaseClass, ProfileName .. HUDAlpha
    float SoundVolume;                                                   // +0x24
    float MusicVolume;                                                   // +0x28
    float DialogueVolume;                                                // +0x2c
    bool UsingHeadPhones;                                                // +0x30
    bool SoundsMuted;                                                    // +0x31
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
    // Nested opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest): the scoreboard settings and
    // their FableWin.pdb accessors (?GetScoreboardUserName@CUserProfileManager@@QAE?AVCWideString@@XZ, ..ProxyIp.., ?GetScoreboardProxyPort
    // @CUserProfileManager@@QAEJXZ, ?GetScoreboardServerPort@..@QAEJXZ; inline in retail, which copies the CWideString member with the
    // copy constructor 0x0099B720 and reads the ports in place). Retail offsets = Ego_r.pdb - 4 (STLport vectors 12, the deque-backed
    // queue 0x28): ScoreBoardUserName +0x110 (0x0055F8A8), ScoreBoardProxyIp +0x118 (0x0055F759), ScoreBoardProxyPort +0x11C
    // (0x0055F7B9), ScoreBoardServerPort +0x120 (0x0055F830). The members before them stay opaque.
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardPassword() { return ScoreBoardPassword; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
    unsigned char _pad_0[0x110 - 0xf0];                                  // UsingHeadPhonesNewProfile .. StereoReversedNewProfile
#else
    unsigned char _pad_0[0x110];                                         // ProfileName .. StereoReversedNewProfile
#endif
    CWideString ScoreBoardUserName;                                      // +0x110
    CWideString ScoreBoardPassword;                                      // +0x114
    CWideString ScoreBoardProxyIp;                                       // +0x118
    long ScoreBoardProxyPort;                                            // +0x11c
    long ScoreBoardServerPort;                                           // +0x120
#endif
};
#endif
