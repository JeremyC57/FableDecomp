#pragma once

#include "rebuild_abi.h"

struct HWND__;
class CPersistContext;

#ifdef FABLE_RETAIL_CIME
// Retail opt-in (chunk 716): NUISystem::CIME with its PDB member names and the members retail expands inline. Evidence:
//   layout Ego_r.pdb CIME (0x54: EnableIME +0, WindowHandle +4, IsInitialised +8, Appearance (IMEUI_APPEARANCE, 0x40) +0xC,
//   PEditBox +0x4C, CancelOnOKClick +0x50; IMEUI_APPEARANCE members from the same PDB), checked against retail Shutdown 0x004051C0
//   (+8 test, +4 WindowHandle, +0x1C symbolFont) and ResetToDefaultSettings 0x00404A90 (stores from gIME+0xC); members:
//   Ego_r.pdb / FableWin.pdb ?Get@CIME@NUISystem@@SIAAV12@XZ (0x00404A80), ?ResetToDefaultSettings@..@@SIXXZ (0x00404A90),
//   ?Shutdown@..@@QAEXXZ (0x004051C0), ?Initialise@..@@QAEXPAUHWND__@@@Z (0x00405650), ?LoadIMESettings@..@@SIXAAVCPersistContext@@@Z
//   (0x00405350), private ?Reset@..@@AAEXXZ; ctor ??0CIME@NUISystem@@QAE@XZ / dtor ??1CIME@NUISystem@@QAE@XZ (FableWin.pdb only:
//   Ego_r and retail expand both). The dynamic initializer of gIME 0x01219E90 is the inline ctor (`call ResetToDefaultSettings`,
//   no member store) and its atexit thunk 0x012288B0 the inline dtor (`mov ecx,offset gIME ; jmp Shutdown`).
class CImeUiFont_Base;
namespace NUISystem { class CEditBox; }
struct IMEUI_APPEARANCE {                                 // ImeUi.h (DXUT ImeUi), 0x40 bytes
    unsigned long    symbolColor;                         // +0x00
    unsigned long    symbolColorOff;                      // +0x04
    unsigned long    symbolColorText;                     // +0x08
    unsigned char    symbolHeight;                        // +0x0C
    unsigned char    symbolTranslucence;                  // +0x0D
    unsigned char    symbolPlacement;                     // +0x0E
    CImeUiFont_Base* symbolFont;                          // +0x10
    unsigned long    candColorBase;                       // +0x14
    unsigned long    candColorBorder;                     // +0x18
    unsigned long    candColorText;                       // +0x1C
    unsigned long    compColorInput;                      // +0x20
    unsigned long    compColorTargetConv;                 // +0x24
    unsigned long    compColorConverted;                  // +0x28
    unsigned long    compColorTargetNotConv;              // +0x2C
    unsigned long    compColorInputErr;                   // +0x30
    unsigned char    compTranslucence;                    // +0x34
    unsigned long    compColorText;                       // +0x38
    unsigned char    caretWidth;                          // +0x3C
    unsigned char    caretYMargin;                        // +0x3D
};
FABLE_STATIC_ASSERT(sizeof(IMEUI_APPEARANCE) == 0x40);
namespace NUISystem
{
    class CIME
    {
    public:
        CIME() { ResetToDefaultSettings(); }
        ~CIME() { Shutdown(); }
        static CIME& FABLE_FASTCALL Get();                                  // 0x00404A80
        static void FABLE_FASTCALL ResetToDefaultSettings();                // 0x00404A90
        static void FABLE_FASTCALL LoadIMESettings(CPersistContext& context);   // 0x00405350
        void Initialise(HWND__* window);                                    // 0x00405650
        void Shutdown();                                                    // 0x004051C0
#ifdef FABLE_RETAIL_CIME_ISIMEACTIVE
        // Retail opt-in (chunk Q21, row 0x00540320 NUISystem::CEditBox::KeyPressed): Ego_r.pdb ?IsIMEActive@CIME@NUISystem@@QBE_NXZ,
        // out of line at 0x00404C50 (called at 0x005403BF on CIME::Get()). Declaration only.
        bool IsIMEActive() const;                                           // 0x00404C50
#endif
    private:
        void Reset();
        bool             EnableIME;                       // +0x00
        HWND__*          WindowHandle;                    // +0x04
        bool             IsInitialised;                   // +0x08
        IMEUI_APPEARANCE Appearance;                      // +0x0C
        CEditBox*        PEditBox;                        // +0x4C
        bool             CancelOnOKClick;                 // +0x50
    };
}
#else
namespace NUISystem
{
    class CIME
    {
    public:
        static void FABLE_FASTCALL LoadIMESettings(
            CPersistContext& context);
        void Initialise(HWND__* window);

    private:
        bool enableIme_;
        fable_u8 padding01_[3];
        HWND__* window_;
        bool initialised_;
        fable_u8 padding09_[3];
        fable_u8 appearance_[0x40];
        void* editBox_;
        bool cancelOnOkClick_;
        fable_u8 padding51_[3];
    };
}

#endif // FABLE_RETAIL_CIME

FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54);
