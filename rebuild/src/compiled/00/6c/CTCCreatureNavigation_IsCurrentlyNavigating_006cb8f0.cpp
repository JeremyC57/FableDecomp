// CTCCreatureNavigation::IsCurrentlyNavigating() const -- navigated this frame or the one before
// Leaf: ?IsCurrentlyNavigating@CTCCreatureNavigation@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB8F0, 39 bytes, plain `ret`. Ego_r.pdb ?IsCurrentlyNavigating@CTCCreatureNavigation@@QBE_NXZ
// (the catalog name; landed as a free __fastcall stub with an invented helperA over a local pad struct): retail sits right after
// GetNearestNavigablePosTo 0x006CB8E0 as in the Ego_r donor order, and reads FrameLastNavigated +0x50 (engine/CTCCreatureNavigation.h).
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused), CWorld::GetFrame() @ 0x0049D870 (static __fastcall).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ISNAVIGATING
#include "engine/ETCInterfaceType.h"
class C3DVector;
#include "engine/CTCCreatureNavigation.h"
#include "engine/CWorld.h"

bool CTCCreatureNavigation::IsCurrentlyNavigating() const
{
    return FrameLastNavigated >= 0 && GetWorld().GetFrame() - FrameLastNavigated <= 1;
}
