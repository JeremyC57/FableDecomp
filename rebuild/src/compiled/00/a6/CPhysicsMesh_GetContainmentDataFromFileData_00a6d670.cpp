// 00a6d670 CPhysicsMesh::GetContainmentDataFromFileData
// Leaf: ?GetContainmentDataFromFileData@CPhysicsMesh@@AAEXABVC3DMeshFile@@@Z
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
#define FABLE_RETAIL_C3DMESHSTATS_FILE_LOADER_PASS28
#define FABLE_RETAIL_C3DDUMMYOBJECT_TRANSFORMCTOR
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
#define FABLE_RETAIL_CVECTORMAP_SORT
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
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);
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
void CPhysicsMesh::GetContainmentDataFromFileData(const C3DMeshFile& file_data)
{
    CCharString starts_with_text("NAV_VOLUME_");
    CCountedPointer<const C3DMeshFileRootChunk> root = file_data.GetRoot();
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileConvexVolumeChunk> volume_data = helpers->FindFirstVolumeW();
        while (volume_data.Get()) {
            C3DMeshVolume mesh_volume(*volume_data, MemoryPool);
            mesh_volume.GetConvexVolume()->CheckDebug();
            CCharString volume_name = mesh_volume.GetName();
            if (volume_name.StartsWith(starts_with_text)) {
                CCharString level_number_string = volume_name.GetSubString(starts_with_text.GetLength(),volume_name.GetLength());
                long level_number = GFCharStringToInt(level_number_string);
                FABLE_RETAIL_ASSERT(level_number > 0 && level_number <= 10);
                NavigationContainmentVolumes.Insert(level_number - 1,mesh_volume);
            } else {
                ContainmentVolumes.Insert(volume_name,mesh_volume);
            }
            volume_data = helpers->FindNextVolumeW(volume_data);
        }
        NavigationContainmentVolumes.Update();
        ContainmentVolumes.Update();
    }
}