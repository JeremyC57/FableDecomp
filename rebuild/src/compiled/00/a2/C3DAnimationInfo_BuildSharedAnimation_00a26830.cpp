// C3DAnimationInfo::BuildSharedAnimation() const -- make sure the animation data is loading, take it off the unload list and build its shared animation
// Leaf: ?BuildSharedAnimation@C3DAnimationInfo@@ABEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26830.
// Identity: Ego_r.pdb ?BuildSharedAnimation@C3DAnimationInfo@@ABEXXZ (private const).
// Callees: CResource::IsLoading / BeginLoading, CResource::RemoveFromResourceList 0x009FC440, C3DAnimationData::BuildSharedAnimation.
// Shape: `if (!IsLoading()) BeginLoading(true); Data->RemoveFromResourceList(); Data->BuildSharedAnimation();`.
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