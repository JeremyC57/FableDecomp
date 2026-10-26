// CTCTavernGame::SetStateAfterOK(CWideString text, CWideString okText, ETavernGameStates state) -- play the selection-change sound, show an OK subtitle prompt, remember the state to enter on OK and enter the OK wait state 44 (or leave the game when a quit is pending)
// Leaf: SetStateAfterOK
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB080, 256 bytes, `ret 0Ch` (the two CWideString arguments are destroyed here).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_OBJECT_SELECTION_CHANGE" @ 0x0125603C),
//   CTCTavernGame::PlaySound(const CCharString&) @ 0x005EA950, ~CCharString @ 0x0099EAE0,
//   CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @ 0x0122D70C), CSubtitleRenderer::SetText @ 0x00492F69 (on g_pPlayerGui
//   @ 0x013B8790 +0x18; font g_pMainGameComponent @ 0x013B86A0 +0x1614C), ~CWideString @ 0x0099B510 (x4),
//   the virtual LeaveGame (slot 30, +0x78). Quit flag @ 0x013BAE1C.
// Layout: retail CTCTavernGame sits 0x1C below Ego_r.pdb (landed SetStateAfterTime 0x005E9B70 / CallFuncAfterOK 0x005E9D60):
//   YesState +0xA8, NoState +0xAC, AltState +0xB0, TavernGameState +0xC0, PreviousTavernGameState +0xC4.
// Shape: the quit test runs both before and after the prompt; VC7.1 merges the two identical exits into the FIRST block
//   (the second test jumps back up to it) only when the second test comes from the inline SetState helper (the
//   SetStateAfterTime body) or an if/else; two plain early-return copies put the merged block at the second site.
#include <stddef.h>
typedef long ETavernGameStates;
extern bool g_bTavernGameQuitRequested;                 // 0x013BAE1C
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
class CMainGameComponent { public:
    unsigned char _pad[0x1614c];
    CFontBank* Font;                                    // +0x1614c
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
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(ETavernGameStates s) {
        if (g_bTavernGameQuitRequested) {
            g_bTavernGameQuitRequested = false;
            LeaveGame();
            return;
        }
        PreviousTavernGameState = TavernGameState;
        TavernGameState = s;
    }
    void SetStateAfterOK(CWideString text, CWideString okText, ETavernGameStates state);
};

void CTCTavernGame::SetStateAfterOK(CWideString text, CWideString okText, ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
        {
            CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
            PlaySound(sound);
        }
        g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, text, L"", okText, L"", 0xa2, 0.0f, false, true);
        YesState = state;
        NoState = 0;
        AltState = 0;
    SetState(44);
}