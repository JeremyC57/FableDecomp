// C3DMesh2::LoadBinary -- load mesh counts, resource blocks, bones, materials and primitives.
// Leaf: ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z
// Retail: 0x00A8AD40,1621bytes,thiscall ret16.
// Original names: both PDBs and actual donor procedure; use shared complete native class models.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_LOCK
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_LOADBINARY
#define FABLE_RETAIL_C3DMESH2_LOADBINARY
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_BUILDMESH
#define FABLE_RETAIL_C3DMESH2_INVERSETRANSFORM
#define FABLE_RETAIL_CMATRIX3X4
#include <string.h>
#ifdef _MSC_VER
typedef __int64 FableSigned64;
#else
typedef long long FableSigned64;
#endif
#include "engine/CMatrix3x4.h"
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/C3DMesh2.h"
#include "engine/CScopedLockFlagBool.h"

bool C3DMesh2::LoadBinary(CMemoryDataInputStream& is, CMovableResourceMemoryPool* pool, const CGraphicDataBank* texture_bank, bool discard_primitives)
{
    CScopedLockFlagBool lock(LockedFromMemoryRelocation);
    MemoryPool = pool;
    if (!C3DMeshStats::LoadBinary(is)) {
        return false;
    }
    MaterialCount = is.ReadULONG();
    PrimitiveCount = is.ReadULONG();
    BoneCount = is.ReadULONG();
    BoneNameDataSize = is.ReadULONG();
    ClothFlag = is.ReadEBOOL();
    unsigned long static_block_count = is.ReadUWORD();
    unsigned long anim_block_count = is.ReadUWORD();
    if (discard_primitives)
        PrimitiveCount = 0;
    FableSigned64 overflow_check_1 = BoneCount * ((0x3c + 0x30 + 0x40))
                           + MaterialCount * sizeof(C3DMeshMaterial) + PrimitiveCount * sizeof(C3DPrimitive2);
    if (overflow_check_1 < 0 || overflow_check_1 > 0x7fffffff) {
        return false;
    }
    FableSigned64 overflow_check_2 = static_block_count * 0x20 + anim_block_count * 0x30;
    if (overflow_check_2 < 0 || overflow_check_2 > 0x7fffffff) {
        return false;
    }
    FableSigned64 overflow_check_3 = BoneNameDataSize + BoneCount * sizeof(unsigned short);
    if (overflow_check_3 < 0 || overflow_check_3 > 0x7fffffff) {
        return false;
    }
    unsigned long buffer_size = BoneCount * ((0x3c + 0x30 + 0x40))
                           + MaterialCount * sizeof(C3DMeshMaterial) + PrimitiveCount * sizeof(C3DPrimitive2);
    unsigned long prim_buffer_size = static_block_count * 0x20 + anim_block_count * 0x30;
    unsigned long bone_name_buffer_size = BoneNameDataSize + BoneCount * sizeof(unsigned short);
    if (!MemoryPool->AllocateResource(&MemoryHandle, this, buffer_size, 0)) {
        return false;
    }
    if (!MemoryPool->AllocateResource(&PrimMemoryHandle, this, prim_buffer_size, 0)) {
        return false;
    }
    if (bone_name_buffer_size && !MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, bone_name_buffer_size, 0)) {
        return false;
    }
    InitialisePointers(BoneCount, buffer_size);
    InitialiseBoneNamePointers(bone_name_buffer_size);
    is.Read(BoneNameIndices, BoneCount * sizeof(unsigned short));
    is.ReadCompressed(BoneNameData, BoneNameDataSize);
    char* current_bone_name = BoneNameData;
    for (unsigned long c_bone = 0; c_bone < BoneCount; ++c_bone) {
        BoneNameIndices[c_bone] = (unsigned short)(current_bone_name - BoneNameData);
        current_bone_name += strlen(current_bone_name) + 1;
    }
    is.ReadCompressed(Bones, BoneCount * 0x3c);
    is.ReadCompressed(BoneKeyframes, BoneCount * 0x30);
    is.ReadCompressed(BoneSpaceTransforms, BoneCount * 0x40);
    is.Read(&InverseTransform, sizeof(InverseTransform));
    for (unsigned long c_material = 0; c_material < MaterialCount; ++c_material)
        Materials[c_material].LoadBinary(is, texture_bank);
    if (discard_primitives)
        return true;
    unsigned char* prim_dest = (unsigned char*)pool->GetPointerFromHandle(PrimMemoryHandle);
    MemoryPool->Lock(MemoryHandle);
    MemoryPool->Lock(PrimMemoryHandle);
    for (unsigned long c_prim = 0; c_prim < PrimitiveCount; ++c_prim) {
        if (!Primitives[c_prim].LoadBinary(is, 0, &prim_dest, MemoryPool)) {
            MemoryPool->Unlock(PrimMemoryHandle);
            MemoryPool->Unlock(MemoryHandle);
            return false;
        }
    }
    MemoryPool->Unlock(PrimMemoryHandle);
    MemoryPool->Unlock(MemoryHandle);
    return true;
}