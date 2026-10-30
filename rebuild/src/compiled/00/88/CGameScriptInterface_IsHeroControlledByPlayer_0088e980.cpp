// CGameScriptInterface::IsHeroControlledByPlayer() const -- false for the player modes 4..15, 17, 18, 21, 23, 26, 27, 29, 30, 32, 36, 37 and 41 (CPlayer::GetCurrentMode @ 0x00633BE0 on the player from the manager at +0x14, 0x00449970); true for every other mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E980, length 96 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (on PlayerManager), CPlayer::GetCurrentMode @ 0x00633BE0. Shape: a `switch` on `mode - 4` over 0..0x25 through a byte-indexed two-entry jump table (`xor al,al` / `mov al,1`), the out-of-range default true.
struct CDisplayEngine_Retail;
class CPlayerManager;
class CWorld;
enum EPlayerMode { };
class CPlayer { public:
    EPlayerMode GetCurrentMode() const;            // 0x00633BE0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                      // 0x00449970
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::IsHeroControlledByPlayer() const
{
    switch (PlayerManager->GetMainPlayer().GetCurrentMode()) {
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
    case 17: case 18: case 21: case 23: case 26: case 27: case 29: case 30: case 32: case 36: case 37: case 41:
        return false;
    }
    return true;
}