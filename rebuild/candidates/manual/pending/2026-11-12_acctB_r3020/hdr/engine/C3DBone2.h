#ifndef FABLE_ENGINE_C3DBONE2_H
#define FABLE_ENGINE_C3DBONE2_H
// New header (chunk 2005, row 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation): Ego_r.pdb C3DBone2 (0x3C bytes, no containers so
// retail = Ego_r): NameCRC +0, Parent +4, OriginalNoChildren +8, LocalizationMatrix +0xC (CMatrix3x4). Retail evidence: the mesh's
// bone array (C3DMesh2::Bones +0x9C) is walked with a 0x3C stride, Parent read at +4 (`cmp eax,-1`), the matrix at +0xC
// (0x00AB2617..0x00AB2749, 0x00AB2759 `imul eax,eax,3Ch` ... `lea eax,[eax+ecx+0Ch]`).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"
class C3DBone2 {
public:
    unsigned long NameCRC;                               // +0x00
    long          Parent;                                // +0x04
    long          OriginalNoChildren;                    // +0x08
    CMatrix3x4    LocalizationMatrix;                    // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(C3DBone2) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DBone2, LocalizationMatrix) == 0xc);
#endif
