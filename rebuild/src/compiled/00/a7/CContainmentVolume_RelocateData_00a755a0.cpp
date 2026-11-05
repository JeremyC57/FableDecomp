// Leaf: ?CContainmentVolume_RelocateData@@YIXPAVCContainmentVolume@@HJPAX1J@Z
#include "engine/CContainmentVolume.h"  // retyped onto the PDB layout; byte parity re-verified

void __fastcall CContainmentVolume_RelocateData(
    CContainmentVolume* volume, int /*edx*/, long a1,
    void* oldBase, void* newBase, long a4)
{
    (void)a1;
    (void)a4;
    if (volume->ContainmentPlanes) {
        volume->ContainmentPlanes = (CPlane*)((char*)volume->ContainmentPlanes
            - (char*)oldBase + (char*)newBase);
    }
}
