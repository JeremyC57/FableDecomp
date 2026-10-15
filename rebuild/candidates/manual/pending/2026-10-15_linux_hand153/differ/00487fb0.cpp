// CPlayer::InitPlayerGui() -- for the main player only: create the player GUI (component, world, player interface, player manager and the component's graphic bank), hand it to the global GUI holder and open it
// Grade: DIFFER(97v99) -- argument scheduling of the CPlayerGui constructor call: retail loads the bank, player manager and player interface into edi/ecx/ebx (pushing ebx) before the pushes, ours interleaves loads and pushes without ebx; tried the member compare, a number local (fixed the compare), optimize("s"), inline accessors for every argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487FB0, length 99 bytes.
// Callees: CPlayerManager::GetMainPlayerNumber @ 0x00449700, operator new @ 0x00BFEA1A (0x338 bytes),
// CPlayerGui::CPlayerGui(CMainGameComponent*, CWorld*, CGamePlayerInterface*, CPlayerManager*, CGraphicDataBank*)
// @ 0x0043B570, the global holder's Reset(CPlayerGui*) @ 0x004195AF (its manifest label is propagated), CPlayerGui::Open
// @ 0x004374B0 (tail jump through the holder's pointer @ 0x013B8790).
// Layout: engine/CPlayer.h (PlayerInterface +0x0C, PlayerManager +0x10, Component +0x14, World +0x1C, Number +0x28);
// the component's graphic data bank pointer sits at +0x161C8 (descriptive).
// Shape: the player number is read into a local before GetMainPlayerNumber (`mov edi,[esi+28h] ... cmp edi,eax`); a plain `new` (no EH frame),
// the null result passed as 0 (`xor eax,eax` arm).
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
        g_PlayerGui.Reset(new CPlayerGui(Component, World, PlayerInterface, PlayerManager,
                                         reinterpret_cast<CMainGameComponentBank*>(Component)->GraphicDataBank));
        g_PlayerGui->Open();
    }
}
