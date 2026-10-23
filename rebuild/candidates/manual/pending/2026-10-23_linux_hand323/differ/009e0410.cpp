// CStateBlockFunctionAlphaBoolean::Apply() -- queue the alpha-blend render states of this state-block function on the render state manager
// Grade: DIFFER(1738v1738) -- register tie only: capture blocks swap esi/edx ([+0x2814] vs [state+8]) from block 7/8 on; allocator flip point is global (8 helper forms, per-block mixes, member-function manager model all move it chaotically). Landed twin 009df060 Sold also regrades DIFFER in this lane
// Retail 0x009E0410, 1744 bytes (virtual Apply, ?Apply@CStateBlockFunctionAlphaBoolean@@UAEXXZ).
//
// Sibling of the landed CStateBlockFunctionSold::Apply (0x009DF060): GFGetSystemManager()
// (0x009A4EC0, fastcall) -> system +0x60 -> render system +0x08 is the tracked-state manager.
// Each tracked dword state is a 0x14-byte record; QueueTrackedDwordState is the inlined
// "set requested value" helper: when the value changes, the state is captured once per active
// capture mask (+0x2814, stack at +0x0008, count +0x2808) and queued once for realisation
// (dirty list at +0x2008, count +0x280C). Values come from the retail D3D value globals.
#include "fable_state_block_sold.h"

extern fable_u32 DAT_01396de8;   // 0x01396DE8
extern fable_u32 DAT_01396e58;   // 0x01396E58
extern fable_u32 DAT_01396e70;   // 0x01396E70
extern fable_u32 DAT_01396e7c;   // 0x01396E7C
extern fable_u32 DAT_01396eec;   // 0x01396EEC
extern fable_u32 DAT_01396ef0;   // 0x01396EF0
extern fable_u32 DAT_01396f78;   // 0x01396F78
extern fable_u32 DAT_01396f7c;   // 0x01396F7C

struct CStateBlockFunctionAlphaBoolean
{
    virtual void Apply();
};

template <typename T>
static __forceinline T& StateAt(fable_u8* base, fable_u32 offset)
{
    return *reinterpret_cast<T*>(base + offset);
}

static __forceinline void QueueTrackedDwordState(
    fable_u8* manager, fable_u32 stateOffset, fable_u32 requestedValue)
{
    CTrackedDwordStateSoldView* state =
        reinterpret_cast<CTrackedDwordStateSoldView*>(manager + stateOffset);
    if (state->requestedValue04 == requestedValue)
    {
        return;
    }

    const fable_u32 activeCaptureMask = StateAt<fable_u32>(manager, 0x2814);
    const fable_u32 stateCaptureMask = state->captureMask08;
    if ((activeCaptureMask & stateCaptureMask) == 0)
    {
        state->captureMask08 = stateCaptureMask | activeCaptureMask;

        fable_u32& captureCount = StateAt<fable_u32>(manager, 0x2808);
        CCapturedDwordStateSoldView* captureStack =
            reinterpret_cast<CCapturedDwordStateSoldView*>(manager + 0x0008);
        captureStack[captureCount].state00 = state;
        captureStack[captureCount].priorRequestedValue04 = state->requestedValue04;
        ++captureCount;
    }

    state->requestedValue04 = requestedValue;
    if (state->queuedForRealise10 == 0)
    {
        state->queuedForRealise10 = 1;

        fable_u32& dirtyCount = StateAt<fable_u32>(manager, 0x280C);
        CTrackedDwordStateSoldView** dirtyStates =
            reinterpret_cast<CTrackedDwordStateSoldView**>(manager + 0x2008);
        dirtyStates[dirtyCount] = state;
        ++dirtyCount;
    }
}

void CStateBlockFunctionAlphaBoolean::Apply()
{
    fable_u8* systemManager = reinterpret_cast<fable_u8*>(GFGetSystemManager());
    fable_u8* renderSystem = StateAt<fable_u8*>(systemManager, 0x60);
    fable_u8* manager = StateAt<fable_u8*>(renderSystem, 0x08);

    QueueTrackedDwordState(manager, 0x291C, DAT_01396f78);
    QueueTrackedDwordState(manager, 0x2930, DAT_01396f7c);
    QueueTrackedDwordState(manager, 0x2868, 1);
    QueueTrackedDwordState(manager, 0x2854, 1);
    QueueTrackedDwordState(manager, 0x28B8, 1);
    QueueTrackedDwordState(manager, 0x28CC, 0);
    QueueTrackedDwordState(manager, 0x28A4, DAT_01396de8);
    QueueTrackedDwordState(manager, 0x3344, DAT_01396ef0);
    QueueTrackedDwordState(manager, 0x33E4, DAT_01396eec);
    QueueTrackedDwordState(manager, 0x3484, DAT_01396ef0);
    QueueTrackedDwordState(manager, 0x3524, DAT_01396eec);
    QueueTrackedDwordState(manager, 0x3204, DAT_01396e7c);
    QueueTrackedDwordState(manager, 0x32A4, DAT_01396e7c);
    QueueTrackedDwordState(manager, 0x3218, DAT_01396e70);
    QueueTrackedDwordState(manager, 0x32B8, DAT_01396e70);
    QueueTrackedDwordState(manager, 0x30C4, DAT_01396e58);
    QueueTrackedDwordState(manager, 0x3164, DAT_01396e58);
}
