// CPlayer::UninitModeOracleMinigame(EPlayerMode mode, CProcessedInput* input) -- leaving the mode detaches the oracle-minigame and quick-access-items input processes from the player's interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633BB0, length 29 bytes (padded to 32).
// Leaf: UninitModeOracleMinigame
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// The row is referenced only from the CPlayer mode-table initialiser (0x0121B803), stored right after the (unnamed)
// mode Init @ 0x006367C0 (0x0121B7F9), which attaches InputProcessOracleMinigame +0x1ec and InputProcessQuickAccessItems
// +0x160 -- the two processes this body removes, in the same order. The mode name is inferred from those members.
// Callees: CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (non-virtual, twice).
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members); CPlayer_Methods is the lane's method-carrier
// convention for generated data-only headers (as the landed CPlayer::UninitModeBerserk 0x006339B0).
// Shape: both mode arguments are unused (`ret 8`); the pointers are loaded straight into ecx in member order.
#include "engine/CPlayer.h"
typedef long EPlayerMode;
class CProcessedInput;
class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeOracleMinigame(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeOracleMinigame(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessOracleMinigame_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
}