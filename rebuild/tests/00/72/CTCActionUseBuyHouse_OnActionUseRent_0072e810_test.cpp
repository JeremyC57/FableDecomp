// Behaviour test for CTCActionUseBuyHouse::OnActionUseRent (0x0072E810): which prompt is shown.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* key; CCharString(const char* t, long = -1) : key(t) {} };
class CWideString { public:
    char text[96];
    CWideString() { text[0] = 0; }
    const CWideString& operator+=(const CWideString& o) { std::strcat(text, o.text); return *this; }
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
struct CBaseIntelligentPointer { void* m_pItem; void* GetPItem() const { return m_pItem; } };
class CTCBuyableHouse { public: bool OwnedByPlayer; bool Rented; long MaxDressLevel; CBaseIntelligentPointer WifeLivingHere; };
class CFontBank { public: int id; };
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& s) { CWideString w; std::strcpy(w.text, s.key); return w; }
};
class CMainGameComponent { public: CDataBank* PTextBank; CFontBank* PFont; CDataBank* GetTextBank() const { return PTextBank; } };
static int g_shown = 0; static char g_text[96]; static const CFontBank* g_font = 0; static unsigned long g_flags = 0;
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float, bool, bool)
    { ++g_shown; std::strcpy(g_text, text.text); g_font = &font; g_flags = flags; }
};
class CPlayerGui { public: CSubtitleRenderer SubtitleRenderer; };
static CMainGameComponent* g_pMainGameComponent;
static CPlayerGui* g_pPlayerGui;
class CTCActionUseBuyHouse {
public:
    bool WaitingForConfirmation;
    long HouseQuestion;
    void OnActionUseRent(CTCBuyableHouse* house)
    {
        HouseQuestion = 2;
        CWideString text;
        CWideString caption;
        if (house->MaxDressLevel > 0) {
            if (house->Rented)
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE");
            else if (house->OwnedByPlayer && !house->WifeLivingHere.GetPItem())
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_RENT");
        } else {
            if (house->Rented)
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_STOP");
            else if (house->OwnedByPlayer && !house->WifeLivingHere.GetPItem())
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_RENT_START");
        }
        if (text != "") {
            WaitingForConfirmation = true;
            g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->PFont, text, 4, 0.0f, false, true);
        }
    }
};

int main()
{
    int ok = 1;
    CDataBank bank; CFontBank font; CMainGameComponent game; game.PTextBank = &bank; game.PFont = &font;
    CPlayerGui gui; g_pMainGameComponent = &game; g_pPlayerGui = &gui;
    CTCBuyableHouse h; h.OwnedByPlayer = true; h.Rented = false; h.MaxDressLevel = 0; h.WifeLivingHere.m_pItem = 0;
    CTCActionUseBuyHouse use; use.WaitingForConfirmation = false; use.HouseQuestion = 1;
    use.OnActionUseRent(&h);
    if (use.HouseQuestion != 2 || !use.WaitingForConfirmation || std::strcmp(g_text, "TEXT_GUI_BUYABLE_HOUSE_RENT_START") != 0) ok = 0;
    if (g_font != &font || g_flags != 4) ok = 0;
    h.Rented = true; use.OnActionUseRent(&h);
    if (std::strcmp(g_text, "TEXT_GUI_BUYABLE_HOUSE_RENT_STOP") != 0) ok = 0;
    h.MaxDressLevel = 3; use.OnActionUseRent(&h);
    if (std::strcmp(g_text, "TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_LIVE") != 0) ok = 0;
    h.Rented = false; use.OnActionUseRent(&h);
    if (std::strcmp(g_text, "TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_RENT") != 0 || g_shown != 4) ok = 0;
    int wife = 1; h.WifeLivingHere.m_pItem = &wife; use.WaitingForConfirmation = false;
    use.OnActionUseRent(&h);                                  // wife at home: nothing to offer
    if (g_shown != 4 || use.WaitingForConfirmation) ok = 0;
    if (!ok) { std::printf("0072e810_TEST FAIL\n"); return 1; }
    std::printf("0072e810_TEST PASS\n");
    return 0;
}