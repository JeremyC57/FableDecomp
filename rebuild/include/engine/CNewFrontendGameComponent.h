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
    unsigned char m_bInitMainGame[0x10];                                 // +0x2C CMainGameComponentInit
    unsigned char m_pFrontEndDef[4];                                     // +0x3C CDefPointer<const CFrontEndDef>
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;            // +0x40
    CCountedPointer<CMeshDataBank> m_pMeshBank;                          // +0x48
    CCountedPointer<CASoundBank> PSampleBank;                            // +0x50
    CCountedPointer<CIEngine> m_pEngine;                                 // +0x58
    CCountedPointer<NGameText::CDataBank> m_pTextBank;                   // +0x60
    CInputManager* m_pInputManager;                                      // +0x68
    unsigned char _pad_0x6c[0x148 - 0x6c];                               // +0x6C FileNames .. WindowsMediaPlayerInstalled (opaque)
public:
    static CNewFrontendGameComponent* __fastcall Get() { return m_pNewFrontendGameComponent; }
    bool IsTextBankInitialised() const { return !m_pTextBank.IsNull(); }
    const NGameText::CDataBank& GetTextBank() const { return *m_pTextBank; }
};
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
#endif
