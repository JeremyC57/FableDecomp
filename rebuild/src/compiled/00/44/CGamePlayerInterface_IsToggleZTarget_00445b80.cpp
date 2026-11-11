// CGamePlayerInterface::IsToggleZTarget() const -- the controls definition's ToggleZTarget flag (CControlsDef +0x48 through PControlsDef +0x7a0)
// Leaf: ?IsToggleZTarget@CGamePlayerInterface@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00445B80, 10 bytes: mov eax,[ecx+7A0h] ; mov al,[eax+48h] ; ret.
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?IsToggleZTarget@CGamePlayerInterface@@QBE_NXZ (engine/CGamePlayerInterface.h
//   declares it under FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET). Ego_r IsToggleZTarget 0x004431B0 / IsToggleSneak 0x004431D0 sit
//   in the same order as retail 0x00445B80 / 0x00445BA0, and the flag read is the first of CControlsDef's six toggles (Ego_r
//   ToggleZTarget +0x50, retail +0x48). Was the free stand-in CTCShotDefinitionBase_CanBeHitByMeleeStrike (catalog label propagated).
// Layout: CGamePlayerInterface PControlsDef_Object +0x7a0 (engine/CGamePlayerInterface.h); CControlsDef (no engine header) in its
//   retail layout as the landed sibling 0x00445BA0 models it.
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET
#include "engine/CGamePlayerInterface.h"

class CControlsDef { public:                             // retail layout (Ego_r less the 4-byte def base and STLport vector savings)
    unsigned char _base[0x3c];                           // CDefClassBase
    void*         Controls[3];                           // +0x3c vector<CActionInputControl>
    bool          ToggleZTarget;                         // +0x48
    bool          ToggleSpells;                          // +0x49
    bool          ToggleSneak;                           // +0x4a
    bool          ToggleExpressionMenu;                  // +0x4b
    bool          ToggleExpressionShift;                 // +0x4c
    bool          FlourishNeedsAttackButtonHeld;         // +0x4d
};

bool CGamePlayerInterface::IsToggleZTarget() const
{
    return ((const CControlsDef*)PControlsDef_Object)->ToggleZTarget;
}
