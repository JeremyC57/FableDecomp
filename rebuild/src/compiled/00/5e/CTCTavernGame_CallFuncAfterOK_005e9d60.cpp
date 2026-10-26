// CTCTavernGame::CallFuncAfterOK(CWideString text, CWideString okText, void (__fastcall *okFunc)()) -- show an OK subtitle prompt, announce it with game event 9, remember the OK callback and enter the OK wait state 44 (or leave the game when a quit is pending)
// Leaf: CallFuncAfterOK
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9D60, 271 bytes, `ret 0Ch` (the two CWideString arguments are destroyed here),
//   /GS cookie 0x0139C8A8 / __security_check_cookie 0x00BFE9F9. Ego_r.pdb `?CallFuncAfterOK@CTCTavernGame@@IAEXVCWideString@@0P6IXXZ@Z`.
// Callees: CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @ 0x0122D70C), CSubtitleRenderer::SetText(CFontBank*, const CWideString&
//   x4, unsigned long, float, bool, bool) @ 0x00492F69 (on g_pPlayerGui @ 0x013B8790 +0x18; font g_pMainGameComponent
//   @ 0x013B86A0 +0x1614C), ~CWideString @ 0x0099B510 (x4), CMemoryBuffer::Copy @ 0x009F1760 (inline CGameEvent::AddBOOL),
//   CMainGameComponent::AddGameEvent @ 0x0041649C, the virtual LeaveGame (slot 30, +0x78). Quit flag @ 0x013BAE1C.
// Layout: retail CTCTavernGame sits 0x1C below Ego_r.pdb (landed SetStateAfterTime 0x005E9B70): YesFunc (the OK callback) +0xB4,
//   TavernGameState +0xC0, PreviousTavernGameState +0xC4.
// Shape: the CallFuncAfterYesNo twin (0x005E9C50): the empty name and second button are implicit L"" conversion
//   temporaries (the rightmost built first); the CGameEvent (Type 9, Player -1, EndPos 0, Valid, not Replacement) is a block local whose char data takes the
//   /GS frame; then the quit test as SetStateAfterTime.
#include <stddef.h>
typedef long ETavernGameStates;
extern bool g_bTavernGameQuitRequested;                 // 0x013BAE1C
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
};
class CFontBank;
class CSubtitleRenderer {
public:
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, unsigned long flags, float time, bool b1, bool b2);   // 0x00492F69
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBOOL(bool value) { Copy(1, &value); }
};
class CMainGameComponent { public:
    unsigned char _pad[0x1614c];
    CFontBank* Font;                                    // +0x1614c
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
typedef void (__fastcall *TTavernGameFunc)();
class CTCTavernGame { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void LeaveGame();                           // slot 30 (+0x78)
    unsigned char     _pad_0x04[0xa8 - 0x04];
    ETavernGameStates YesState;                         // +0xa8
    ETavernGameStates NoState;                          // +0xac
    ETavernGameStates AltState;                         // +0xb0
    TTavernGameFunc   YesFunc;                          // +0xb4
    TTavernGameFunc   NoFunc;                           // +0xb8
    TTavernGameFunc   AltFunc;                          // +0xbc
    ETavernGameStates TavernGameState;                  // +0xc0
    ETavernGameStates PreviousTavernGameState;          // +0xc4
    void CallFuncAfterOK(CWideString text, CWideString okText, TTavernGameFunc okFunc);
};

void CTCTavernGame::CallFuncAfterOK(CWideString text, CWideString okText, TTavernGameFunc okFunc)
{
    g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, text, L"", okText, L"", 0xa2, 0.0f, false, true);
    {
        CGameEvent event(9);
        event.AddBOOL(false);
        g_pMainGameComponent->AddGameEvent(event);
    }
    YesFunc = okFunc;
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = 44;
}