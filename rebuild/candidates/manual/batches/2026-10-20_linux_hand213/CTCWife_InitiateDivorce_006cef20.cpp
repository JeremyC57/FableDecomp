// CTCWife::InitiateDivorce() -- unless a hero-information screen is up, shows the def's "divorce occurred" subtitle, flags WaitingForDivorce, joins the global update list and pauses the game for the current player (SendPauseEvent(true) inlined)
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CEF20, 192 bytes. __fastcall, `ret`, /GS cookie 0x0139C8A8.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (manifest "CWorld::DrawGetEnvironment", propagated);
//          CSubtitleRenderer::SetText(CFontBank*, const CCharString&, unsigned long, float, bool, bool) 0x00492ED1 on the
//          renderer embedded at CPlayerGui+0x18; CTCBase::AddToGlobalUpdateTCs() 0x00686ED0; CTCBase::GetWorld() const
//          0x00686A80; CWorld::GetCurrentPlayer() 0x0049D9D0 (player number byte +0x28); CMemoryBuffer::Copy 0x009F1760;
//          CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C; __security_check_cookie 0x00BFE9F9.
//          Globals: g_pPlayerGui 0x013B8790, g_pMainGameComponent 0x013B86A0 (font bank at +0x1614c).
// Shape: the screens manager's +0x34 flag byte (descriptive: a screen is active) is the only guard. SetText takes the
//        font bank, the def's TextGuiWifeDivorceOccurred (CHeroMarriageDef +0xa4, formed with `add eax,0A4h`), colour 0x22,
//        time 0.0f, true, true. The pause event is the landed CTCWife::SendPauseEvent(bool) 0x006CEBF0 body inlined with
//        a constant true: CGameEvent(9) initialised in its frame slot (Type 9, Player -1, EndPos 0, Valid, !Replacement),
//        the player number stored over Player, the normalised flag byte in its own slot handed to Copy(1, &flag). The
//        40-byte event buffer is what makes the TU /GS.
// Layout: CTCWife ParentThing +0x04, PHeroMarriageDef +0x0c, WaitingForDivorce +0x29 (engine/CTCWife.h).
struct CCharString;
struct CFontBank;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBool(bool value) { value = value ? true : false; Copy(1, &value); }
};
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank* m_pFontBank;                        // +0x1614c
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CSubtitleRenderer {                         // embedded at CPlayerGui+0x18
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
namespace NHeroInformationScreens {
struct CManager {
    unsigned char _pad00[0x34];
    unsigned char m_screenActive;                  // +0x34 (descriptive: a hero-information screen is up)
};
}
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer m_subtitleRenderer;          // +0x18
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
struct CPlayer {
    unsigned char _pad_0x00[0x28];
    signed char   m_playerNumber;                  // +0x28
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;             // 0x0049D9D0
};
struct CHeroMarriageDef {
    unsigned char _pad00[0xa4];
    unsigned char TextGuiWifeDivorceOccurred[4];   // +0xa4 CCharString
};
struct CTCWife {
    virtual void v00();
    void*             ParentThing;                 // +0x04
    unsigned char     _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;            // +0x0c CDefPointer<CHeroMarriageDef const>::Object
    unsigned char     _pad_0x10[0x29 - 0x10];
    bool              WaitingForDivorce;           // +0x29
    void AddToGlobalUpdateTCs();                   // 0x00686ED0 (CTCBase)
    const CWorld* GetWorld() const;                // 0x00686A80 (CTCBase)
    void InitiateDivorce();
};
void CTCWife::InitiateDivorce()
{
    if (g_pPlayerGui->GetPHeroInfoScreensManager()->m_screenActive)
        return;
    g_pPlayerGui->m_subtitleRenderer.SetText(g_pMainGameComponent->m_pFontBank,
        *(const CCharString*)PHeroMarriageDef->TextGuiWifeDivorceOccurred, 0x22, 0.0f, true, true);
    WaitingForDivorce = true;
    AddToGlobalUpdateTCs();
    CGameEvent gameEvent(9);                       // SendPauseEvent(true) written out: the constant flag folds to one store
    gameEvent.Player = GetWorld()->GetCurrentPlayer()->m_playerNumber;
    gameEvent.AddBool(true);
    g_pMainGameComponent->AddGameEvent(gameEvent);
}
