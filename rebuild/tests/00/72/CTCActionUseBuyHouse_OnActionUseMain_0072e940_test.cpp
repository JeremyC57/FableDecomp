// Behaviour test for 0x0072E940 CTCActionUseBuyHouse::OnActionUseMain: tutorial short-circuit, buy / cannot-afford /
// sell texts (marital variants when the house has dress levels) and the confirm flag.
#include <cstdio>
#include <cstring>

static char g_last[64]; static unsigned long g_flags; static int g_calls;
class CThing { public: bool Dead; bool hasHero; bool tutorial; };
class CTCBuyableHouse { public: bool OwnedByPlayer; long MaxDressLevel; long price; long GetPrice() { return price; } };
class CWideString { public:
    char buf[64];
    CWideString() { buf[0] = 0; }
    CWideString(const char* s) { std::strcpy(buf, s); }
    const CWideString& operator+=(const CWideString& o) { std::strcat(buf, o.buf); return *this; }
    bool operator!=(const char* s) const { return std::strcmp(buf, s) != 0; }
};
static CWideString GetTextBySymbol(const char* s) { return CWideString(s); }
static void SetText(const CWideString& t, unsigned long flags) { std::strcpy(g_last, t.buf); g_flags = flags; ++g_calls; }
struct Action {
    bool WaitingForConfirmation; long HouseQuestion;
    void OnActionUseMain(CThing* hero, CTCBuyableHouse* house, long money)
    {
        if (hero && !hero->Dead) {
            if (hero->hasHero && hero->tutorial)
                return;
        }
        HouseQuestion = 1;
        CWideString text;
        bool confirm = true;
        if (!house->OwnedByPlayer) {
            long price = house->GetPrice();
            if (price != -1 && price <= money) {
                if (house->MaxDressLevel > 0) text += GetTextBySymbol("MBUY"); else text += GetTextBySymbol("BUY");
            } else {
                if (house->MaxDressLevel > 0) text += GetTextBySymbol("MNO"); else text += GetTextBySymbol("NO");
                confirm = false;
            }
        } else {
            if (house->MaxDressLevel > 0) text = GetTextBySymbol("MSELL"); else text = GetTextBySymbol("SELL");
        }
        if (text != "") {
            if (confirm) { WaitingForConfirmation = true; SetText(text, 4); }
            else SetText(text, 2);
        }
    }
};
int main()
{
    CThing hero = { false, true, true };
    CTCBuyableHouse h = { false, 0, 100 };
    Action a = { false, 0 };
    a.OnActionUseMain(&hero, &h, 500);
    bool ok = g_calls == 0 && a.HouseQuestion == 0;
    hero.tutorial = false;
    a.OnActionUseMain(&hero, &h, 500);
    ok = ok && g_calls == 1 && std::strcmp(g_last, "BUY") == 0 && g_flags == 4 && a.WaitingForConfirmation && a.HouseQuestion == 1;
    a.WaitingForConfirmation = false; h.MaxDressLevel = 2;
    a.OnActionUseMain(0, &h, 50);
    ok = ok && std::strcmp(g_last, "MNO") == 0 && g_flags == 2 && !a.WaitingForConfirmation;
    h.OwnedByPlayer = true; h.MaxDressLevel = 0;
    a.OnActionUseMain(0, &h, 0);
    ok = ok && std::strcmp(g_last, "SELL") == 0 && g_flags == 4;
    if (!ok) { std::printf("0072e940_TEST FAIL\n"); return 1; }
    std::printf("0072e940_TEST PASS\n");
    return 0;
}