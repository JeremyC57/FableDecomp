// 00a6d0e0 CPhysicsMesh::GetHelperDataFromFileData
// Leaf: ?GetHelperDataFromFileData@CPhysicsMesh@@AAEXABVC3DMeshFile@@@Z
// Flags: /Gr
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK_FINDERS
#define FABLE_RETAIL_C3DMESHFILEROOTCHUNK_HELPERS
#define FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK
#define FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILECONVEXVOLUMECHUNK_GETTERS
#define FABLE_RETAIL_C3DHELPERPOINT_LAYOUT_PASS27

#define FABLE_RETAIL_PHYSICS_FILE_COPY_CTORS_PASS29
#define FABLE_RETAIL_C3DMESHVOLUME_CHUNKCTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_SLONGMAP
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCONTAINMENTVOLUME
#define FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
#define FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CVECTORMAP_SORT_INLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CCHUNKEDFILE_GETCHUNKEDFILEROOT
#include "fable_assert.h"
#include "engine/CPhysicsMesh.h"
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileRootChunk.h"
#include "engine/C3DMeshFileHelpersChunk.h"
#include "engine/C3DMeshFileHelperPointChunk.h"
#include "engine/C3DMeshFileDummyObjectChunk.h"
#include "engine/C3DMeshFileConvexVolumeChunk.h"
#include "engine/CContainmentVolume.h"
// Inline-budget: These two mesh readers both call the original 101-byte GetRoot.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
inline CCountedPointer<const C3DMeshFileRootChunk> C3DMeshFile::GetRoot() const
{
    CCountedPointer<const C3DMeshFileRootChunk> root;
    root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(GetChunkedFileRoot()));
    return root;
}

#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CPhysicsMesh::GetHelperDataFromFileData(const C3DMeshFile& file_data)
{
    CCountedPointer<const C3DMeshFileRootChunk> root = file_data.GetRoot();
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileHelperPointChunk> point_data = helpers->FindFirstPoint();
        while (point_data.Get()) {
            C3DHelperPoint helper_point(point_data->GetPosition(), point_data->GetBoneIndex());
            HelperPoints.Insert(point_data->GetName(), helper_point);
            point_data = helpers->FindNextPoint(point_data);
        }
        CCountedPointer<const C3DMeshFileDummyObjectChunk> dummy_data = helpers->FindFirstDummy();
        while (dummy_data.Get()) {
            C3DDummyObject dummy_object(dummy_data->GetTransform(), dummy_data->GetBoneIndex());
            DummyObjects.Insert(dummy_data->GetName(), dummy_object);
            dummy_data = helpers->FindNextDummy(dummy_data);
        }
    }
    HelperPoints.Sort();
    DummyObjects.Sort();
}