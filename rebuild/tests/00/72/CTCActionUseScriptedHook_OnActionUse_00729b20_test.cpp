// Behaviour test for 00729b20: src/00729b20.cpp copied verbatim; CCharString::operator!=, the main game component and
// the subtitle renderer stubbed so the flag updates and the two subtitle requests (confirm colour 4, plain 0x322) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCActionUseScriptedHook::OnActionUse(CThing& user) -- mark the hook for performing; when the owner thing carries script data other than "" / "NULL", either ask for confirmation first (ForceConfirmation: wait instead of perform) or show the text straight away, through the game UI's subtitle renderer
// Retry 10b (getsw sweep, FIXES B-15): the font bank is read through an inline getter `GetFontBank()`; the plain member read gives the parked register/scheduling difference.
// Leaf: ?OnActionUse@CTCActionUseScriptedHook@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729B20, length 150 bytes (+10 int3 padding). Non-virtual (`QAE`); `ret 4`
// (the user is unused).
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 (x2: "" @ 0x0122D70E, "NULL" @ 0x012393E4), the CTCBase main-game-
// component accessor @ 0x00686D60 (catalog "CViewBase::GetDisplayEngine"; its font bank at +0x1614C), CSubtitleRenderer::SetText
// (CFontBank*, const CCharString&, unsigned long, float, bool, bool) @ 0x00492ED1 on the game UI singleton @ 0x013B8790 (+0x18).
// Shape: &ParentThing->ScriptData formed once in edi; PerformAction = true and WaitingForConfirmation = false stored before the
// first compare; the two arms are separate SetText calls (4 with the flags flipped, 0x322 otherwise), each with its own return.
// Layout: PDB CTCActionUseScriptedHook (ForceConfirmation +0xD, WaitingForConfirmation +0xE, PerformAction +0xF); retail CThing
// ScriptData at +0x78 (PDB +0x88, 16 bytes of narrower containers before it).
class CThing;
struct CCharString {
    void* PString;
    bool operator!=(const char* text) const;    // 0x0099E960
};
class CThing {
public:
    unsigned char _pad00[0x78];
    CCharString ScriptData;                     // +0x78
};
struct CFontBank;
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank* PFontBank;                       // +0x1614c
    CFontBank* GetFontBank() const { return PFontBank; }
};
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
struct CGameUI {
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                // +0x18
};
extern CGameUI* g_pGameUI;                      // 0x013B8790

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CMainGameComponent* GetMainGameComponent() const;   // 0x00686D60
};

class CTCActionUseScriptedHook : public CTCBase {
public:
    bool Usable;                                // +0x0c
    bool ForceConfirmation;                     // +0x0d
    bool WaitingForConfirmation;                // +0x0e
    bool PerformAction;                         // +0x0f
    bool TeleportToRegionEntrance;              // +0x10
    void OnActionUse(CThing& user);
};

void CTCActionUseScriptedHook::OnActionUse(CThing&)
{
    CCharString& text = ParentThing->ScriptData;
    PerformAction = true;
    WaitingForConfirmation = false;
    if (text != "" && text != "NULL") {
        if (ForceConfirmation) {
            PerformAction = false;
            WaitingForConfirmation = true;
            CFontBank* font = GetMainGameComponent()->GetFontBank();
            CGameUI* ui = g_pGameUI;
            ui->Subtitles.SetText(font, text, 4, 0.0f, false, true);
        } else {
            g_pGameUI->Subtitles.SetText(GetMainGameComponent()->GetFontBank(), text, 0x322, 0.0f, false, true);
        }
    }
}
// ---- end source ----
bool CCharString::operator!=(const char* text) const { return strcmp((const char*)PString, text) != 0; }
static unsigned char g_mgcRaw[sizeof(CMainGameComponent)];
static CMainGameComponent* g_mgc = (CMainGameComponent*)g_mgcRaw;
CMainGameComponent* CTCBase::GetMainGameComponent() const { return g_mgc; }
CTCBase::~CTCBase() {}
static int g_texts = 0; static CFontBank* g_font = 0; static const CCharString* g_text = 0; static unsigned long g_colour = 0;
static float g_time = -1; static bool g_a = true, g_b = false;
void CSubtitleRenderer::SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b)
{ ++g_texts; g_font = font; g_text = &text; g_colour = colour; g_time = time; g_a = a; g_b = b; }
static CGameUI g_ui;
CGameUI* g_pGameUI = &g_ui;
int main()
{
    g_mgc->PFontBank = (CFontBank*)&g_texts;
    static unsigned char thingRaw[sizeof(CThing)]; CThing* thing = (CThing*)thingRaw;
    static unsigned char hookRaw[sizeof(CTCActionUseScriptedHook)]; CTCActionUseScriptedHook* hook = (CTCActionUseScriptedHook*)hookRaw;
    hook->ParentThing = thing; hook->ForceConfirmation = false;
    thing->ScriptData.PString = (void*)"";
    hook->OnActionUse(*thing);
    CHECK(hook->PerformAction == true && hook->WaitingForConfirmation == false && g_texts == 0);
    thing->ScriptData.PString = (void*)"NULL";
    hook->OnActionUse(*thing); CHECK(g_texts == 0);
    thing->ScriptData.PString = (void*)"Hello";
    hook->OnActionUse(*thing);
    CHECK(g_texts == 1 && g_colour == 0x322 && g_font == (CFontBank*)&g_texts && g_text == &thing->ScriptData && g_time == 0.0f && !g_a && g_b);
    CHECK(hook->PerformAction == true && hook->WaitingForConfirmation == false);
    hook->ForceConfirmation = true;
    hook->OnActionUse(*thing);
    CHECK(g_texts == 2 && g_colour == 4 && hook->PerformAction == false && hook->WaitingForConfirmation == true);
    if (fails == 0) printf("00729b20_TEST PASS\n"); else printf("00729b20_TEST FAIL\n");
    return fails;
}