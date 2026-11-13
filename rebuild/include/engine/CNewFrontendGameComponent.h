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
    unsigned char FileNames[0xc];                                        // +0x6C vector<CWideString>
    unsigned char FileTimes[0xc];                                        // +0x78 vector<CDateAndTime>
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
    unsigned char _pad_0x6c[0x148 - 0x6c];                               // +0x6C FileNames .. WindowsMediaPlayerInstalled (opaque)
public:
#endif
    static CNewFrontendGameComponent* __fastcall Get() { return m_pNewFrontendGameComponent; }
    bool IsTextBankInitialised() const { return !m_pTextBank.IsNull(); }
    const NGameText::CDataBank& GetTextBank() const { return *m_pTextBank; }
};
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
#endif
