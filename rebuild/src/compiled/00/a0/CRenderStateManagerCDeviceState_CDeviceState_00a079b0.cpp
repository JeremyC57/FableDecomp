// CRenderStateManager::CDeviceState::CDeviceState() -- retail 0x00A079B0, 4950 bytes.
// Constructs the 231 CStateInfo slots of the device-state mirror (55 scalar states + 22 per-stage arrays of 8):
// each element's inline CStateInfo ctor zeroes CurrentState, DesiredState, BookmarkMask, DirtyListFlag and StateType.
// No callees (CStateInfo::CStateInfo is inline in release; FableWin debug body 0x030EC910). Retail shows the ctor
// inline and visible: 0x00A079B0 itself contains no call, and the sole caller CRenderStateManager::CRenderStateManager
// (0x00A05970) expands the same five zero stores for BookmarkDummyState at +0x3A24 with no call at 0x00A05989..0x00A059A3
// (xor eax,eax; +0x3A24/+0x3A28/+0x3A2C dwords, +0x3A34/+0x3A35 bytes). The inline body below copies the identical
// member-init list of the landed row rebuild/src/compiled/00/a0/CRenderStateManager_CRenderStateManager_00a05970.cpp.
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