// CLandscapeBackgroundPatch::RegisterForZSorting() -- register the patch with the landscape renderer's z-sorter, which fills ZSorterHandle
// Leaf: RegisterForZSorting
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5D20, 20 bytes, `ret`. Catalogued FUN_00be5d20; named from Ego_r.pdb
//   `?RegisterForZSorting@CLandscapeBackgroundPatch@@QAEXXZ` and its callee
//   `?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscapeBackgroundPatch@@@Z` @ 0x00B67E30
//   (handle by reference, then the patch), on the renderer global 0x01436EA8.
// Layout: engine/CLandscapeBackgroundPatch.h (ZSorterHandle +0x100 agrees with retail).
// Shape: `push ecx` (the patch) then `add ecx,100h ; push ecx` (&ZSorterHandle), then the renderer into ecx.
#include "engine/CLandscapeBackgroundPatch.h"

class CEngineLandscapeRenderer { public:
    void RegisterSubPatchForZSorting(long& handle, CLandscapeBackgroundPatch* patch);   // 0x00B67E30
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;     // 0x01436EA8

struct CLandscapeBackgroundPatchMethods : CLandscapeBackgroundPatch {
    void RegisterForZSorting();
};

void CLandscapeBackgroundPatchMethods::RegisterForZSorting()
{
    g_pLandscapeRenderer->RegisterSubPatchForZSorting(ZSorterHandle, this);
}