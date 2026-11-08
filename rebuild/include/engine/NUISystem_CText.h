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
    virtual ETextAlignement GetAlignment() const;                                  // slot 147 0x0054FFB0
    virtual void ForceWidth(float width);                                          // slot 148 0x0054FFE0
    virtual void SetTextWindow(C2DBoxF& window);                                   // slot 149 0x0054FB10
    virtual C2DBoxF& GetTextWindow();                                              // slot 150 0x0054FFF0
};
}
#endif // FABLE_ENGINE_NUISYSTEM_CTEXT_H
