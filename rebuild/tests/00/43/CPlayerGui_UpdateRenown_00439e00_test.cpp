// Behaviour test for 00439e00: src/00439e00.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CPlayerGui::UpdateRenown()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayerGui::UpdateRenown() -- feed the pending renown of the stat-updates display into the renown stat bar: once the bar is initialised, animate it (step count from the bar's value change) and advance its update counter; otherwise snap the bar to the pending value; finally clear the pending amount when the bar reports mode 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00439E00, length 278 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (PlayerManager +0x08), CPlayer::GetPlayerCharacter 0x00487B70,
// __ftol2 0x00BFEA70, PeekGuiDef 0x00438570 (the lazily built GUI definition, x2), CPlayerGui::UpdateStat(float, float,
// float, float, float, CStatBase*) 0x00437AE0. Floats 0.2f 0x012305A0, 0.5f 0x0122F59C, 0.01f as an immediate.
// Layout (retail; engine/CPlayerGui.h is 8 bytes high after the narrower CSubtitleRenderer and 4 per vector):
// PDrawStatUpdates +0x1a8 (pending renown +0x28), PRenown (CStatBar) +0x2d4; Ego_r.pdb CStatBase ValueInitialised +8,
// CurrentValue +0xc, OldValue +0x10, LastOldValue +0x14, GuiMode +0x28 (retail 12-byte vectors: the bar's update counter
// +0x48, its previous value +0x4c); the GUI def's renown bar colours +0x4e8/+0x4ec.
// Shape: the dead-bit test uses the constant 1 held in ebx (also the minimum step, true and the mode compare); the
// pending amount is a named local homed in a frame word for the fild; each snap store re-reads PRenown.
struct CPlayerGuiDef {
    unsigned char _pad000[0x4e8];
    float         RenownBarValueA;                     // +0x4e8
    float         RenownBarValueB;                     // +0x4ec
};
CPlayerGuiDef* __cdecl PeekGuiDef();                   // 0x00438570

extern "C" long __cdecl _ftol2(double);
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char StateFlags;                          // +0x91 (bit 0: dead)
};
struct CPlayer {
    CThing* GetPlayerCharacter();                      // 0x00487B70
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                          // 0x00449970
};
struct CStatBase {
    void*          __vftable;                          // +0x00
    CPlayerGuiDef* GuiDef;                             // +0x04
    bool           ValueInitialised;                   // +0x08
    float          CurrentValue;                       // +0x0c
    float          OldValue;                           // +0x10
    float          LastOldValue;                       // +0x14
    float          RecentMaxValue;                     // +0x18
    float          OldRecentMaxValue;                  // +0x1c
    unsigned long  FrameCurrentValueLastMoved;         // +0x20
    unsigned long  FrameFullyOpenedOn;                 // +0x24
    long           GuiMode;                            // +0x28
    float          FadeInTimer;                        // +0x2c
    unsigned char  _vectors[0x18];                     // PrimitiveHandles, Effects
    long           UpdateCount;                        // +0x48
    long           PreviousUpdateCount;                // +0x4c
    float GetValueChange() const { return (float)fabs(OldValue - CurrentValue); }
};
struct CDrawStatUpdates {
    unsigned char _pad00[0x28];
    long          PendingRenown;                       // +0x28
};

struct CPlayerGui {
    unsigned char     _pad000[8];
    CPlayerManager*   PlayerManager;                   // +0x08
    unsigned char     _pad00c[0x19c];
    CDrawStatUpdates* PDrawStatUpdates;                // +0x1a8 (CCountedPointer::Data)
    unsigned char     _pad1ac[0x128];
    CStatBase*        PRenown;                         // +0x2d4 (CCountedPointer<CStatBar>::Data)
    void UpdateStat(float value, float speed, float steps, float a, float b, CStatBase* stat);   // 0x00437AE0
    void UpdateRenown();
};

