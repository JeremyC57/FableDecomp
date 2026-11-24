// Initialize mesh and animation entries from their serialized metadata.
// Leaf: ?InitialiseEntry@CMeshDataBank@@MAEXKKABV?$CArray@D@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_INITIALISEENTRY_PASS27
#define FABLE_RETAIL_CARRAY_PUSHBACK_VALUE_DECL_PASS27
#define FABLE_RETAIL_CARRAY_AT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETMINMAX
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#include "fable_string.h"
#include "fable_assert.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMemoryDataInputStream.h"

void CMeshDataBank::InitialiseEntry(unsigned long datatype, unsigned long index, const CArray<char>& info_buffer)
{
    CMemoryDataInputStream info_stream(info_buffer.GetAsCArray(), info_buffer.size());
    CMeshDataBankEntry* mesh_entry = MeshesAndAnimations.At(index);
    FABLE_RETAIL_ASSERT(mesh_entry);
    if (IsRenderMesh(datatype)) {
        C3DMeshInfo* meshinfo = static_cast<C3DMeshInfo*>(mesh_entry);
        unsigned long physics_mesh_index = info_stream.ReadULONG();
        if ((IsStaticMesh(datatype) || IsAnimatedMesh(datatype)) && physics_mesh_index)
            meshinfo->SetPhysicsMeshIndex(physics_mesh_index);
        C3DVector center;
        center = info_stream.Read3DVector();
        float radius = info_stream.ReadFloat();
        C3DBoundingSphere bsphere(center, radius);
        C3DVector bbox_min = info_stream.Read3DVector();
        C3DVector bbox_max = info_stream.Read3DVector();
        C3DBoundingBox bbox;
        bbox.SetMinMax(bbox_min, bbox_max);
        float safe_radius = radius;
        if (IsAnimatedMesh(datatype))
            safe_radius += bsphere.Center.GetMagnitude();
        meshinfo->SetBoundingVolumes(bsphere, bbox);
        meshinfo->SafeBoundingRadius = safe_radius;
        unsigned long no_lods = info_stream.ReadULONG();
        FABLE_RETAIL_ASSERT(no_lods <= 4);
        std::fill_n(meshinfo->LODDataSizes, 4L, 0);
        for (unsigned long c_lod = 0; c_lod < no_lods; ++c_lod)
            meshinfo->LODDataSizes[c_lod] = info_stream.ReadULONG();
        std::fill_n(meshinfo->LODErrors, 4L, 0.0f);
        for (unsigned long c_lod = 0; c_lod < no_lods; ++c_lod)
            meshinfo->LODErrors[c_lod] = info_stream.ReadFloat();
    } else if (IsAnimation(datatype)) {
        C3DAnimationInfo* animinfo = static_cast<C3DAnimationInfo*>(mesh_entry);
        animinfo->Duration = info_stream.ReadFloat();
        animinfo->NonLoopingDuration = info_stream.ReadFloat();
        animinfo->MovementVector = info_stream.Read3DVector();
        animinfo->Rotation = info_stream.ReadFloat();
        if ((datatype & 0xf) == 8) {
            CCharString anim_name = info_stream.ReadNullTerminatedString();
            CCharString phoneme_name = info_stream.ReadNullTerminatedString();
            CLipSyncEntry lipsync(phoneme_name, index);
            LipSyncAnimations[anim_name].push_back(lipsync);
        }
    }
}