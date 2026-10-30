// NPlayerGui::CDrawAbilityButtons::GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def) -- the special-move slot the main player's interface assigns to this button def's controller button
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643FC8, 28 bytes (/Os module, unaligned). Virtual slot 3 of
// NPlayerGui::CDrawAbilityButtons' vtable 0x0125A6C8; the PC subclass's slot 3 (0x00657200) is the same body reading the def's
// mouse button (+0x44) and calling GetAssignableSpecialMoveIndexForMouseButton 0x00445F40. The method name is descriptive.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`mov eax,[ecx+1Ch] ; push eax ; call GetPlayer`), CGamePlayerInterface::
// GetAssignableSpecialMoveIndexForButton(EXboxControllerButton) @ 0x00445EE0 (`ret 4`, long result).
// Layout: PlayerManager +0x08 (as the landed CDrawAbilityButtons::DrawAbilityList 0x0064DB2B); CPlayer::PlayerInterface +0x0c
// (engine/CPlayer.h); CAbilityButtonGuiDef ControllerType/ControllerButton at retail +0x40/+0x44 is PDB +64/+68 shifted 4 down
// (the def base is 4 bytes smaller in the shipped build), so ControllerButton is +0x40.
// Shape: the interface goes into a NAMED local first (`mov eax,[eax+0Ch]` after GetMainPlayer, then the argument push, then
// `mov ecx,eax`); the one-expression chain pushes the argument before GetMainPlayer. The long result is returned (`ret 4`).
#pragma optimize("s", on)
#include "engine/CPlayer.h"

enum EXboxControllerButton { XBOX_BUTTON_NONE };

class CGamePlayerInterface { public:
    long GetAssignableSpecialMoveIndexForButton(EXboxControllerButton button);   // 0x00445EE0
};
class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CAbilityButtonGuiDef { public:
    unsigned char         _def00[0x40];            // CGameDefinition base + gui def members
    EXboxControllerButton ControllerButton;        // +0x40 (PDB +68)
    long                  MouseButton;             // +0x44 (PDB +72)
};

namespace NPlayerGui {
class CDrawAbilityButtons { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0
    virtual void ClearPrimitives();                // slot 1
    virtual void v02();                            // slot 2
    virtual long GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def);   // slot 3
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
};

long CDrawAbilityButtons::GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def)
{
    CGamePlayerInterface* playerInterface = PlayerManager->GetMainPlayer().PlayerInterface;
    return playerInterface->GetAssignableSpecialMoveIndexForButton(def.ControllerButton);
}
}