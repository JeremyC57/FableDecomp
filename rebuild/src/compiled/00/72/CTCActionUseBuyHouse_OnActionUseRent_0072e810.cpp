// CTCActionUseBuyHouse::OnActionUseRent(CTCBuyableHouse* house) -- switch to the rent question and show the matching rent prompt (marital home live/rent for a furnished house, rent stop/start otherwise; offered only when the player owns the house and no wife lives there), waiting for the answer when there is text
// Leaf: OnActionUseRent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072E810, 290 bytes (+14 int3 padding), `ret 4`. Called from GlobalUpdate 0x0072EE30.
// Callees: CWideString::CWideString() 0x0099AED0, CCharString(const char*, long) 0x0099EBF0, CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, CDataBank::GetTextBySymbol(const CCharString&) 0x009C95E0 (CWideString by value),
//   CWideString::operator+=(const CWideString&) 0x0099B8D0, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0,
//   CWideString::operator!=(const wchar_t*) const 0x0099B150 (L"" 0x0122D70C), CSubtitleRenderer::SetText(const
//   CFontBank&, const CWideString&, unsigned long, float, bool, bool) 0x00492E4B (landed).
//   Literals TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE 0x01266BC0, _MARITAL_HOME_RENT 0x01266B94, _RENT_STOP 0x01266B70,
//   _RENT_START 0x01266B4C. Globals g_pMainGameComponent 0x013B86A0 (text bank +0x14, font +0x1614C), g_pPlayerGui
//   0x013B8790 (subtitle renderer +0x18).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, Rented +0x12, MaxDressLevel +0x20, WifeLivingHere +0x38);
//   CTCActionUseBuyHouse WaitingForConfirmation +0x18, HouseQuestion +0x1c.
// Shape: four complete `text += bank->GetTextBySymbol("KEY");` statements (an IMPLICIT conversion temporary, re-addressed
//   with `lea`, homed in the dead `house` slot) that VC cross-jumps into one call tail; the second wide string is an unused local built and
//   destroyed with the text; the text bank comes through an inline GetTextBank() accessor (loaded into ecx before the
//   argument `lea`s; the plain member chain loads it last); the prompt goes through the player GUI's subtitle renderer.
#include <stddef.h>
#include "engine/CTCBuyableHouse.h"

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                           // 0x0099AED0
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
    bool operator!=(const wchar_t* text) const;              // 0x0099B150
};
struct CBaseIntelligentPointer {                             // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    void* GetPItem() const;                                  // 0x00A01B50
};
class CFontBank;
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);  // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    PTextBank;                                 // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank*    PFont;                                     // +0x1614C
    CDataBank* GetTextBank() const { return PTextBank; }
};
struct CSubtitleRenderer {
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast,
                 bool pauseGame);                            // 0x00492E4B
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                      // +0x18
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                             // 0x013B8790

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
            text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE");
        else if (house->OwnedByPlayer && !((CBaseIntelligentPointer*)house->WifeLivingHere)->GetPItem())
            text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_RENT");
    } else {
        if (house->Rented)
            text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_STOP");
        else if (house->OwnedByPlayer && !((CBaseIntelligentPointer*)house->WifeLivingHere)->GetPItem())
            text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_START");
    }
    if (text != L"") {
        WaitingForConfirmation = true;
        g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->PFont, text, 4, 0.0f, false, true);
    }
}