// CTCActionUseBuyHouse::OnActionUseRent(CTCBuyableHouse* house) -- switch to the rent question and show the matching rent prompt (marital home live/rent for a furnished house, rent stop/start otherwise; offered only when the player owns the house and no wife lives there), waiting for the answer when there is text
// Leaf: OnActionUseRent
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072E810, 290 bytes (+14 int3 padding), `ret 4`. Called from GlobalUpdate 0x0072EE30.
// Callees: CWideString::CWideString() 0x0099AED0, CCharString(const char*, long) 0x0099EBF0, CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, CDataBank::GetTextBySymbol(const CCharString&) 0x009C95E0 (CWideString by value),
//   CWideString::operator+=(const CWideString&) 0x0099B8D0, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0,
//   CWideString::operator!=(const wchar_t*) const 0x0099B150 (L"" 0x0122D70C), CSubtitleRenderer::SetText(const
//   CFontBank&, const CWideString&, unsigned long, float, bool, bool) 0x00492E4B (landed).
//   Literals TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE 0x01266BC0, _MARITAL_HOME_RENT 0x01266B94, _RENT_STOP 0x01266B70,
//   _RENT_START 0x01266B4C. Globals CMainGameComponent::Get() 0x013B86A0 (text bank +0x14, font +0x1614C), (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get())
//   0x013B8790 (subtitle renderer +0x18).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, Rented +0x12, MaxDressLevel +0x20, WifeLivingHere +0x38);
//   CTCActionUseBuyHouse WaitingForConfirmation +0x18, HouseQuestion +0x1c.
// Shape: four complete `text += bank->GetTextBySymbol("KEY");` statements (an IMPLICIT conversion temporary, re-addressed
//   with `lea`, homed in the dead `house` slot) that VC cross-jumps into one call tail; the second wide string is an unused local built and
//   destroyed with the text; the text bank comes through an inline GetTextBank() accessor (loaded into ecx before the
//   argument `lea`s; the plain member chain loads it last); the prompt goes through the player GUI's subtitle renderer.
#include <stddef.h>
#include "engine/CTCBuyableHouse.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                           // 0x0099AED0
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
    bool operator!=(const wchar_t* text) const;              // 0x0099B150
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CFontBank;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;  // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    CDataBank*    PTextBank;                                 // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank*    PFont;                                     // +0x1614C
    CDataBank* GetTextBank() const { return PTextBank; }
};
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast,
                 bool pauseGame);                            // 0x00492E4B
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                      // +0x18
};

enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2 };
class CTCActionUseBuyHouse {
public:
    void*          __vftable;                                // +0x00 (CTCBase)
    void*          ParentThing;                              // +0x04
    unsigned char  _pad08[0x18 - 0x08];                      // CTCBase / CTCActionUseBase rest, PDef +0x14
    bool           WaitingForConfirmation;                   // +0x18
    EHouseQuestion HouseQuestion;                            // +0x1c
    void OnActionUseRent(CTCBuyableHouse* house);
};

void CTCActionUseBuyHouse::OnActionUseRent(CTCBuyableHouse* house)
{
    HouseQuestion = HOUSE_QUESTION_RENT;
    CWideString text;
    CWideString caption;
    if (house->MaxDressLevel > 0) {
        if (house->Rented)
            text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE");
        else if (house->OwnedByPlayer && !((CBaseIntelligentPointer*)house->WifeLivingHere)->GetPItem())
            text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_RENT");
    } else {
        if (house->Rented)
            text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_STOP");
        else if (house->OwnedByPlayer && !((CBaseIntelligentPointer*)house->WifeLivingHere)->GetPItem())
            text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_START");
    }
    if (text != L"") {
        WaitingForConfirmation = true;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(*CMainGameComponent::Get()->PFont, text, 4, 0.0f, false, true);
    }
}