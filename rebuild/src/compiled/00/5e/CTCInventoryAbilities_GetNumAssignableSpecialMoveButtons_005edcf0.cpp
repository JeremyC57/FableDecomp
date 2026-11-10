// CTCInventoryAbilities::GetNumAssignableSpecialMoveButtons() const -- the main player's interface's count of assignable special-move buttons
// Leaf: ?GetNumAssignableSpecialMoveButtons@CTCInventoryAbilities@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EDCF0, thiscall, a tail jump.
// Identity (proven): Ego_r.pdb and FableWin.pdb both list ?GetNumAssignableSpecialMoveButtons@CTCInventoryAbilities@@QBEJXZ, and the
//   landed CTCInventoryAbilities rows (0x005F6BD6 SetAbilityAsAvailable via engine/CTCInventoryAbilities.h BUTTONS) call 0x005EDCF0
//   under it; the body is the three-call chain that name implies.
// Callees: CTCBase::PeekPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960, then the CPlayer's
//   PlayerInterface (+0x0C, read in place: FableWin's inline GetPlayerInterface) and a tail jump to
//   CGamePlayerInterface::GetNumberOfAssignableSpecialMoveButtons() const 0x00445940.
// Shape: `call 0x00686D50; mov ecx,eax; call 0x00449960; mov ecx,[eax+0Ch]; jmp 0x00445940`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#include "fable_string.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"

long CTCInventoryAbilities::GetNumAssignableSpecialMoveButtons() const
{
    return PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons();
}
