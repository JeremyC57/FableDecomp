#ifndef FABLE_ENGINE_CNEWFRONTENDGAMECOMPONENT_H
#define FABLE_ENGINE_CNEWFRONTENDGAMECOMPONENT_H
// New header (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay, which expands CProgressDisplay::GetPTextBank).
// Ego_r.pdb CNewFrontendGameComponent (struct_layouts_egor.tsv) on the retail CGameComponent base (engine/CGameComponent.h, 0x10):
// m_pMenuFont +0x10 .. m_pInputManager +0x68 sit at the Ego_r offsets (no container before them); FileNames (STLport vector,
// 0xC in retail, 0x10 in Ego_r) and the rest are opaque, sizeof 0x148 in retail (fable_game.h stand-in). Static instance
// ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A 0x013B871C (reloc binding; private static) read through the
// inline accessors FableWin.pdb ?Get@CNewFrontendGameComponent@@SIPAV1@XZ (0x0187B470), ?GetTextBank@..@@QBEABVCDataBank@NGameText@@XZ
// (0x0187B480) and ?IsTextBankInitialised@..@@QBE_NXZ (0x018B4760), all inline in retail (the landed GetPTextBank 0x00497B30 and
// DrawRetailDisplay 0x0049936B read the instance word and the m_pTextBank object word +0x60 in place).
#include "engine/CGameComponent.h"
#include "engine/CCountedPointer.h"
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#include "engine/CDefPointer.h"
#include "engine/CStopWatch.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#include "stlport/fable_containers.h"
#include "engine/CDateAndTime.h"
#endif
class CEngineInterpolationInfo;
namespace NUISystem { class CFrontEndManager; }
#endif
class CFontBank; class CGraphicDataBank; class CMeshDataBank; class CASoundBank; class CIEngine; class CInputManager;
template <class T> class CDefPointer; class CFrontEndDef;
namespace NGameText { class CDataBank; }
class CNewFrontendGameComponent : public CGameComponent {
private:
    static CNewFrontendGameComponent* m_pNewFrontendGameComponent;       // 0x013B871C
    CCountedPointer<const CFontBank> m_pMenuFont;                        // +0x10
    unsigned char CurrentMovement[4];                                    // +0x18 ECursorMovement
    bool StartedRepeatingMovement;                                       // +0x1C
    double TimeLastMovementHappened;                                     // +0x20
    bool m_bAllowInputs;                                                 // +0x28
    bool m_bCreateGame;                                                  // +0x29
    bool CreateDevFrontEnd;                                              // +0x2A
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
    unsigned char m_bInitMainGame[0x10];                                 // +0x2C CMainGameComponentInit
    CDefPointer<const CFrontEndDef> m_pFrontEndDef;                      // +0x3C
#else
    unsigned char m_bInitMainGame[0x10];                                 // +0x2C CMainGameComponentInit
    unsigned char m_pFrontEndDef[4];                                     // +0x3C CDefPointer<const CFrontEndDef>
#endif
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;            // +0x40
    CCountedPointer<CMeshDataBank> m_pMeshBank;                          // +0x48
    CCountedPointer<CASoundBank> PSampleBank;                            // +0x50
    CCountedPointer<CIEngine> m_pEngine;                                 // +0x58
    CCountedPointer<NGameText::CDataBank> m_pTextBank;                   // +0x60
    CInputManager* m_pInputManager;                                      // +0x68
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): the Ego_r.pdb members after the two STLport vectors, 8 bytes lower than Ego_r (retail vectors 0xC):
    // SaveGame +0x84 (Run 0x0042F43C), m_pFrontEndManager +0xB4, m_fLastUpdateTime +0xB8 / m_fLastRenderTime +0xBC (0x0042EC91),
    // m_attractModeTimer +0x100, m_inputDelayTimer +0x120 (its m_bIsRunning read at +0x138, 0x0042F07F), WindowsMediaPlayerInstalled
    // +0x144 (0x0042EDE2); the AVI members stay opaque.
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
    // NF231 opt-in (row 0x0042EBB6 CNewFrontendGameComponent::Shutdown, needs FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN): the two
    // STLport vectors typed as Ego_r.pdb (struct_layouts_egor.tsv: vector<CWideString,std::allocator<CWideString> > FileNames +0x6C,
    // vector<CDateAndTime,std::allocator<CDateAndTime> > FileTimes) and Ego_d.pdb (field list 0x57888: FileNames type 0x7D2B
    // std::vector<CWideString,std::allocator<CWideString> >, FileTimes type 0x1D9E0 std::vector<CDateAndTime,...>, both private)
    // declare them; 12 bytes each in retail (+0x6C / +0x78, Shutdown 0x0042EC31 `lea ecx,[esi+6Ch]` -> erase 0x00412450, 0x0042EC3E `lea ecx,[esi+78h]` -> erase 0x00411B00).
    // No layout change (same offsets and sizes as the opaque arrays below).
    std::vector<CWideString> FileNames;                                  // +0x6C
    std::vector<CDateAndTime> FileTimes;                                 // +0x78
