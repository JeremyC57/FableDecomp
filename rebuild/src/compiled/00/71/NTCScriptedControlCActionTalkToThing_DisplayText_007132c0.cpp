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
struct CCharStringData {
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
struct CFontBank;
struct CSubtitleRenderer {
    void ClearText(bool immediately);           // 0x00492BAB
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
        const CWideString& button2Text, const CWideString& button3Text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492973
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
        const CWideString& button2Text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492F69
    void SetText(CFontBank* font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame);   // 0x00492E4B
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;         // +0x18
    unsigned char _pad19[0xff - 0x19];
    bool ShowSubtitleBackground;                // +0xff
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
extern long g_FramesPerSecond;                  // 0x01375550
struct CMainGameComponentFontView { unsigned char _prefix[0x1614c]; CFontBank* Font; };   // +0x1614C
struct CThing {
    CMainGameComponentFontView* PeekComponent();    // 0x004C7A70
};
struct CCreaturePointer { void* __vftable; CThing* PThing; };
struct CSubtitledMessage {                      // retail 0x44
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