// CTCActionUseScriptedHook::OnActionUse(CThing& user) -- mark the hook for performing; when the owner thing carries script data other than "" / "NULL", either ask for confirmation first (ForceConfirmation: wait instead of perform) or show the text straight away, through the game UI's subtitle renderer
// Grade: DIFFER(125v150) -- our two SetText arms cross-jump into one shared tail (push text/font, add ecx,18h, call) where retail keeps two complete calls with their own `ret 4` (arm 1 loads font into eax and the UI before the pushes, arm 2 font into ecx before the pushes and the UI after them) | tried: font expression inline, font named local per arm, explicit return in arm 1, UI pointer local in arm 1 (no change)
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
            CFontBank* font = GetMainGameComponent()->PFontBank;
            CGameUI* ui = g_pGameUI;
            ui->Subtitles.SetText(font, text, 4, 0.0f, false, true);
            return;
        } else {
            CFontBank* font = GetMainGameComponent()->PFontBank;
            g_pGameUI->Subtitles.SetText(font, text, 0x322, 0.0f, false, true);
        }
    }
}