#else
    unsigned char FileNames[0xc];                                        // +0x6C vector<CWideString>
    unsigned char FileTimes[0xc];                                        // +0x78 vector<CDateAndTime>
#endif
    CWideString SaveGame;                                                // +0x84
    unsigned char _pad_0x88[0xb4 - 0x88];                                // +0x88 PAVITextureShow .. XMVCodeLoaded
    NUISystem::CFrontEndManager* m_pFrontEndManager;                     // +0xB4
    float m_fLastUpdateTime;                                             // +0xB8
    float m_fLastRenderTime;                                             // +0xBC
    double m_fLastRealUpdateTime;                                        // +0xC0
    bool CreatedMarkerFile;                                              // +0xC8
    unsigned char m_LastInterp[0xc];                                     // +0xCC CEngineInterpolationInfoSet
    CStopWatch m_RepeatTimer;                                            // +0xD8
    float m_fRepeatDelay;                                                // +0xF8
    unsigned long m_dwDPadTriggerState;                                  // +0xFC
    CStopWatch m_attractModeTimer;                                       // +0x100
    CStopWatch m_inputDelayTimer;                                        // +0x120
    long LastActiveJoystickDeviceNumber;                                 // +0x140
    bool WindowsMediaPlayerInstalled;                                    // +0x144
    // Ego_r.pdb / FableWin.pdb private members (AAE) this row calls.
    void Init2();                                                        // 0x0042E98F
    void InitialiseEngine();                                             // 0x0042E204
    void Input();                                                        // 0x0042E3EE
    void Update();                                                       // 0x0042DC94
    void UpdateInterpolation(CEngineInterpolationInfo& info);            // 0x0042DBFA
    void Draw(const CEngineInterpolationInfo& info);                     // 0x0042DF9E
    void ReleaseXMVCode();                                               // 0x0042DBD8
    void CleanUpInterfaces();                                            // 0x0042DD28
    void Shutdown();                                                     // 0x0042EBB6
public:
    virtual bool Run(CGameComponent** next_component);                   // 0x0042EC7C ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z
    virtual void ChangeTextureColourDepth(long depth);                   // 0x0042DC5C
#else
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT
    // Opt-in (agentNF47, row 0x0042F75E CNewFrontendGameComponent::Init): Ego_r.pdb XMVCodeLoaded (+0xB9 there, +0xB1 in retail: the two
    // STLport vectors before it are 0xC), the private ?InitialiseDefs@CNewFrontendGameComponent@@AAEXXZ (retail 0x0042F722) and the
    // virtual ?Init@CNewFrontendGameComponent@@UAEXXZ (retail 0x0042F75E, overrides CGameComponent::Init: no vtable layout change).
    unsigned char _pad_0x6c[0xb1 - 0x6c];                               // +0x6C FileNames .. XMVCode (opaque)
    bool XMVCodeLoaded;                                                  // +0xB1
    unsigned char _pad_0xb2[0x148 - 0xb2];                               // +0xB2 .. WindowsMediaPlayerInstalled (opaque)
    void InitialiseDefs();                                               // 0x0042F722
public:
    virtual void Init();                                                 // 0x0042F75E
