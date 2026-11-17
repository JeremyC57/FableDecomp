//   Global byte 0x013756E9 = the debug-helper switch.
// Shape: IsThingMainPlayer's argument is pushed before the player-manager call (object expression evaluated after the arguments);
//   STLport erase(begin(), end()) expands the trivial copy (`last == first` early-out, memmove + byte count) and sets the finish.
// Name fix (NF59, queue q10517; NF55 porting 0x00725C40 Move): Ego_r.pdb 0x0073CBC0 + FableWin.pdb 0x02528460
//   ?AdjustThingListForDebugHelpers@CTCPhysicsControlled@@AAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z (private).
//   The local CTCBase / CTCPhysicsStandard / CPlayerManager / CThingVector models (a public member over a made-up vector struct) are
//   replaced by NF55's 0x00725C40 include set (engine/CTCPhysicsControlled.h FABLE_RETAIL_CTCPHYSICSCONTROLLED_MOVE declares the member,
//   stlport/fable_containers.h the vector) plus the retail engine/CPlayerManager.h; the parent comes from CTCBase::GetParentThing (+0x04).
// Leaf: ?AdjustThingListForDebugHelpers@CTCPhysicsControlled@@AAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
#define FABLE_RETAIL_CTCPHYSICSCONTROLLED
#define FABLE_RETAIL_CTCPHYSICSCONTROLLED_MOVE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERMANAGER
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CTCPhysicsControlled.h"
#include "engine/CPlayerManager.h"

extern bool g_bDebugHelpers;                // 0x013756E9

void CTCPhysicsControlled::AdjustThingListForDebugHelpers(std::vector<CThing*>& things)
{
    if (GetPlayerManager().IsThingMainPlayer(GetParentThing()) && !g_bDebugHelpers)
        things.erase(things.begin(), things.end());
}
