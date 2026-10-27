// CWorld::SetStartTime(double start_time) -- store the game start time
// Leaf: ?SetStartTime@CWorld@@QAEXN@Z
// Retail: Fable.exe @ 0x0049CA20, 13 bytes, __fastcall `ret 8`: `fld qword [esp+4] ; fstp qword [ecx+180h]`.
// Identity (probable): FableWin.pdb names ?SetStartTime@CWorld@@QAEXN@Z (thiscall, one double, void) and engine/CWorld.h puts
// StartTime (Ego_r double) at retail +0x180 -- the landed CWorld ctor 0x004A67D0 zeroes TimePlayed at +0x188 right after it.
// Only the double is written (HasInitialisedStartTime +0x190 untouched). No direct caller (gap-scan, unreferenced).
#define CWorld DonorCWorld
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public DonorCWorld { public:               // engine/CWorld.h: StartTime +0x180
    void SetStartTime(double start_time);
};

void CWorld::SetStartTime(double start_time)
{
    StartTime = start_time;
}