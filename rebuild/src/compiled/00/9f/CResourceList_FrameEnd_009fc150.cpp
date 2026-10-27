// CResourceList::FrameEnd() -- advances the frame counter and clears the per-frame unload count
// Retail: Fable.exe 0x009FC150, 11 bytes: `inc dword ptr [ecx+2Ch] ; mov dword ptr [ecx+38h],0 ; ret`.
// Identity: catalog name; Ego_r.pdb ?FrameEnd@CResourceList@@QAEXXZ. Members from engine/CResourceList.h:
// CurrentFrame +0x2C, UnloadedThisFrame +0x38.
// Leaf: ?FrameEnd@CResourceList@@QAEXXZ
#define CResourceList DonorCResourceList
#include "engine/CResourceList.h"
#undef CResourceList

class CResourceList : public DonorCResourceList {
public:
    void FrameEnd();
};

void CResourceList::FrameEnd()
{
    ++CurrentFrame;
    UnloadedThisFrame = 0;
}