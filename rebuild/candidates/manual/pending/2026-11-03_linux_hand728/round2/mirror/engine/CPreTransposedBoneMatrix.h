#ifndef FABLE_ENGINE_CPRETRANSPOSEDBONEMATRIX_H
#define FABLE_ENGINE_CPRETRANSPOSEDBONEMATRIX_H
// CPreTransposedBoneMatrix (chunk 723b): Ego_r.pdb struct (U-keyed, pdb_class_keys.tsv; 0x30 bytes: float Data[3][4]). The empty
// user constructor is what retail's CShaderRenderManager::SetBoneMatrixCount expands (BoneMatrices.resize(n, CPreTransposedBoneMatrix())
// with no fill of the temporary: 0x00B77E20).
#include "rebuild_abi.h"
struct CPreTransposedBoneMatrix {
    CPreTransposedBoneMatrix() { }
    float Data[3][4];                                                    // +0x00
};
FABLE_STATIC_ASSERT(sizeof(CPreTransposedBoneMatrix) == 0x30);
#endif // FABLE_ENGINE_CPRETRANSPOSEDBONEMATRIX_H
