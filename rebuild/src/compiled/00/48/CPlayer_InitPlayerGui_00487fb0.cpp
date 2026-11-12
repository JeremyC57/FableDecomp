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

class CGraphicDataBank;

struct CMainGameComponentBank {
    unsigned char     _pad00[0x161c8];
    CGraphicDataBank* GraphicDataBank;                      // +0x161C8
};

struct CPlayerManagerCalls {
    long GetMainPlayerNumber();                             // 0x00449700
};

class CPlayerGui { public:
    unsigned char _body[0x338];
    CPlayerGui(CMainGameComponent* component, CWorld* world, CGamePlayerInterface* playerInterface,
               CPlayerManager* playerManager, CGraphicDataBank* bank);   // 0x0043B570
    void Open();                                            // 0x004374B0
};

// Name fix (agentNF27, 2026-11-12, FAM10 NAME_FIXES): the global owning pointer @ 0x013B8790 is
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass (established ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A,
//   Ego_r / FableWin `dynamic initializer for 'CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass''), set through the inline static Set
//   (FableWin.pdb ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z, FABLE_RETAIL_SINGLETON_SET) = its out-of-line
//   Reset 0x004195AF, FableWin.pdb ?Reset@?$CCountedPointer@VCPlayerGui@@@@QAEXPAVCPlayerGui@@@Z (stores DeleteData<CPlayerGui> 0x00419091;
//   declared only here, FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL), and read back through the inline Get
//   (?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ). Were the stand-ins g_PlayerGui / CPlayerGuiHolder::Reset.
#define FABLE_RETAIL_SINGLETON_SET
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"

struct CPlayer_Methods : CPlayer {
    void InitPlayerGui();
};

void CPlayer_Methods::InitPlayerGui()
{
    long number = Number;
    if (number == reinterpret_cast<CPlayerManagerCalls*>(PlayerManager)->GetMainPlayerNumber()) {
        CGamePlayerInterface* playerInterface;
        CGraphicDataBank* bank;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(new CPlayerGui(Component, World, playerInterface = PlayerInterface, PlayerManager,
                                         bank = reinterpret_cast<CMainGameComponentBank*>(Component)->GraphicDataBank));
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Open();
    }
}