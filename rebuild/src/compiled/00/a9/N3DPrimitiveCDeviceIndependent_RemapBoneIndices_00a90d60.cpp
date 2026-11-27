// 00a90d60 N3DPrimitive::CDeviceIndependent::RemapBoneIndices
// Flags: (none)
// Leaf: ?RemapBoneIndices@CDeviceIndependent@N3DPrimitive@@AAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
#define FABLE_RETAIL_GROUP_REMAP_PASS29
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_FILL_FASTCALL
#define FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22

#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C

#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27

#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_CARRAY_AT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK
#define FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DGROUP2
#include "stlport/fable_map.h"
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
#include "engine/C3DMeshFileXTriangleStripChunk.h"
#include "engine/C3DGroup2.h"
#include "engine/EVertexType.h"
#include "engine/C3DPrimitive2.h"
namespace N3DPrimitive {
void CDeviceIndependent::RemapBoneIndices(std::map<unsigned long,unsigned long>& index_map)
{
    if (Groups.empty()) return;
    CArray<CCountedPointer<C3DGroup2> > new_groups(index_map.size());
    unsigned long max_new_index=0;
    for (std::map<unsigned long,unsigned long>::iterator itr=index_map.begin();itr!=index_map.end();++itr) {
        unsigned long old_index=itr->first;
        unsigned long new_index=itr->second;
        if (old_index < Groups.size()) {
            new_groups.At(new_index)=Groups.At(old_index);
            new_groups.At(new_index)->SetBoneIndex(new_index);
            if (max_new_index<=new_index) max_new_index=new_index;
        }
    }
    new_groups.resize(max_new_index+1);
    Groups=new_groups;
}
}