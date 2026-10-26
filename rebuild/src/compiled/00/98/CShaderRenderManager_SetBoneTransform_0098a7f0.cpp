// CShaderRenderManager::SetBoneTransform(unsigned long index, const CMatrix3x4* matrix) -- store a bone's 4x3 matrix pre-transposed (three float4 rows) into BoneMatrices[index] and mark the bone constants dirty
// Leaf: SetBoneTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098A7F0, length 104 bytes. No callees.
// Shape: the element address `data + index * 0x30` (`lea eax,[eax+eax*2] ; shl eax,4`) in a reference
// local, twelve dword copies through esi in destination order, then `UpdateFlags |= 0x20` in memory form.
// Layout: PDB BoneMatrices (CArray<CPreTransposedBoneMatrix>) +0x3fc, UpdateFlags +0x3f8; retail +0x3dc
// (the array's data pointer) and +0x3d8. Floats copied as dwords (unsigned int).
class CMatrix3x4 { public:                              // 4 rows of 3 floats
    unsigned int m[12];
};

struct CPreTransposedBoneMatrix {                // 3 rows of 4 floats (0x30 bytes)
    unsigned int m[12];
};

class CShaderRenderManager { public:
    unsigned char             _pad000[0x3d8];
    unsigned int              UpdateFlags;       // +0x3d8
    CPreTransposedBoneMatrix* BoneMatrices;      // +0x3dc (CArray data)
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix);
};

void CShaderRenderManager::SetBoneTransform(unsigned long index, const CMatrix3x4* matrix)
{
    CPreTransposedBoneMatrix& bone = BoneMatrices[index];
    bone.m[0] = matrix->m[0];
    bone.m[1] = matrix->m[3];
    bone.m[2] = matrix->m[6];
    bone.m[3] = matrix->m[9];
    bone.m[4] = matrix->m[1];
    bone.m[5] = matrix->m[4];
    bone.m[6] = matrix->m[7];
    bone.m[7] = matrix->m[10];
    bone.m[8] = matrix->m[2];
    bone.m[9] = matrix->m[5];
    bone.m[10] = matrix->m[8];
    bone.m[11] = matrix->m[11];
    unsigned int* flags = &UpdateFlags;          // a pointer local gives the memory-form `or`
    *flags |= 0x20u;
}