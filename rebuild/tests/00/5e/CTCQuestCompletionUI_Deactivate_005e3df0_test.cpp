// Behaviour test for CTCQuestCompletionUI::Deactivate @ 0x005E3DF0: does nothing while the UI is inactive; when
// active it sets QuittingFadeOut, calls the black overlay component's virtual slot 48 (SetFading) with true, then
// queues game event 9 carrying one zero byte on the main game component. Checks the inactive path (overlay never
// touched), the active path (flag, call order, the slot-48 argument, event contents) and that Active itself is kept.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

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
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetFading(bool on);               // slot 48 (+0xC0)
};
}
struct CTCQuestCompletionUI {
    virtual void v00();
    unsigned char _pad04[0x08];
    bool          Active;                          // +0x0c
    bool          ReallyActive;                    // +0x0d
    unsigned char _pad0e[0xbc - 0x0e];
    NUISystem::CComponent* PBlack;                 // +0xbc
    bool          QuittingFadeOut;                 // +0xc0
    void Deactivate();
};

void CTCQuestCompletionUI::Deactivate()
{
    if (Active) {
        QuittingFadeOut = true;
        PBlack->SetFading(true);
        CGameEvent gameEvent(9);
        gameEvent.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(gameEvent);
    }
}

// ---- instrumented stubs ----
#define V(n) void NUISystem::CComponent::v##n() { g_fail = 1; }
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15)
V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31)
V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47)
#undef V
static int g_seq = 0, g_fadeAt = 0, g_eventAt = 0, g_fades = 0, g_events = 0;
static bool g_fadeArg = false, g_flagAtFade = false;
static CTCQuestCompletionUI* g_ui = 0;
struct TestBlack : NUISystem::CComponent {
    virtual void SetFading(bool on) { ++g_fades; g_fadeAt = ++g_seq; g_fadeArg = on; g_flagAtFade = g_ui->QuittingFadeOut; if (this != (void*)g_ui->PBlack) g_fail = 1; }
};
void NUISystem::CComponent::SetFading(bool) { g_fail = 1; }
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}
static CGameEvent g_last(-5);
void CMainGameComponent::AddGameEvent(const CGameEvent& event) { ++g_events; g_eventAt = ++g_seq; g_last = event; if (this != g_pMainGameComponent) g_fail = 1; }
CMainGameComponent* g_pMainGameComponent;
void CTCQuestCompletionUI::v00() { }

static void reset() { g_seq = g_fadeAt = g_eventAt = g_fades = g_events = 0; g_fadeArg = g_flagAtFade = false; g_last = CGameEvent(-5); }

int main()
{
    CMainGameComponent mgc; g_pMainGameComponent = &mgc;
    TestBlack black;
    CTCQuestCompletionUI ui; g_ui = &ui; ui.PBlack = &black;
    ui.Active = false; ui.ReallyActive = true; ui.QuittingFadeOut = false;

    // Inactive: nothing.
    reset();
    ui.Deactivate();
    CHECK(g_fades == 0 && g_events == 0 && !ui.QuittingFadeOut && !ui.Active && ui.ReallyActive);

    // Inactive with a null overlay: still nothing (overlay not dereferenced).
    ui.PBlack = 0;
    ui.Deactivate();
    CHECK(g_fades == 0 && g_events == 0);
    ui.PBlack = &black;

    // Active: flag first, fade(true), then event 9 with one zero byte.
    reset(); ui.Active = true;
    ui.Deactivate();
    CHECK(ui.QuittingFadeOut && g_fades == 1 && g_fadeArg && g_flagAtFade && g_events == 1 && g_fadeAt == 1 && g_eventAt == 2);
    CHECK(g_last.Type == 9 && g_last.Player == -1 && g_last.EndPos == 1 && g_last.Data[0] == 0 && g_last.Valid && !g_last.Replacement);
    CHECK(ui.Active && ui.ReallyActive);

    if (g_fail) { std::printf("005e3df0_TEST FAIL\n"); return 1; }
    std::printf("005e3df0_TEST PASS\n");
    return 0;
}