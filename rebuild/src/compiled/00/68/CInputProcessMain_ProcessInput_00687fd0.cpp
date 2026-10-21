// CInputProcessMain::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a press of action 0x35 in normal play (world load status 0 or 9, nothing pending, no auto-save banner) take a quick photojournal screenshot (without the HUD while Shift is held) and mark the input handled; otherwise make the console on first use, let it see the event unless it is open, and on a non-type-1 event steer the player GUI's mouse cursor for action 0x47
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687FD0, length 430 bytes (`ret 8`), /GS cookie 0x0139C8A8 / check 0x00BFE9F9.
// Callees: CInputEvent ctor 0x00A04410 / dtor 0x00A03B30 / GetType 0x00A03B40 (an unused 0x34-byte frame local, as the landed
//   CTCAssassinRush::UpdateDirectionOverride 0x007A9960); CInputProcessGameBase::IsEventGameAction 0x00687CF0 /
//   IsEventButtonPressed 0x00687A90 / GetMouseMovement 0x00687D20 (C2DVector by hidden pointer; it forwards to
//   CGamePlayerInterface::GetMouseMovement 0x004458F0); CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetHeroCreature
//   0x00487DC0; the component map LowerBound 0x0040F020; GetAsyncKeyState (import 0x014402CC);
//   CTCPhotojournalCapture::TakeQuickScreenshotNoHud 0x0055D490 / TakeQuickScreenshot 0x0055D180; operator new 0x00BFEA1A,
//   CConsole ctor 0x009ECD80, CCountedPointer<CConsole>::Reset 0x00413520, CConsole::Get 0x00414C90, CConsole::ProcessInput
//   0x009EA540 (a `xor al,al` stub); CPlayerGui::MoveCursor 0x004379C0; CProcessedInput::SetHandled 0x00A0D390.
// Layout: the main game component singleton 0x013B86A0 (PPlayerManager +0x1c, PWorld +0x24 as Ego_r.pdb CMainGameComponent);
//   CWorld SaveGameLoadStatus +0x104 (PDB offset), +0xf8 a pending-state word (descriptive); g_bAutoSaveProgressShown
//   0x013B89C1 and the player GUI 0x013B8790 as the landed sources name them; the console counted pointer 0x013CAA40 and
//   CConsole +0x78 open flag (descriptive) as the landed CInputProcessConsole::ProcessInput 0x00687F80; CThing component bits
//   +0x20 / map +0x44 as the landed CInputProcessQuickAccessMenu::ProcessInput 0x00690FD0 (kind 0x10E = word 8 bit 14).
// Shape: the mouse movement is a NAMED C2DVector local (retail re-takes its address with `lea` for MoveCursor); passing the
//   call result straight through pushes the returned pointer instead.
extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int key);
struct C2DVector { float X, Y; C2DVector() {} };
class CInputEvent {
public:
    unsigned char _body[0x34];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
    long GetType() const;                          // 0x00A03B40
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
struct CTCPhotojournalCapture {
    void TakeQuickScreenshotNoHud();               // 0x0055D490
    void TakeQuickScreenshot();                    // 0x0055D180
};
struct STCEntry { long m_kind; CTCPhotojournalCapture* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCPhotojournalCapture* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCPhotojournalCapture* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CPlayer {
    CThing* GetHeroCreature();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CWorld {
    unsigned char _pad00[0xf8];
    long          PendingState;                    // +0xf8 (descriptive)
    unsigned char _padfc[0x104 - 0xfc];
    long          SaveGameLoadStatus;              // +0x104
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                // +0x1c
    unsigned char   _pad20[4];
    CWorld*         PWorld;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
extern bool g_bAutoSaveProgressShown;              // 0x013B89C1
struct CPlayerGui {
    void MoveCursor(const C2DVector& movement);    // 0x004379C0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
struct CConsole {                                  // vtable 0x0129C600, 0x9c bytes
    virtual void v00();
    unsigned char _pad04[0x78 - 0x04];
    bool          IsOpen;                          // +0x78 (descriptive)
    unsigned char _body[0x9c - 0x79];
    CConsole();                                    // 0x009ECD80
    static CConsole* __fastcall Get();             // 0x00414C90
    bool ProcessInput(const CInputEvent& event);   // 0x009EA540
};
struct CCountedPointer_CConsole {
    CConsole* m_pObject;
    void*     m_pCount;
    void Reset(CConsole* object);                  // 0x00413520
};
extern CCountedPointer_CConsole g_pConsole;        // 0x013CAA40
struct CGamePlayerInterface;
class CInputProcessMain {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // slot 4
    unsigned char           _pad04[0x24 - 0x04];
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    C2DVector GetMouseMovement(const CInputEvent& event);             // 0x00687D20
};

void CInputProcessMain::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CInputEvent unused;
    if (IsEventGameAction(0x35, event) && IsEventButtonPressed(event) && g_pMainGameComponent) {
        CWorld* world = g_pMainGameComponent->PWorld;
        if ((world->SaveGameLoadStatus == 0 || world->SaveGameLoadStatus == 9) && world->PendingState == 0
            && !g_bAutoSaveProgressShown) {
            CThing* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
            if (hero->HasTC(0x10e)) {
                CTCPhotojournalCapture* capture = hero->GetTC(0x10e);
                if (GetAsyncKeyState(0x10) < 0) {
                    capture->TakeQuickScreenshotNoHud();
                    processed.SetHandled();
                } else {
                    capture->TakeQuickScreenshot();
                    processed.SetHandled();
                }
                return;
            }
        }
    }
    if (!g_pConsole.m_pObject)
        g_pConsole.Reset(new CConsole());
    if (g_pConsole.m_pObject->IsOpen)
        return;
    if (CConsole::Get()->ProcessInput(event))
        processed.SetHandled();
    if (event.GetType() == 1)
        return;
    if (IsEventButtonPressed(event))
        IsEventGameAction(6, event);
    else if (IsEventGameAction(0x47, event)) {
        C2DVector movement = GetMouseMovement(event);
        g_pPlayerGui->MoveCursor(movement);
    }
}