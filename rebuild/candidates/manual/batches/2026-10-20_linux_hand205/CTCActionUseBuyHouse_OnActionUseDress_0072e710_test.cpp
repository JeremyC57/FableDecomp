// Behaviour test for CTCActionUseBuyHouse::OnActionUseDress (0x0072E710).
#include <cstdio>
#include <cstring>

static const char* g_lastSymbol = 0;
static long g_lastValue = 0;
static unsigned long g_lastFlags = 0;
static const void* g_lastText = 0;
static int g_textAlive = 0;
struct CCharString {
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) {}
    ~CCharString() {}
};
struct CWideString {
    const char* m_pData;
    CWideString(const char* s) : m_pData(s) { ++g_textAlive; }
    CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_textAlive; }
    ~CWideString() { --g_textAlive; }
};
static bool ConstructString(CWideString* text, long value) { g_lastText = text->m_pData; g_lastValue = value; return true; }
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) const { g_lastSymbol = symbol.PStringData; return CWideString(symbol.PStringData); }
};
class CFontBank { public: int dummy; };
struct CMainGameComponent { CDataBank* TextBank; CFontBank* Font; };
static CMainGameComponent* g_pMainGameComponent;
static const CFontBank* g_lastFont = 0;
struct CSubtitleRenderer {
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame)
    {
        g_lastFont = &font; g_lastFlags = flags;
        if (time != 0.0f || clickPast || !pauseGame || text.m_pData != g_lastText) g_lastFlags = 0xdead;
    }
};
struct CPlayerGui { CSubtitleRenderer SubtitleRenderer; };
static CPlayerGui* g_pPlayerGui;
class CTCBuyableHouse { public: long Price; long GetPriceForNextDressLevel() const { return Price; } };
class CTCActionUseBuyHouse {
public:
    const void* PDef; bool WaitingForConfirmation; long HouseQuestion;
    void OnActionUseDress(CTCBuyableHouse* house, long money)
    {
        HouseQuestion = 3;
        long price = house->GetPriceForNextDressLevel();
        if (price != -1 && price <= money) {
            CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_DRESS_IMPROVE");
            ConstructString(&text, price);
            WaitingForConfirmation = true;
            g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->Font, text, 4, 0.0f, false, true);
        } else {
            CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_DRESS_LACK_FUNDS");
            ConstructString(&text, price);
            g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->Font, text, 2, 0.0f, false, true);
        }
    }
};

static bool Run(long price, long money, bool expectAfford)
{
    CDataBank bank; CFontBank font; CMainGameComponent mgc = { &bank, &font }; CPlayerGui gui;
    g_pMainGameComponent = &mgc; g_pPlayerGui = &gui;
    CTCBuyableHouse house; house.Price = price;
    CTCActionUseBuyHouse a; a.PDef = 0; a.WaitingForConfirmation = false; a.HouseQuestion = 0;
    a.OnActionUseDress(&house, money);
    const char* sym = expectAfford ? "TEXT_GUI_BUYABLE_HOUSE_DRESS_IMPROVE" : "TEXT_GUI_BUYABLE_HOUSE_DRESS_LACK_FUNDS";
    return a.HouseQuestion == 3 && a.WaitingForConfirmation == expectAfford && std::strcmp(g_lastSymbol, sym) == 0
        && g_lastValue == price && g_lastFlags == (expectAfford ? 4u : 2u) && g_lastFont == &font && g_textAlive == 0;
}

int main()
{
    if (!Run(100, 150, true) || !Run(100, 100, true) || !Run(200, 150, false) || !Run(-1, 1000, false)) {
        std::printf("0072e710_TEST FAIL\n"); return 1;
    }
    std::printf("0072e710_TEST PASS\n");
    return 0;
}
