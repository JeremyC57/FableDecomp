// CPlayer::IsMainPlayer at 0x00487520.
// Leaf: ?IsMainPlayer@CPlayer@@QBE_NXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

bool CPlayer::IsMainPlayer() const
{
    long number = Number;
    return number == PlayerManager->GetMainPlayerNumber() ? true : false;
}
