// CTCFireheartMinigame::DelayedQuit() -- once only: play the selection-change sound, put up the "quit?" subtitle prompt (QuitText with YesText/NoText buttons) in the player GUI's subtitle renderer, and queue game event 9 with a true payload byte
// Leaf: DelayedQuit
// Flags: /GS
//
// Retail 0x007E7AC0, 320 bytes incl. padding (plain `ret`, /GS cookie 0x0139C8A8). Ego_r.pdb
// `?DelayedQuit@CTCFireheartMinigame@@...` (catalog row).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 (literal 0x0125603C
// "TAVERN_GAME_OBJECT_SELECTION_CHANGE"), CTCFireheartMinigame::PlaySound(const CCharString&) @ 0x007E7880 (label
// "CTCBetting::PlaySoundW" is a byte-twin propagation), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 (empty wide
// literal 0x0122D70C; label "CCharString::AssignFromWide" propagated), CTCFireheartMinigame::GetText(ulong) const @ 0x007E7580
// (three times, hidden result; label "CTCBetting::GetText" propagated), CSubtitleRenderer::SetText(CFontBank*, 4 x const
// CWideString&, ulong, float, bool, bool) @ 0x00492F69 (landed), ~CWideString @ 0x0099B510 (four), CMemoryBuffer::Copy @
// 0x009F1760, CMainGameComponent::AddGameEvent @ 0x0041649C, __security_check_cookie @ 0x00BFE9F9. Globals:
// g_pMainGameComponent 0x013B86A0 (font bank +0x1614C), g_pPlayerGui 0x013B8790 (SubtitleRenderer +0x18).
// Layout: retail CTCFireheartMinigame QuitWithDelay +0x88, PDef +0x98 (PDB +0x9C, the Handles vector 12 bytes);
// CFireheartMinigameDef QuitText +0x30, YesText +0x34, NoText +0x38.
// Shape: the sound name, the prompt and the event live in three sibling blocks (the frame slots are shared); the def, the font bank and the GUI are locals taken before the argument pushes (ebp/ebx/edi); the GetText
// temporaries are built right to left and destroyed after the call; the event is the landed CGameEvent model with an
// inline by-value AddBOOL (the payload byte homed in the frame).
#include <stddef.h>

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                       // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
};
class CFontBank;
class CSubtitleRenderer {
public:
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, unsigned long flags, float time, bool b1, bool b2);   // 0x00492F69
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);      // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                     // +0x00
    signed char   Player;                                   // +0x04
    unsigned char Data[32];                                 // +0x05
    unsigned char EndPos;                                   // +0x25
    bool          Valid;                                    // +0x26
    bool          Replacement;                              // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBOOL(bool value) { Copy(1, &value); }
};
class CMainGameComponent { public:
    unsigned char _pad[0x1614c];
    CFontBank* Font;                                        // +0x1614c
    void AddGameEvent(const CGameEvent& event);             // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
class CFireheartMinigameDef { public:
    unsigned char _pad00[0x30];
    unsigned long QuitText;                                 // +0x30
    unsigned long YesText;                                  // +0x34
    unsigned long NoText;                                   // +0x38
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCFireheartMinigame {
public:
    virtual void v00();
    unsigned char _pad04[0x88 - 0x04];
    bool QuitWithDelay;                                     // +0x88
    unsigned char _pad89[0x98 - 0x89];
    CDefPointer<const CFireheartMinigameDef> PDef;          // +0x98
    unsigned long PlaySound(const CCharString& name);       // 0x007E7880
    CWideString GetText(unsigned long id) const;            // 0x007E7580
    void DelayedQuit();
};

void CTCFireheartMinigame::DelayedQuit()
{
    if (QuitWithDelay)
        return;
    QuitWithDelay = true;
    {
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
        PlaySound(sound);
    }
    {
        CWideString empty(L"");
        const CFireheartMinigameDef* def = PDef.operator->();
        CFontBank* font = g_pMainGameComponent->Font;
        CPlayerGui* gui = g_pPlayerGui;
        gui->SubtitleRenderer.SetText(font, GetText(def->QuitText), empty, GetText(def->YesText), GetText(def->NoText),
                                      0x1004, 0.0f, false, true);
    }
    {
        CGameEvent event(9);
        event.AddBOOL(true);
        g_pMainGameComponent->AddGameEvent(event);
    }
}