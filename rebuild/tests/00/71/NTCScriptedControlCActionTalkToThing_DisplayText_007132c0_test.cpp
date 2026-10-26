// Behaviour test for NTCScriptedControl::CActionTalkToThing::DisplayText (0x007132C0): model and body verbatim, callees stubbed;
// covers the plain subtitle (flags 0x13 / 0x813, duration / fps, background cleared only when the sound has not started),
// the two- and three-button questions, and the temporary lifetimes around each SetText.
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// NTCScriptedControl::CActionTalkToThing::DisplayText() -- show the current line through the player GUI's subtitle renderer: as a two- or three-button question when asking the hero, otherwise as timed subtitle text
//   result passed straight to SetText (retail destroys it BEFORE the button ToWideString temporaries), the font and the gui are
//   __forceinline getters evaluated in the argument list (text ctor, then PeekComponent, then the gui load, before the pushes);
//   in the plain branch the font view is a named local so the font word lands in edx.
// Leaf: ?DisplayText@CActionTalkToThing
// Layout: retail CActionTalkBase is Ego_r.pdb less 4 (CArray<CSubtitledMessage> 12 bytes: Messages +0x30, CurrentMessageIndex +0x3c,
//   GameTurnDuration +0x48, SoundStartedPlaying +0x73); retail CSubtitledMessage is 0x44 (Text +0xc); CActionTalkToThing
//   AskingHeroQuestion +0x91, DisplayOverScreenFade +0x92, QuestionButton1..3 +0x94/+0x98/+0x9c (PDB less 8).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CWideString;
class CWideStringText {                         // the message text as stored (converted by 0x0099B720)
public:
    void* PStringData;
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideStringText& other);  // 0x0099B720
    ~CWideString();                             // 0x0099B510
};
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    CWideString ToWideString() const;           // 0x0099E480
};
class CFontBank;
class CSubtitleRenderer { public:
    void ClearText(bool immediately);           // 0x00492BAB
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
        const CWideString& button2Text, const CWideString& button3Text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492973
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
        const CWideString& button2Text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492F69
    void SetText(CFontBank* font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492E4B
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;         // +0x18
    unsigned char _pad19[0xff - 0x19];
    bool ShowSubtitleBackground;                // +0xff
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
extern long g_FramesPerSecond;                  // 0x01375550
struct CMainGameComponentFontView { unsigned char _prefix[0x1614c]; CFontBank* Font; };   // +0x1614C
class CThing { public:
    CMainGameComponentFontView* PeekComponent();    // 0x004C7A70
};
struct CCreaturePointer { void* __vftable; CThing* PThing; };
class CSubtitledMessage { public:                      // retail 0x44
    unsigned char _pad00[0x0c];
    CWideStringText Text;                       // +0x0c
    unsigned char _pad10[0x44 - 0x10];
};
class CActionTalkToThing {
public:
    void* __vftable;                            // +0x00
    CCreaturePointer* PControl;                 // +0x04
    unsigned char _pad08[0x30 - 0x08];
    CSubtitledMessage* Messages;                // +0x30 (CArray data)
    unsigned char _pad34[0x3c - 0x34];
    long CurrentMessageIndex;                   // +0x3c
    unsigned char _pad40[0x48 - 0x40];
    long GameTurnDuration;                      // +0x48
    unsigned char _pad4c[0x73 - 0x4c];
    bool SoundStartedPlaying;                   // +0x73
    unsigned char _pad74[0x91 - 0x74];
    bool AskingHeroQuestion;                    // +0x91
    bool DisplayOverScreenFade;                 // +0x92
    unsigned char _pad93;
    CCharString QuestionButton1;                // +0x94
    CCharString QuestionButton2;                // +0x98
    CCharString QuestionButton3;                // +0x9c
    __forceinline CWideString GetCurrentText() const { return CWideString(Messages[CurrentMessageIndex].Text); }
    __forceinline CFontBank* GetFont() const { return PControl->PThing->PeekComponent()->Font; }
    __forceinline CPlayerGui* GetPlayerGui() const { return g_pPlayerGui; }
    void DisplayText();
};

void CActionTalkToThing::DisplayText()
{
    g_pPlayerGui->SubtitleRenderer.ClearText(false);
    if (AskingHeroQuestion) {
        if (QuestionButton3 == "") {
            CWideString name(L"");
            GetPlayerGui()->SubtitleRenderer.SetText(GetFont(), GetCurrentText(), name,
                QuestionButton1.ToWideString(), QuestionButton2.ToWideString(), 4, 0.0f, false, true);
        } else {
            CWideString name(L"");
            GetPlayerGui()->SubtitleRenderer.SetText(GetFont(), GetCurrentText(), name,
                QuestionButton1.ToWideString(), QuestionButton2.ToWideString(), QuestionButton3.ToWideString(), 4, 0.0f, false, true);
        }
    } else {
        unsigned long flags = 0x13;
        if (DisplayOverScreenFade)
            flags = 0x813;
        CWideString text(Messages[CurrentMessageIndex].Text);
        CMainGameComponentFontView* view = PControl->PThing->PeekComponent();
        CFontBank* font = view->Font;
        g_pPlayerGui->SubtitleRenderer.SetText(font, text, flags,
            (float)GameTurnDuration / g_FramesPerSecond, false, true);
        if (!SoundStartedPlaying)
            g_pPlayerGui->ShowSubtitleBackground = false;
    }
}

// ---- stubs and harness ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
CPlayerGui* g_pPlayerGui;
long g_FramesPerSecond = 30;
static int g_live = 0, g_dtorAfterSet = 0, g_setCalls = 0, g_clearCalls = 0, g_peekCalls = 0;
static int g_lastKind = 0; static const void* g_lastFont = 0; static void* g_lastText = 0; static void* g_lastName = 0;
static void* g_lastB[3]; static unsigned long g_lastFlags = 0; static float g_lastTime = -1.0f; static bool g_lastClick = true, g_lastPause = false;
static CMainGameComponentFontView g_view; static CFontBank* g_font = (CFontBank*)0x1234; static CPlayerGui g_gui;
static int g_wideOrder[8]; static int g_wideN = 0;
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CWideString::CWideString(const wchar_t* text) { PStringData = (void*)text; ++g_live; }
CWideString::CWideString(const CWideStringText& other) { PStringData = other.PStringData; ++g_live; }
CWideString::~CWideString() { --g_live; if (g_setCalls) ++g_dtorAfterSet; }
CWideString CCharString::ToWideString() const { return CWideString((const wchar_t*)PStringData); }
void CSubtitleRenderer::ClearText(bool immediately) { ++g_clearCalls; CHECK(!immediately); CHECK(this == &g_gui.SubtitleRenderer); }
CMainGameComponentFontView* CThing::PeekComponent() { ++g_peekCalls; return &g_view; }
static void record(CSubtitleRenderer* r, int kind, CFontBank* font, const CWideString& text, unsigned long flags, float time, bool click, bool pause) {
    ++g_setCalls; g_lastKind = kind; g_lastFont = font; g_lastText = text.PStringData; g_lastFlags = flags; g_lastTime = time; g_lastClick = click; g_lastPause = pause;
    CHECK(r == &g_gui.SubtitleRenderer); CHECK(g_peekCalls == g_setCalls);
}
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& b1, const CWideString& b2, const CWideString& b3, unsigned long flags, float time, bool click, bool pause) {
    record(this, 3, font, text, flags, time, click, pause); g_lastName = nameText.PStringData; g_lastB[0] = b1.PStringData; g_lastB[1] = b2.PStringData; g_lastB[2] = b3.PStringData; CHECK(g_live == 5);
}
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& b1, const CWideString& b2, unsigned long flags, float time, bool click, bool pause) {
    record(this, 2, font, text, flags, time, click, pause); g_lastName = nameText.PStringData; g_lastB[0] = b1.PStringData; g_lastB[1] = b2.PStringData; g_lastB[2] = 0; CHECK(g_live == 4);
}
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, unsigned long flags, float time, bool click, bool pause) {
    record(this, 1, font, text, flags, time, click, pause); CHECK(g_live == 1);
}
static CCharStringData* mkstr(const char* s) { CCharStringData* d = new CCharStringData; d->Data.PData = (char*)s; d->Data.StringLength = (unsigned long)strlen(s); d->Data.DataLength = d->Data.StringLength + 1; d->Data.UseFastExtend = false; d->NoRefs = 1; return d; }
int main()
{
    g_pPlayerGui = &g_gui; g_view.Font = g_font;
    CSubtitledMessage msgs[3]; memset(msgs, 0, sizeof(msgs));
    static int payload[3];
    for (int i = 0; i < 3; ++i) msgs[i].Text.PStringData = &payload[i];
    unsigned char raw[sizeof(CActionTalkToThing)]; memset(raw, 0, sizeof(raw));
    CActionTalkToThing* a = (CActionTalkToThing*)raw;
    a->PControl = new CCreaturePointer; a->PControl->PThing = (CThing*)raw;   // PeekComponent is stubbed, any object serves
    a->Messages = msgs; a->CurrentMessageIndex = 2; a->GameTurnDuration = 45;
    // plain subtitle, sound not started: flags 0x13, time 1.5, background cleared
    g_gui.ShowSubtitleBackground = true;
    a->DisplayText();
    CHECK(g_clearCalls == 1 && g_setCalls == 1 && g_lastKind == 1 && g_lastFont == g_font && g_lastText == &payload[2]);
    CHECK(g_lastFlags == 0x13 && g_lastTime == 1.5f && !g_lastClick && g_lastPause && g_live == 0);
    CHECK(!g_gui.ShowSubtitleBackground);
    // over screen fade, sound started: flags 0x813, background kept
    a->DisplayOverScreenFade = true; a->SoundStartedPlaying = true; g_gui.ShowSubtitleBackground = true; a->CurrentMessageIndex = 0;
    a->DisplayText();
    CHECK(g_setCalls == 2 && g_lastKind == 1 && g_lastFlags == 0x813 && g_lastText == &payload[0] && g_gui.ShowSubtitleBackground && g_live == 0);
    // two-button question (button 3 empty)
    a->AskingHeroQuestion = true; a->QuestionButton1.PStringData = mkstr("Yes"); a->QuestionButton2.PStringData = mkstr("No"); a->QuestionButton3.PStringData = 0;
    a->CurrentMessageIndex = 1; g_dtorAfterSet = 0; g_setCalls = 0; g_peekCalls = 0;
    a->DisplayText();
    CHECK(g_setCalls == 1 && g_lastKind == 2 && g_lastText == &payload[1] && g_lastFont == g_font && g_lastFlags == 4 && g_lastTime == 0.0f && !g_lastClick && g_lastPause);
    CHECK(g_lastB[0] == a->QuestionButton1.PStringData && g_lastB[1] == a->QuestionButton2.PStringData && g_lastName != 0 && *(const wchar_t*)g_lastName == 0);
    CHECK(g_dtorAfterSet == 4 && g_live == 0);
    // button 3 set to "" (non-null empty string) still takes the two-button form
    a->QuestionButton3.PStringData = mkstr(""); g_setCalls = 0; g_peekCalls = 0;
    a->DisplayText();
    CHECK(g_setCalls == 1 && g_lastKind == 2 && g_live == 0);
    // three-button question
    a->QuestionButton3.PStringData = mkstr("Maybe"); g_setCalls = 0; g_peekCalls = 0; g_dtorAfterSet = 0;
    a->DisplayText();
    CHECK(g_setCalls == 1 && g_lastKind == 3 && g_lastB[2] == a->QuestionButton3.PStringData && g_lastB[0] == a->QuestionButton1.PStringData && g_lastFlags == 4);
    CHECK(g_dtorAfterSet == 5 && g_live == 0 && g_clearCalls == 5);
    printf("007132c0_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail;
}