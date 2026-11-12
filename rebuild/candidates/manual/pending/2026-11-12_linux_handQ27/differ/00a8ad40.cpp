// C3DMesh2::LoadBinary(CMemoryDataInputStream& in, CMovableResourceMemoryPool* pool, const CGraphicDataBank* bank, bool skipPrimitives) -- load the mesh stats, the counts, allocate the three resource blocks, read bone tables and names, the materials and (unless skipped) the primitives, with the mesh locked against relocation throughout
// Grade: DIFFER(1621v1621) register allocation only: retail keeps vertexCount in edi / indexCount in edx and spills mainSize to [esp+18h], ours spills vertexCount to [esp+10h]; knock-on primData slot (0x28 v 0x14) and prologue push ebp position (ushort counts, top decls, term order, store order, helper tried)
// Leaf: ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8AD40, 1621 bytes, thiscall, `ret 10h`.
// Identity: Ego_r.pdb / FableWin.pdb ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z
//   (the draft's CBankFile* bank and pointer stream were guesses).
// Callees: C3DMeshStats::LoadBinary 0x00A2F2D0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inside the inline
//   Read / ReadULONG / ReadEBOOL / ReadUWORD), stream slot 4 ReadCompressed (+0x10; CMemoryDataInputStream 0x00995B10),
//   CMovableResourceMemoryPool::AllocateResource 0x00A62E40, C3DMesh2::InitialisePointers 0x00A896A0 /
//   InitialiseBoneNamePointers 0x00A892E0, C3DMeshMaterial::LoadBinary 0x00ABF100, CHandleBasedMemoryPool::GetPointerFromHandle
//   0x00A026B0 / Lock 0x00A026F0 / Unlock 0x00A02710, C3DPrimitive2::LoadBinary 0x00A92900, strlen (intrinsic).
// Layout: engine/C3DMesh2.h retail branch (base C3DMeshStats 0x7c with MemoryPool +0x04; MemoryHandle +0x7c ..
//   LockedFromMemoryRelocation +0xe5); C3DBone2 0x3c, C3DKeyframe 0x30, CMatrix4x4 0x40 (no headers: the sizes are literal).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_LOCK
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_LOADBINARY
#define FABLE_RETAIL_C3DMESH2_LOADBINARY
#include <string.h>
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/C3DMesh2.h"

bool C3DMesh2::LoadBinary(CMemoryDataInputStream& in, CMovableResourceMemoryPool* pool, const CGraphicDataBank* bank, bool skipPrimitives)
{
    LockedFromMemoryRelocation = true;
    MemoryPool = pool;
    if (!C3DMeshStats::LoadBinary(in)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    MaterialCount = in.ReadULONG();
    PrimitiveCount = in.ReadULONG();
    BoneCount = in.ReadULONG();
    BoneNameDataSize = in.ReadULONG();
    ClothFlag = in.ReadEBOOL();
    unsigned long vertexCount = in.ReadUWORD();
    unsigned long indexCount = in.ReadUWORD();
    if (skipPrimitives)
        PrimitiveCount = 0;
    unsigned long mainSize = BoneCount * ((0x3c + 0x30 + 0x40))
                           + MaterialCount * sizeof(C3DMeshMaterial) + PrimitiveCount * sizeof(C3DPrimitive2);
    __int64 mainSize64 = mainSize;
    if (mainSize64 < 0 || mainSize64 > 0x7fffffff) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    unsigned long primSize = vertexCount * 0x20 + indexCount * 0x30;
    __int64 primSize64 = primSize;
    if (primSize64 < 0 || primSize64 > 0x7fffffff) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    unsigned long nameSize = BoneNameDataSize + BoneCount * sizeof(unsigned short);
    __int64 nameSize64 = nameSize;
    if (nameSize64 < 0 || nameSize64 > 0x7fffffff) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (!MemoryPool->AllocateResource(&MemoryHandle, this, mainSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (!MemoryPool->AllocateResource(&PrimMemoryHandle, this, primSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (nameSize && !MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, nameSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    InitialisePointers(BoneCount, mainSize);
    InitialiseBoneNamePointers(nameSize);
    in.Read(BoneNameIndices, BoneCount * sizeof(unsigned short));
    in.ReadCompressed(BoneNameData, BoneNameDataSize);
    const char* name = BoneNameData;
    for (unsigned long i = 0; i < BoneCount; ++i) {
        BoneNameIndices[i] = (unsigned short)(name - BoneNameData);
        name += strlen(name) + 1;
    }
    in.ReadCompressed(Bones, BoneCount * 0x3c);
    in.ReadCompressed(BoneKeyframes, BoneCount * 0x30);
    in.ReadCompressed(BoneSpaceTransforms, BoneCount * 0x40);
    in.Read(&InverseTransform, sizeof(InverseTransform));
    for (unsigned long m = 0; m < MaterialCount; ++m)
        Materials[m].LoadBinary(in, bank);
    if (!skipPrimitives) {
        unsigned char* primData = (unsigned char*)pool->GetPointerFromHandle(PrimMemoryHandle);
        MemoryPool->Lock(MemoryHandle);
        MemoryPool->Lock(PrimMemoryHandle);
        for (unsigned long p = 0; p < PrimitiveCount; ++p) {
            if (!Primitives[p].LoadBinary(in, 0, &primData, MemoryPool)) {
                MemoryPool->Unlock(PrimMemoryHandle);
                MemoryPool->Unlock(MemoryHandle);
                LockedFromMemoryRelocation = false;
                return false;
            }
        }
        MemoryPool->Unlock(PrimMemoryHandle);
        MemoryPool->Unlock(MemoryHandle);
    }
    LockedFromMemoryRelocation = false;
    return true;
}
