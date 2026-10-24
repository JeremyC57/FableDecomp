// CPlayer::UninitModeClickPastText(EPlayerMode mode, CProcessedInput* input) -- leaving the mode detaches the yes/no question and right-stick look-around input processes from the player's interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633670, length 29 bytes (padded to 32).
// Leaf: UninitModeClickPastText
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// The row is referenced only from the CPlayer mode-table initialiser (0x0121B169), stored right after the landed
// CPlayer::InitModeClickPastText @ 0x006359C0 (0x0121B15F), and it removes exactly the two processes that init attaches
// (InputProcessYesNoQuestion +0x144, InputProcessRightStickLookAround +0x170) -- the matching Uninit of that mode.
// Callees: CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (non-virtual, twice).
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members); CPlayer_Methods is the lane's method-carrier
// convention for generated data-only headers (as the landed CPlayer::UninitModeBerserk 0x006339B0).
// Shape: both mode arguments are unused (`ret 8`); the pointers are loaded straight into ecx in member order.
#include "engine/CPlayer.h"
typedef long EPlayerMode;
struct CProcessedInput;
struct CAInputProcess { void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeClickPastText(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeClickPastText(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessYesNoQuestion_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
}