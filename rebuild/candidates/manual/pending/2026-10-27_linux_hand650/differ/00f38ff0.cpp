// CSurfaceCompositor::SetBaseBlendModes() -- queue the compositor's base blend/texture-stage render states
// Grade: DIFFER(1841v1842) -- register tie only (same family as 009df730): capture blocks swap esi/edx and one block picks edi-vs-edx for the value, 1 byte; global allocator flip
// Retail 0x00F38FF0, 1856 bytes. Same family as CStateBlockFunctionSold::Apply (0x009DF060, landed):
// GFGetSystemManager() (0x009A4EC0) -> system +0x60 -> render system +0x08 is the tracked-state manager,
// and each of the 18 states goes through the inlined "set requested dword state" helper (capture once per
// active capture mask, queue once for realisation). Values are the retail D3D value globals or 0/1.
#include "fable_state_block_sold.h"

extern fable_u32 DAT_01396dc8;   // 0x01396DC8
extern fable_u32 DAT_01396e70;   // 0x01396E70
extern fable_u32 DAT_01396e7c;   // 0x01396E7C
extern fable_u32 DAT_01396eec;   // 0x01396EEC
extern fable_u32 DAT_01396ef0;   // 0x01396EF0
extern fable_u32 DAT_01396fa0;   // 0x01396FA0
extern fable_u32 DAT_01396fa4;   // 0x01396FA4
extern fable_u32 DAT_01396fb8;   // 0x01396FB8

struct CSurfaceCompositor
{
    void SetBaseBlendModes();
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

void CSurfaceCompositor::SetBaseBlendModes()
{
    fable_u8* systemManager = reinterpret_cast<fable_u8*>(GFGetSystemManager());
    fable_u8* renderSystem = StateAt<fable_u8*>(systemManager, 0x60);
    fable_u8* manager = StateAt<fable_u8*>(renderSystem, 0x08);

    QueueTrackedDwordState(manager, 0x2EE4, DAT_01396dc8);
    QueueTrackedDwordState(manager, 0x2F84, DAT_01396dc8);
    QueueTrackedDwordState(manager, 0x2868, 0);
    QueueTrackedDwordState(manager, 0x2890, DAT_01396fb8);
    QueueTrackedDwordState(manager, 0x29E4, 1);
    QueueTrackedDwordState(manager, 0x282C, DAT_01396fa4);
    QueueTrackedDwordState(manager, 0x2818, DAT_01396fa0);
    QueueTrackedDwordState(manager, 0x28B8, 0);
    QueueTrackedDwordState(manager, 0x28CC, 0);
    QueueTrackedDwordState(manager, 0x2840, DAT_01396fa0);
    QueueTrackedDwordState(manager, 0x3344, DAT_01396ef0);
    QueueTrackedDwordState(manager, 0x33E4, DAT_01396eec);
    QueueTrackedDwordState(manager, 0x3204, DAT_01396e7c);
    QueueTrackedDwordState(manager, 0x3484, DAT_01396ef0);
    QueueTrackedDwordState(manager, 0x3524, DAT_01396eec);
    QueueTrackedDwordState(manager, 0x32A4, DAT_01396e7c);
    QueueTrackedDwordState(manager, 0x3218, DAT_01396e70);
    QueueTrackedDwordState(manager, 0x32B8, DAT_01396e70);
}
