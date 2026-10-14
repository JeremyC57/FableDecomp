// CGameScriptInterface::UnfreezeControls() -- unfreezes the current player's controls unless the main player's controlled creature is missing or has flag bit 0 at +0x91 set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E73E0, length 48 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`GetPlayer(MainPlayer)`), CPlayer::GetControlledCreature @ 0x00487DD0
// (`add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`, the PDB's ControlledCreature at +0x2c), CPlayerManager method
// @ 0x00449A50 taking the player index (manifest "CEngine::GetEditorAnimation" is propagated over both twins).
// Twin of 0x006E73B0 (which calls 0x00449A30): both carry the manifest label FreezeControls; the second is named for the
// adjacent manager routine (Unfreeze is inferred, not proven).
// Layout from engine/CGameScriptInterface.h: PlayerManager +0x14, CurrentPlayer +0x1c.
// Shape: the creature pointer is tested, its +0x91 bit 0 tested (`test byte ptr [eax+91h],1`), then the index is read before
// the manager for the final thiscall.
#include "engine/CGameScriptInterface.h"

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = controls already locked)
};

struct CPlayer {
    CThing* GetControlledCreature();                // 0x00487DD0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
    void UnfreezePlayerControls(long player);       // 0x00449A50
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void UnfreezeControls();
};

void CGameScriptInterface_Methods::UnfreezeControls()
{
    CThing* creature = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (creature && !(creature->m_stateFlags & 1))
        PlayerManager->UnfreezePlayerControls(CurrentPlayer);
}