void CPlayerGui::UpdateRenown()
{
    CThing* character = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (character == 0 || (character->StateFlags & 1))
        return;
    long pending = PDrawStatUpdates->PendingRenown;
    CStatBase* const& renown = PRenown;
    if (renown->ValueInitialised) {
        long steps = (long)((PRenown->GetValueChange() - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)pending, 0.01f, (float)steps, PeekGuiDef()->RenownBarValueA, PeekGuiDef()->RenownBarValueB, renown);
        PRenown->PreviousUpdateCount = PRenown->UpdateCount;
        ++PRenown->UpdateCount;
    } else {
        float value = (float)pending;
        PRenown->CurrentValue = value;
        PRenown->OldValue = value;
        PRenown->LastOldValue = value;
        PRenown->ValueInitialised = true;
    }
    if (pending > 0 && PRenown->GuiMode == 1) {
        PDrawStatUpdates->PendingRenown = 0;
        PRenown->ValueInitialised = false;
    }
}

// ---- stubs and driver ----

static CPlayerGuiDef g_def;
CPlayerGuiDef* __cdecl PeekGuiDef() { return &g_def; }
static CThing* g_char;
CThing* CPlayer::GetPlayerCharacter() { return g_char; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
static int g_updates = 0; static float g_v, g_speed, g_steps, g_a, g_b; static CStatBase* g_stat;
void CPlayerGui::UpdateStat(float value, float speed, float steps, float a, float b, CStatBase* stat)
{ ++g_updates; g_v = value; g_speed = speed; g_steps = steps; g_a = a; g_b = b; g_stat = stat; }
int main()
{
    static CThing ch; memset(&ch, 0, sizeof ch); g_char = &ch;
    g_def.RenownBarValueA = 1.5f; g_def.RenownBarValueB = 2.5f;
    static CPlayerManager pm; static CDrawStatUpdates dsu; memset(&dsu, 0, sizeof dsu);
    static CStatBase stat; memset(&stat, 0, sizeof stat);
    static CPlayerGui gui; memset(&gui, 0, sizeof gui); gui.PlayerManager = &pm; gui.PDrawStatUpdates = &dsu; gui.PRenown = &stat;
    dsu.PendingRenown = 40;
    gui.UpdateRenown();                                   // first call initialises the bar
    CHECK(stat.ValueInitialised); CHECK(stat.CurrentValue == 40.0f && stat.OldValue == 40.0f && stat.LastOldValue == 40.0f); CHECK(g_updates == 0);
    stat.OldValue = 10.0f; stat.CurrentValue = 20.6f; stat.UpdateCount = 7; dsu.PendingRenown = 50;
    gui.UpdateRenown();                                   // change 10.6 -> steps (10.4*0.5) = 5
    CHECK(g_updates == 1); CHECK(g_v == 50.0f); CHECK(g_speed == 0.01f); CHECK(g_steps == 5.0f); CHECK(g_a == 1.5f && g_b == 2.5f); CHECK(g_stat == &stat);
    CHECK(stat.PreviousUpdateCount == 7 && stat.UpdateCount == 8);
    stat.OldValue = 20.0f; stat.CurrentValue = 20.0f; stat.GuiMode = 1; dsu.PendingRenown = 3;
    gui.UpdateRenown();                                   // no change -> at least one step; gui mode 1 clears the pending renown
    CHECK(g_steps == 1.0f); CHECK(dsu.PendingRenown == 0); CHECK(!stat.ValueInitialised);
    ch.StateFlags = 1; dsu.PendingRenown = 9; gui.UpdateRenown(); CHECK(g_updates == 2); CHECK(dsu.PendingRenown == 9);   // dead: nothing
    g_char = 0; gui.UpdateRenown(); CHECK(g_updates == 2);
    if (fails == 0) printf("00439e00_TEST PASS\n"); else printf("00439e00_TEST FAIL\n");
    return fails;
}