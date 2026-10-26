// CTCActionUseBuyHouse::OnActionUseDress(CTCBuyableHouse* house, long money) -- ask about dressing the house: show the improve text (and wait for the answer) when the hero can afford the next dress level, the lack-of-funds text otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E710, length 221 bytes (+ int3 padding), `ret 8`.
// Callees: CTCBuyableHouse::GetPriceForNextDressLevel @ 0x006BF920, CCharString(const char*, long) @ 0x0099EBF0,
// CDataBank::GetTextBySymbol @ 0x009C95E0 (the text bank, g_pMainGameComponent 0x013B86A0 -> +0x14),
// ~CCharString @ 0x0099EAE0, ConstructString(CWideString*, long) @ 0x009C7B70 (__fastcall), CSubtitleRenderer::SetText
// @ 0x00492E4B (g_pPlayerGui 0x013B8790 -> +0x18, the main game component's font at +0x1614C), ~CWideString @ 0x0099B510.
// Layout = Ego_r.pdb CTCActionUseBuyHouse: PDef +0x14, WaitingForConfirmation +0x18, HouseQuestion +0x1C (3 = dress).
// Shape: no frame -- the symbol string is homed in the dead `money` slot and the wide text in the dead `house` slot; each
// arm builds its own text and calls SetText, and VC7.1 tail-merges the two SetText calls (the arms only push different
// flags, 4 = confirm, 2 = plain), so the shared tail destroys the text once.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    unsigned short* m_pData;
    ~CWideString();                                     // 0x0099B510
};
bool __fastcall ConstructString(CWideString* text, long value);   // 0x009C7B70
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) const;  // 0x009C95E0
};
class CFontBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank* Font;                                    // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time,
                 bool clickPast, bool pauseGame);       // 0x00492E4B
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

class CTCBuyableHouse {
public:
    long GetPriceForNextDressLevel() const;             // 0x006BF920
};
class CTCActionUseBuyHouse {
public:
    virtual void v00();
    unsigned char _base04[0x14 - 4];                    // CTCActionUseBase
    const void* PDef;                                   // +0x14 CDefPointer<CBuyHouseDef const>
    bool  WaitingForConfirmation;                       // +0x18
    long  HouseQuestion;                                // +0x1c EHouseQuestion
    void OnActionUseDress(CTCBuyableHouse* house, long money);
};

void CTCActionUseBuyHouse::OnActionUseDress(CTCBuyableHouse* house, long money)
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