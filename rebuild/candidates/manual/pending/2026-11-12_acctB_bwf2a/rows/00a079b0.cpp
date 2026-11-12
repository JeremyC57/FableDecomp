// CRenderStateManager::CDeviceState::CDeviceState() -- retail 0x00A079B0, 4950 bytes.
// Constructs the 231 CStateInfo slots of the device-state mirror (55 scalar states + 22 per-stage arrays of 8):
// each element's inline CStateInfo ctor zeroes CurrentState, DesiredState, BookmarkMask, DirtyListFlag and StateType.
// No callees (CStateInfo::CStateInfo is inline in release; FableWin debug body 0x030EC910).
// Leaf: ??0CDeviceState@CRenderStateManager@@QAE@XZ
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#include "engine/CRenderStateManager.h"

inline CRenderStateManager::CStateInfo::CStateInfo()
    : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0)
{
}

CRenderStateManager::CDeviceState::CDeviceState()
{
}
