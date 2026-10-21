// CPlayer::InitPlayerGui() -- for the main player only: create the player GUI (component, world, player interface, player manager and the component's graphic bank), hand it to the global GUI holder and open it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487FB0, length 99 bytes.
// Callees: CPlayerManager::GetMainPlayerNumber @ 0x00449700, operator new @ 0x00BFEA1A (0x338 bytes),
// CPlayerGui::CPlayerGui(CMainGameComponent*, CWorld*, CGamePlayerInterface*, CPlayerManager*, CGraphicDataBank*)
// @ 0x0043B570, the global holder's Reset(CPlayerGui*) @ 0x004195AF (its manifest label is propagated), CPlayerGui::Open
// @ 0x004374B0 (tail jump through the holder's pointer @ 0x013B8790).
// Layout: engine/CPlayer.h (PlayerInterface +0x0C, PlayerManager +0x10, Component +0x14, World +0x1C, Number +0x28);
// the component's graphic data bank pointer sits at +0x161C8 (descriptive).
// Shape: the player number is read into a local before GetMainPlayerNumber (`mov edi,[esi+28h] ... cmp edi,eax`); a plain `new` (no EH frame),
// the null result passed as 0 (`xor eax,eax` arm). The player interface and the graphic bank are assigned to locals
// inside the argument list: retail loads bank/manager/interface into edi/ecx/ebx (ebx pushed in the arm) after the
// null test and before the pushes; locals declared before `new` load before the call, plain arguments interleave.
#include "engine/CPlayer.h"

struct CGraphicDataBank;

struct CMainGameComponentBank {
    unsigned char     _pad00[0x161c8];
    CGraphicDataBank* GraphicDataBank;                      // +0x161C8
};

struct CPlayerManagerCalls {
    long GetMainPlayerNumber();                             // 0x00449700
};

struct CPlayerGui {
    unsigned char _body[0x338];
    CPlayerGui(CMainGameComponent* component, CWorld* world, CGamePlayerInterface* playerInterface,
               CPlayerManager* playerManager, CGraphicDataBank* bank);   // 0x0043B570
    void Open();                                            // 0x004374B0
};

struct CPlayerGuiHolder {                                   // the global owning pointer @ 0x013B8790
    CPlayerGui* m_pGui;
    void Reset(CPlayerGui* gui);                            // 0x004195AF
    CPlayerGui* operator->() const { return m_pGui; }
};
extern CPlayerGuiHolder g_PlayerGui;                        // 0x013B8790

struct CPlayer_Methods : CPlayer {
    void InitPlayerGui();
};

void CPlayer_Methods::InitPlayerGui()
{
    long number = Number;
    if (number == reinterpret_cast<CPlayerManagerCalls*>(PlayerManager)->GetMainPlayerNumber()) {
        CGamePlayerInterface* playerInterface;
        CGraphicDataBank* bank;
        g_PlayerGui.Reset(new CPlayerGui(Component, World, playerInterface = PlayerInterface, PlayerManager,
                                         bank = reinterpret_cast<CMainGameComponentBank*>(Component)->GraphicDataBank));
        g_PlayerGui->Open();
    }
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CPlayerGuiHolder g_PlayerGui;
static long g_mainNumber = 0; static const void* g_numThis = 0;
static CMainGameComponent* g_c; static CWorld* g_w; static CGamePlayerInterface* g_pi; static CPlayerManager* g_pm; static CGraphicDataBank* g_bank;
static int g_ctors = 0, g_opens = 0, g_resets = 0; static CPlayerGui* g_reset = 0; static const CPlayerGui* g_opened = 0;
long CPlayerManagerCalls::GetMainPlayerNumber() { g_numThis = this; return g_mainNumber; }
CPlayerGui::CPlayerGui(CMainGameComponent* component, CWorld* world, CGamePlayerInterface* playerInterface,
                       CPlayerManager* playerManager, CGraphicDataBank* bank)
{ ++g_ctors; g_c = component; g_w = world; g_pi = playerInterface; g_pm = playerManager; g_bank = bank; }
void CPlayerGui::Open() { ++g_opens; g_opened = this; }
void CPlayerGuiHolder::Reset(CPlayerGui* gui) { ++g_resets; g_reset = gui; m_pGui = gui; }
int main()
{
    static unsigned char bankMem[sizeof(CMainGameComponentBank)];
    CMainGameComponentBank* comp = (CMainGameComponentBank*)bankMem;
    comp->GraphicDataBank = (CGraphicDataBank*)0x4444;
    static unsigned char playerMem[sizeof(CPlayer_Methods)];
    CPlayer_Methods* p = (CPlayer_Methods*)playerMem;
    p->PlayerInterface = (CGamePlayerInterface*)0x1111;
    p->PlayerManager = (CPlayerManager*)0x2222;
    p->Component = (CMainGameComponent*)comp;
    p->World = (CWorld*)0x3333;
    p->Number = 1;
    g_mainNumber = 0;
    p->InitPlayerGui();                         // not the main player: nothing
    CHECK(g_numThis == (void*)0x2222 && g_ctors == 0 && g_resets == 0 && g_opens == 0);
    g_mainNumber = 1;
    p->InitPlayerGui();
    CHECK(g_ctors == 1 && g_resets == 1 && g_opens == 1);
    CHECK(g_c == (CMainGameComponent*)comp && g_w == (CWorld*)0x3333 && g_pi == (CGamePlayerInterface*)0x1111);
    CHECK(g_pm == (CPlayerManager*)0x2222 && g_bank == (CGraphicDataBank*)0x4444);
    CHECK(g_reset != 0 && g_opened == g_reset && g_PlayerGui.m_pGui == g_reset);
    if (fails == 0) printf("00487fb0_TEST PASS\n");
    return fails;
}