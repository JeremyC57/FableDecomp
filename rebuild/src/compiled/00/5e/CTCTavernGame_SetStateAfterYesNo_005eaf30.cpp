// CTCTavernGame::SetStateAfterYesNo(CWideString title, CWideString text, ETavernGameStates yesState, CWideString altText, ETavernGameStates noState) -- show a yes/no prompt and remember the follow-up states
// Leaf: ?SetStateAfterYesNo@CTCTavernGame@@
// Flags: /GS
// Retail: Fable.exe @ 0x005EAF30, 326 bytes, `ret 14h`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Catalog row: only the first 22 bytes (it stops at `cmp al,bl`), so no compiled body can equal the catalog oracle.
// Body (decoded from the retail bytes): when the skip-prompt flag (0x013BAE1C) is set it is cleared and own slot 30
// (+0x78) runs; otherwise the selection-change sound plays (0x005EA950 with "TAVERN_GAME_OBJECT_SELECTION_CHANGE"),
// the subtitle renderer (0x013B8790 +0x18, SetText 0x00492F69) shows title / text / alt text in the main game
// component's font (+0x1614C), a type-9 game event carrying a false flag (0x009F1760) goes to
// CMainGameComponent::AddGameEvent (0x0041649C), YesState / NoState / AltState (+0xA8 / +0xAC / +0xB0, Ego_r.pdb names,
// retail 0x1C below the PDB) are stored and the game state moves to 0x2D keeping the previous one (+0xC0 -> +0xC4);
// a skip flag raised meanwhile takes the first path. The three by-value CWideStrings are destroyed on return.
// Graded against the full 0x146-byte body (RELOCATION_MATCH, /GS); the game-event field names are descriptive guesses.
// Shape: if/else on the skip flag with the skip handling written out in both places (VC7.1 merges the two copies into
// retail's first block); the event's bool payload goes through an inline by-value `SetData(T value)` (Copy(sizeof(T),
// &value)) so it is homed in its own aligned frame word and stored last, as retail; a named bool before the event
// gives a byte in a shared word (326v326).
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
enum ETavernGameStates { TAVERN_STATE_YES_NO = 0x2d };
class CTCTavernGame {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void OnYesNoSkipped();                             // slot 30 (+0x78) (name descriptive)
    void PlaySound(const CCharString& name);                   // 0x005EA950
    void SetStateAfterYesNo(CWideString title, CWideString text, ETavernGameStates yesState, CWideString altText,
                            ETavernGameStates noState);
    unsigned char     _pad004[0xa4];
    ETavernGameStates YesState;                                // +0xa8
    ETavernGameStates NoState;                                 // +0xac
    ETavernGameStates AltState;                                // +0xb0
    unsigned char     _pad0b4[0xc];
    ETavernGameStates State;                                   // +0xc0
    ETavernGameStates PreviousState;                           // +0xc4
};
void CTCTavernGame::SetStateAfterYesNo(CWideString title, CWideString text, ETavernGameStates yesState, CWideString altText,
                                       ETavernGameStates noState)
{
    if (g_bTavernSkipYesNo) {
        g_bTavernSkipYesNo = false;
        OnYesNoSkipped();
    } else {
        PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        g_pSubtitleOwner->Renderer.SetText(g_pMainGameComponent->PFont, title, L"", text, altText, 0x84, 0.0f, false, true);
        CGameEvent event(9);
        event.SetData(false);
        g_pMainGameComponent->AddGameEvent(&event);
        YesState = yesState;
        NoState = noState;
        AltState = (ETavernGameStates)0;
        if (g_bTavernSkipYesNo) {
            g_bTavernSkipYesNo = false;
            OnYesNoSkipped();
        } else {
            PreviousState = State;
            State = TAVERN_STATE_YES_NO;
        }
    }
}