#else
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETLASTACTIVEJOYSTICKDEVICENUMBER
    // NF229 opt-in (row 0x00599D5C NUISystem::CFrontEndManager::DoPressStart): Ego_r.pdb LastActiveJoystickDeviceNumber (+0x148 there,
    // +0x140 in retail: the two STLport vectors are 0xC, as the RUN opt-in above) and FableWin.pdb's public inline
    // ?GetLastActiveJoystickDeviceNumber@CNewFrontendGameComponent@@QBEJXZ (0x0201D7C8); retail expands it in 0x00599D5C
    // (0x00599E1B `mov eax,[0x013B871C] ; push dword ptr [eax+140h]`). No layout change (the opaque pad split around the member).
    unsigned char _pad_0x6c[0x140 - 0x6c];                               // +0x6C FileNames .. m_inputDelayTimer (opaque)
    long LastActiveJoystickDeviceNumber;                                 // +0x140
    unsigned char _pad_0x144[0x148 - 0x144];                             // +0x144 WindowsMediaPlayerInstalled (opaque)
public:
    long GetLastActiveJoystickDeviceNumber() const { return LastActiveJoystickDeviceNumber; }
#else
    unsigned char _pad_0x6c[0x148 - 0x6c];                               // +0x6C FileNames .. WindowsMediaPlayerInstalled (opaque)
public:
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME
    // NF76 opt-in (row 0x00599E3F NUISystem::CFrontEndManager::Update): Ego_r.pdb ?StartGame@CNewFrontendGameComponent@@QAEXXZ, expanded in
    // that row (`mov eax,[edi] ; mov ecx,edi ; call [eax+10h]` = CGameComponent slot 4 SetQuit, then `mov byte ptr [edi+29h],1`,
    // 0x00599FC6..0x0059A1C9; the body is reconstructed from those bytes), and ?LoadGame@CNewFrontendGameComponent@@QAEXAAVCWideString@@@Z
    // (out of line at 0x00594F36). No layout change.
    void StartGame() { SetQuit(); m_bCreateGame = true; }
    void LoadGame(CWideString& game);                                    // 0x00594F36
#endif
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RESETATTRACTMODETIMER
    // NF230 opt-in (rows 0x00597BF2 GotoPreviousScreen, 0x0059A238 CFrontEndManager::Action): Ego_r.pdb and FableWin.pdb
    // ?ResetAttractModeTimer@CNewFrontendGameComponent@@QAEXXZ (public, non-virtual), out of line at 0x0042DBEE. No layout change.
    void ResetAttractModeTimer();                                        // 0x0042DBEE
#endif
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETPSOUNDBANK
    // NF230 opt-in (row 0x0041D0A8 NUISystem::CManager::PlaySoundW): Ego_d.pdb CNewFrontendGameComponent field list LF_ONEMETHOD
    // `GetPSoundBank`, public, non-const, `CASoundBank* ()` (?GetPSoundBank@CNewFrontendGameComponent@@QAEPAVCASoundBank@@XZ; not in
    // Ego_r / FableWin: inline, no out-of-line copy). Second source: the retail read 0x0041D0A8 `mov eax,[0x013B871C] ... mov eax,[eax+50h]`
    // (PSampleBank's Data word; the body is reconstructed from those bytes with the FableWin CCountedPointer<T>::Get). No layout change.
    CASoundBank* GetPSoundBank() { return PSampleBank.Get(); }
#endif
#ifdef FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
    // NF230 opt-in (row 0x0059A238 CFrontEndManager::Action): FableWin.pdb ?SetQuit@CNewFrontendGameComponent@@UAEXXZ, the PUBLIC
    // override of the protected CGameComponent::SetQuit (?SetQuit@CGameComponent@@MAEXXZ); retail RTTI vtable CNewFrontendGameComponent
    // slot 4 holds 0x00430340 (`mov byte ptr [ecx+8],1 ; ret`), not CGameComponent's 0x00415930. Callers reach it through slot 4
    // (`call [eax+10h]`). Declaration only: an override adds no vtable slot; no layout change.
    virtual void SetQuit();                                              // 0x00430340
#endif
    static CNewFrontendGameComponent* __fastcall Get() { return m_pNewFrontendGameComponent; }
    bool IsTextBankInitialised() const { return !m_pTextBank.IsNull(); }
    const NGameText::CDataBank& GetTextBank() const { return *m_pTextBank; }
};
FABLE_STATIC_ASSERT_AT(89, sizeof(CNewFrontendGameComponent) == 0x148);
#endif
