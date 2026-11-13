// C3DMeshLODInfo::BuildMesh(bool, bool) -- build this LOD's C3DMesh2 from the loaded bank-file bytes: swap the bank's mesh-data /
//   cloth pools into the global pools, make room in the stats resource list for LOD 4, load the mesh (on failure: a fresh mesh,
//   defragment the pool, reopen the stream and retry once without the flag), stamp the next mesh id, compute the texture-change
//   flags when the bank has a texture bank, initialise the mesh generators, then upload every primitive's buffers by entry type
//   (animated / repeated / type 4 / static; bump flag 2 when the material maps bit 2 and the bank enables DP3 bump), restore the
//   pools; on failure release the mesh and return false, else add the LOD to its bank's resource bank, drop the file data and
//   record the resource size (the inline C3DMeshLODInfo::Sizeof).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24520, length 847 bytes (`ret 8`; the first bool is unused).
// Callees: CBankFile::GetEntryDataType 0x009CC210 / GetEntrySymbol 0x009CC1D0, CBankFileAsyncData::GetData 0x009D5530,
//   CMemoryDataInputStream ctor 0x00994930 / OpenMemorySource 0x00994570, ~CDataStream 0x00993B10, ~CCharString 0x0099EAE0,
//   CResourceList::MakeFreeSpace 0x009FC4F0, operator new 0x00BFEA1A, C3DMesh2 ctor 0x00A89190 / LoadBinary 0x00A8AD40 /
//   CalculateTextureChangeFlags 0x00A89380 / LockPrimitive 0x00A89250 / UnlockPrimitive 0x00A892A0 / Sizeof 0x00A893C0,
//   CCountedPointer<C3DMesh2>::Reset 0x00A299C0, CHandleBasedMemoryPool::Defragment 0x00A02730,
//   C3DMeshStats::InitialiseMeshGenerators 0x00A2EC40, C3DPrimitive2::UploadAnimatedBuffers 0x00A92870 / UploadRepeatedBuffers
//   0x00A932C0 / UploadStaticBuffers 0x00A93320, CResourceBank::AddResource 0x009FC380, CBankFileAsyncData::Sizeof 0x009D5620,
//   CResource::SetResourceSize 0x009FBF20, operator delete 0x00BFE9BC.
// Shape: no /EH (no SEH frame despite the two destructed locals), as the same-file siblings 0x00A24D70 / 0x00A26AD0. The upload
//   result is folded with `ok &=` in each arm (one `and bl,al` tail): with a shared `result` local VC spills ok instead of the
//   primitive index. The mesh release on failure is the header's default ReleaseData (Info zeroed first); FileData.Reset(0) and
//   Sizeof() are expanded inline. g_MeshDataMemoryPool (0x013D2D6C) is a descriptive name (no symbol found).
// Leaf: ?BuildMesh@C3DMeshLODInfo@@AAE_N_N0@Z
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANKENTRY_LODINFO_FRIEND
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CBANKFILE_ENTRYSYMBOL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESHSTATS_MESHGENERATORS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESH2_BUILDMESH
#define FABLE_RETAIL_C3DMESHLODINFO
#define FABLE_RETAIL_C3DPRIMITIVE2_UPLOADBUFFERS
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#include "engine/CBankFileAsyncData.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshLODInfo.h"

extern unsigned long g_NextMeshID;                         // 0x01397248
extern CMovableResourceMemoryPool* g_MeshDataMemoryPool;   // 0x013D2D6C
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70

bool C3DMeshLODInfo::BuildMesh(bool, bool flag)
{
    CMeshDataBank* bank = MeshInfo->Bank;
    unsigned long index = MeshInfo->Index;
    unsigned long type = bank->GetEntryDataType(index) & 0xf;
    bool animated = type == 5;
    bool repeated = type == 2;
    bool type4 = type == 4;
    CMemoryDataInputStream stream(FileData->GetData(), FileData->GetLength());
    unsigned long id = g_NextMeshID++;
    CMovableResourceMemoryPool* oldMeshPool = g_MeshDataMemoryPool;
    g_MeshDataMemoryPool = bank->MeshDataMemoryPool_Data;
    CMovableResourceMemoryPool* oldClothPool = g_ParticleMemoryPool;
    g_ParticleMemoryPool = bank->ClothMemoryPool_Data;
    if (LOD == 4)
        MeshInfo->Bank->MeshStatsResourceBank.GetResourceList().MakeFreeSpace(stream.GetLength());
    const CGraphicDataBank* textureBank = bank->TextureBank;
    CMovableResourceMemoryPool* pool = bank->MeshDataMemoryPool_Data;
    CCharString symbol = bank->GetEntrySymbol(index);
    Mesh.Reset(new C3DMesh2);
    if (!Mesh->LoadBinary(stream, pool, textureBank, flag)) {
        Mesh.Reset(new C3DMesh2);
        pool->Defragment();
        stream.OpenMemorySource(FileData->GetData(), FileData->GetLength());
        if (!Mesh->LoadBinary(stream, pool, textureBank, false)) {
            Mesh.ReleaseData();
            return false;
        }
    }
    Mesh->MeshID = id;
    if (textureBank)
        Mesh->CalculateTextureChangeFlags();
    Mesh->InitialiseMeshGenerators(*bank, symbol);
    bool ok = true;
    for (unsigned long i = 0; i < Mesh->PrimitiveCount && ok; ++i) {
        C3DPrimitive2* prim = Mesh->LockPrimitive(i);
        unsigned long uploadFlags = 0;
        if ((Mesh->Materials[prim->MaterialIndex].MapFlags & 2) && MeshInfo->Bank->DP3BumpEnable)
            uploadFlags = 2;
        if (animated)
            ok &= prim->UploadAnimatedBuffers(uploadFlags | 8);
        else if (repeated)
            ok &= prim->UploadRepeatedBuffers(uploadFlags, 0x20);
        else if (type4)
            ok &= prim->UploadRepeatedBuffers(uploadFlags, 0x20);
        else
            ok &= prim->UploadStaticBuffers(uploadFlags | 8);
        Mesh->UnlockPrimitive(i);
    }
    g_MeshDataMemoryPool = oldMeshPool;
    g_ParticleMemoryPool = oldClothPool;
    if (!ok) {
        Mesh.ReleaseData();
        return false;
    }
    if (LOD == 4)
        MeshInfo->Bank->MeshStatsResourceBank.AddResource(*this);
    else
        MeshInfo->Bank->MeshResourceBank.AddResource(*this);
    FileData.Reset(0);
    SetResourceSize(Sizeof());
    return true;
}