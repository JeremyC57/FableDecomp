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
#ifdef FABLE_RETAIL_C3DBONE2_PEEK
    // Retail opt-in (chunk 2014, row 0x00A50510): FableWin.pdb ?PeekLocalizationMatrix@C3DBone2@@QBEABVCMatrix3x4@@XZ (0x03226620, emitted
    // right after InitialiseAnimationSequences, its first user); inline in retail.
    const CMatrix3x4& PeekLocalizationMatrix() const { return LocalizationMatrix; }
    // Ego_r.pdb / FableWin.pdb ?GetParent@C3DBone2@@QBEJXZ and ?GetOriginalNoChildren@C3DBone2@@QBEJXZ: retail keeps their COMDATs in
    // this TU's inline run (0x00A4BD30 `mov eax,[ecx+4]`, 0x00A4BD40 `mov eax,[ecx+8]`, right ahead of PeekLocalizationMatrix
    // 0x00A4BD50, GetBoneCount 0x00A4BD60 and GetBone 0x00A4BD70).
    long GetParent() const { return Parent; }
    long GetOriginalNoChildren() const { return OriginalNoChildren; }
#endif
    unsigned long NameCRC;                               // +0x00
    long          Parent;                                // +0x04
    long          OriginalNoChildren;                    // +0x08
    CMatrix3x4    LocalizationMatrix;                    // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(C3DBone2) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DBone2, LocalizationMatrix) == 0xc);
#endif
