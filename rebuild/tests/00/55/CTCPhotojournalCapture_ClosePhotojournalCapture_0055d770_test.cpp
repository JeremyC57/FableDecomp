#include <cstdio>
// Behaviour model of CTCPhotojournalCapture::ClosePhotojournalCapture (0x0055D770)
struct CGameEvent { long Type; signed char Player; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long t) : Type(t), Player(-1), EndPos(0), Valid(true), Replacement(false) {} };
static long g_lastType = 0, g_lastPlayer = 0, g_events = 0, g_renders = 0;
struct CPlayerManager { long main; long GetMainPlayerNumber() { return main; } };
struct CMainGameComponent { CPlayerManager* PlayerManager; void AddGameEvent(const CGameEvent& e) { g_lastType = e.Type; g_lastPlayer = e.Player; ++g_events; } };
struct CTCInventory { void StartGameRendering() { ++g_renders; } };
struct CThing { CTCInventory* inv; bool GetInterface(CTCInventory** out, long t) { if (t == 0x11 && inv) { *out = inv; return true; } return false; } };
static CMainGameComponent* g_pMainGameComponent;
struct CTCPhotojournalCapture { CThing* ParentThing; bool ScreenOpen;
    void ClosePhotojournalCapture() {
        ScreenOpen = false;
        CGameEvent event(0x43);
        event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
        g_pMainGameComponent->AddGameEvent(event);
        CTCInventory* inventory;
        if (ParentThing->GetInterface(&inventory, 0x11)) inventory->StartGameRendering();
    } };
int main()
{
    CPlayerManager pm = { 2 }; CMainGameComponent mgc = { &pm }; g_pMainGameComponent = &mgc;
    CTCInventory inv; CThing t1 = { &inv }; CThing t2 = { 0 };
    CTCPhotojournalCapture c = { &t1, true };
    c.ClosePhotojournalCapture();
    if (c.ScreenOpen || g_lastType != 0x43 || g_lastPlayer != 2 || g_events != 1 || g_renders != 1) { std::printf("0055d770_TEST FAIL\n"); return 1; }
    c.ParentThing = &t2; c.ClosePhotojournalCapture();
    if (g_events != 2 || g_renders != 1) { std::printf("0055d770_TEST FAIL\n"); return 1; }
    std::printf("0055d770_TEST PASS\n");
    return 0;
}