// Leaf: ?SetStateAfterYesNoAlt@CTCTavernGame@@
// Flags: /GS
// Grade: RELOCATION_MATCH 354/354 on the real span 0x005EADC0..0x005EAF1F (ret 1Ch), /GS
// CTCTavernGame::SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
//   ETavernGameStates noState, CWideString altText, ETavernGameStates altState) -- show a three-way (yes / no / alternative)
//   prompt and remember the three follow-up states
// Retail: Fable.exe @ 0x005EADC0, 354 bytes, `ret 1Ch` at 0x005EAF1F, /GS (cookie 0x0139C8A8, __security_check_cookie
//   0x00BFE9F9). Name and signature: Ego_r.pdb `?SetStateAfterYesNoAlt@CTCTavernGame@@IAEXVCWideString@@0W4ETavernGameStates@1@0101@Z`.
// Catalog row: only the first 22 bytes (the /GS prologue up to `cmp al,bl`); the landed source was an asm_bake of those bytes.
// Body: the three-option sibling of SetStateAfterYesNo 0x005EAF30 (the model): skip flag 0x013BAE1C -> clear it and run
//   own slot 30 (+0x78); else the selection-change sound, the subtitle renderer's five-string SetText overload 0x00492973
//   (title, "", text, noText, altText, 0x84, 0.0f, false, true), the type-9 game event with a false payload, then
//   YesState / NoState / AltState (+0xA8 / +0xAC / +0xB0) take the three state arguments and the game state moves to
//   0x2E keeping the previous one; a skip raised meanwhile takes the skip path. The four by-value CWideStrings are
//   destroyed on return. Parameter names noText / altText are descriptive (the PDB keeps only types).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                            // 0x0099B510
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                 const CWideString& altText, unsigned long flags, float time, bool b0, bool b1);   // 0x00492F69
    void SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                 const CWideString& altText, const CWideString& altText2, unsigned long flags, float time, bool b0,
                 bool b1);                                     // 0x00492973
};
struct CGameEventData {
    unsigned char Bytes[0x24];
    void Copy(long size, const void* data);                    // 0x009F1760
};
struct CGameEvent {
    long           Type;                                       // +0x00
    unsigned char  Flag;                                       // +0x04
    unsigned char  _pad05[0x20];
    unsigned char  B25, B26, B27;                              // +0x25..+0x27
    CGameEvent(long type) : Type(type), Flag(0xff), B25(0), B26(1), B27(0) { }
    template <class T> void SetData(T value) { ((CGameEventData*)this)->Copy(sizeof(T), &value); }
};
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank*    PFont;                                       // +0x1614c
    void AddGameEvent(CGameEvent* event);                      // 0x0041649C
};
struct CSubtitleOwner { unsigned char _pad[0x18]; CSubtitleRenderer Renderer; };
extern CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0
extern CSubtitleOwner* g_pSubtitleOwner;                       // 0x013B8790
extern bool g_bTavernSkipYesNo;                                // 0x013BAE1C
enum ETavernGameStates { TAVERN_STATE_YES_NO = 0x2d, TAVERN_STATE_YES_NO_ALT = 0x2e };
class CTCTavernGame {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void OnYesNoSkipped();                             // slot 30 (+0x78) (name descriptive)
    void PlaySound(const CCharString& name);                   // 0x005EA950
    void SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
                               ETavernGameStates noState, CWideString altText, ETavernGameStates altState);
    unsigned char     _pad004[0xa4];
    ETavernGameStates YesState;                                // +0xa8
    ETavernGameStates NoState;                                 // +0xac
    ETavernGameStates AltState;                                // +0xb0
    unsigned char     _pad0b4[0xc];
    ETavernGameStates State;                                   // +0xc0
    ETavernGameStates PreviousState;                           // +0xc4
};
void CTCTavernGame::SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
                                          ETavernGameStates noState, CWideString altText, ETavernGameStates altState)
{
    if (g_bTavernSkipYesNo) {
        g_bTavernSkipYesNo = false;
        OnYesNoSkipped();
    } else {
        PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        g_pSubtitleOwner->Renderer.SetText(g_pMainGameComponent->PFont, title, L"", text, noText, altText, 0x84, 0.0f, false, true);
        CGameEvent event(9);
        event.SetData(false);
        g_pMainGameComponent->AddGameEvent(&event);
        YesState = yesState;
        NoState = noState;
        AltState = altState;
        if (g_bTavernSkipYesNo) {
            g_bTavernSkipYesNo = false;
            OnYesNoSkipped();
        } else {
            PreviousState = State;
            State = TAVERN_STATE_YES_NO_ALT;
        }
    }
}
