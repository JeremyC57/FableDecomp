// C3DAnimationInfo::Preload() const -- preload the animation: start loading if needed, keep it off the unload list and build the shared animation (BuildSharedAnimation expanded)
// Leaf: ?Preload@C3DAnimationInfo@@QBEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26AA0.
// Identity: Ego_r.pdb ?Preload@C3DAnimationInfo@@QBEXXZ (donor order after TouchAnimation).
// Shape: `BuildSharedAnimation();` with the private const member body in the TU (inlined).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DANIMATIONINFO
#include "engine/C3DAnimationInfo.h"

void C3DAnimationInfo::BuildSharedAnimation() const
{
    if (!IsLoading())
        BeginLoading(true);
    Data->RemoveFromResourceList();
    Data->BuildSharedAnimation();
}

void C3DAnimationInfo::Preload() const
{
    BuildSharedAnimation();
}