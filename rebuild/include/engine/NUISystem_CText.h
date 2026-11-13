#ifndef FABLE_ENGINE_NUISYSTEM_CTEXT_H
#define FABLE_ENGINE_NUISYSTEM_CTEXT_H
// NUISystem::CText (chunk 867, 2026-11-08): the text component the PC inventory screens write into. RTTI .?AVCText@NUISystem@@ :
// CTextContainer : CChangingStateComponent : CComponent (ghidra_out/rtti_bases.tsv); vtable 0x01249CCC (rebuild/manifest/rtti-vtables.tsv).
// Its own virtuals, in Ego_r.pdb / FableWin.pdb spelling, at the retail slots:
//   142 GetValue 0x0054FF10 (?GetValue@CText@NUISystem@@UBEABVCWideString@@XZ -- the landed inventory rows called it "GetText")
//   143..145 SetValue 0x0054FBC0 / 0x0054F410 / 0x0054F370: VC lays an overload set out in reverse declaration order, so the three
//       declarations below give slot 145 = SetValue(const CCharString&,bool,bool), 144 = SetValue(const CWideString&,bool,bool)
//       (the inventory rows' `call [vtbl+240h]` with a CWideString and two `push 1`; landed rows called it "SetText"), 143 =
//       SetValue(unsigned long,const CWideString&,bool) (NUISystem::CIconText::vfunc143 0x00542140 overrides it).
//   146 GetFont 0x0054FF20, 147 GetAlignment 0x0054FFB0, 148 ForceWidth 0x0054FFE0, 149 SetTextWindow 0x0054FB10, 150 GetTextWindow 0x0054FFF0.
// Only the virtuals are modelled (declarations, no layout): the rows create the component through CManager::CreateComponent
// and cast the CComponent pointer.
#include "engine/NUISystem_CTextContainer.h"
#include "engine/C2DBoxF.h"
class CFontBank;
namespace NUISystem {
enum ETextAlignement;                                            // MS forward enum (?GetAlignment@CText@NUISystem@@UBE?AW4ETextAlignement@2@XZ)
class CText : public CTextContainer {
public:
    virtual const CWideString& GetValue() const;                                   // slot 142 0x0054FF10
    virtual void SetValue(const CCharString& value, bool a, bool b);               // slot 145 0x0054F370
    virtual void SetValue(const CWideString& value, bool a, bool b);               // slot 144 0x0054F410
    virtual void SetValue(unsigned long textId, const CWideString& value, bool a); // slot 143 0x0054FBC0
    virtual CCountedPointer<CFontBank>& GetFont();                                 // slot 146 0x0054FF20
    // Retail order of slots 147..150 (default since agentFAM10, 2026-11-12, queue land1011dl; was the opt-in
    // FABLE_RETAIL_CTEXT_WINDOW_SLOTS of chunk Q21, row 0x00540320 CEditBox::KeyPressed, which rows may still define: it no longer
    // changes anything). Read from the bodies in vtable 0x01249CCC: 147 0x0054FFB0 copies a 16-byte box to +0x178
    // (SetTextWindow(C2DBoxF&), `ret 4`), 148 0x0054FFE0 is `lea eax,[ecx+178h] ; ret` (GetTextWindow), 149 0x0054FB10 measures the
    // text with GetCharWidthW (ForceWidth(float)), 150 0x0054FFF0 returns a value from the +0x12E flag byte (GetAlignment); the
    // catalog names the four addresses the same way, and Ego_r.pdb / FableWin.pdb spell them ?SetTextWindow@CText@NUISystem@@UAEXAAVC2DBoxF@@@Z,
    // ?GetTextWindow@CText@NUISystem@@UAEAAVC2DBoxF@@XZ, ?ForceWidth@CText@NUISystem@@UAEXM@Z, ?GetAlignment@CText@NUISystem@@UBE?AW4ETextAlignement@2@XZ.
    // KeyPressed calls slot 148 and reads the box's BRX (+8) from the result (0x00540564 `call [edx+250h]` ; `fcomp [eax+8]`). The
    // former default (Alignment, ForceWidth, Set/GetTextWindow) put GetTextWindow at 150. Declarations only.
    virtual void SetTextWindow(C2DBoxF& window);                                   // slot 147 0x0054FFB0
    virtual C2DBoxF& GetTextWindow();                                              // slot 148 0x0054FFE0
    virtual void ForceWidth(float width);                                          // slot 149 0x0054FB10
    virtual ETextAlignement GetAlignment() const;                                  // slot 150 0x0054FFF0
#ifdef FABLE_RETAIL_CTEXT_GETCHARWIDTHW
    // Retail opt-in (chunk Q21, row 0x00540320 CEditBox::KeyPressed): Ego_r.pdb ?GetCharWidthW@CText@NUISystem@@SII_W@Z (static
    // __fastcall, landed at 0x0054F4B0; needs /Zc:wchar_t for the _W spelling). Declaration only.
    static unsigned int FABLE_FASTCALL GetCharWidthW(wchar_t c);                   // 0x0054F4B0
#endif
#ifdef FABLE_RETAIL_CTEXT_VSLOTS
// agentFAM04 (2026-11-11): retail CComponent overrides of NUISystem::CText (vtable 0x01249CCC slots 94 / 95 / 111, Ego_r / FableWin
// ?MakeAvailable@CText@NUISystem@@UAEXXZ, ?MakeAvailableDynamically@CText@NUISystem@@UAEXXZ, ?GetDynamicSize@CText@NUISystem@@UBE?AVC2DVector@@XZ)
// and the Ego_r.pdb CText data members 0x30 lower in retail (on CChangingStateComponent's 0x15c; CTextContainer's members not
// declared): Unavailable +0x189 is the byte MakeAvailable 0x0054FF90 clears. Declarations and members only.
    virtual void MakeAvailable();                                   // slot 94 0x0054FF90
    virtual void MakeAvailableDynamically();                        // slot 95 0x0054FFA0
    virtual C2DVector GetDynamicSize() const;                       // slot 111 0x0054FF50
protected:
    CWideString Value;                                              // +0x15c (Ego_r +396)
    CCountedPointer<CFontBank> PFont;                               // +0x160
    unsigned char Handle[8];                                        // +0x168 CEnginePrimitiveHandle
    unsigned char HandleDropShadowBottomRight[8];                   // +0x170 CEnginePrimitiveHandle
    C2DBoxF Window;                                                 // +0x178
    bool ScaleText;                                                 // +0x188
    bool Unavailable;                                               // +0x189
    unsigned char NoRenderNextFrame;                                // +0x18a
public:
#ifdef FABLE_RETAIL_CTEXT_FITINWINDOW
    // Opt-in (row 0x0054FBC0 SetValue): Ego_r.pdb / FableWin.pdb ?FitInWindow@CText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z,
    // protected, out of line (landed 0x0054F8E0). Declaration only.
protected:
    long FitInWindow(CWideString& text, const C2DBoxF& window);    // 0x0054F8E0
public:
#endif
#endif // FABLE_RETAIL_CTEXT_VSLOTS
};
}
#endif // FABLE_ENGINE_NUISYSTEM_CTEXT_H
