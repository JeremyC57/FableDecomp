// CGameScriptInterface::FreezeControls() -- freezes the current player's controls unless the main player's controlled creature is missing or has flag bit 0 at +0x91 set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E73B0, length 48 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`GetPlayer(MainPlayer)`), CPlayer::GetControlledCreature @ 0x00487DD0
// (`add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`, the PDB's ControlledCreature at +0x2c), CPlayerManager method
// @ 0x00449A30 taking the player index (manifest "CEngine::GetEditorAnimation" is propagated over both twins).
// Layout from engine/CGameScriptInterface.h: PlayerManager +0x14, CurrentPlayer +0x1c.
// Shape: the creature pointer is tested, its +0x91 bit 0 tested (`test byte ptr [eax+91h],1`), then the index is read before
// the manager for the final thiscall.
#include "engine/CGameScriptInterface.h"

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = controls already locked)
};

class CPlayer { public:
    CThing* GetControlledCreature();                // 0x00487DD0
};

class CPlayerManager { public:
    void FreezePlayerControls(long player);         // 0x00449A30
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void FreezeControls();
};

void CGameScriptInterface_Methods::FreezeControls()
{
    CThing* creature = PlayerManager->GetMainPlayer().GetControlledCreature();
    if (creature && !(creature->m_stateFlags & 1))
        PlayerManager->FreezePlayerControls(CurrentPlayer);
}