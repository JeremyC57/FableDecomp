// CPhysicsMeshObject::GetNavigationLevelVolumes(CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >& volumes) const -- the mesh's navigation-level containment volumes moved into world space: every plane's normal rotated and renormalised, its point transformed, one new counted volume per level added to the map, the map sorted
// Grade: RELOCATION_MATCH(1144v1144)
// Leaf: ?GetNavigationLevelVolumes@CPhysicsMeshObject@@QBEXAAV?$CVectorMap@JV?$CCountedPointer@VCContainmentVolume@@@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@@@Z
// Retail: Fable.exe 0x00A12EE0, 1144 bytes (`ret 4`). Callees: CVectorMap / vector reserve 0x00A13650, C3DVector::AccurateNormalise
//   0x00A14440, CPlane::Initialise 0x00A42140, vector<CPlane> insert-overflow 0x00A11210, CContainmentVolume ctor 0x00A752D0 /
//   Initialise 0x00A755C0, vector<pair> insert-overflow 0x00A139B0, operator new / delete / free; std::sort helpers 0x008274B0 /
//   0x008270E0 / 0x00825B40 (Sort inline).
// Shape: `const C3DMeshVolume& mesh_volume = it->second;` read twice = retail's second induction pointer it+8 ([esp+14h]);
//   CMatrix3x4::TransformPlane (FableWin: first COMDAT after this function) expanded per plane; `const CPhysicsMesh* mesh = Mesh.Get();`
//   (FableWin Get + IsNull after the function) gives the latch reload order.
//   `const CPlane& source_plane = source_planes[i]; CPlane plane = source_plane;` (one more named symbol at function level) gives the
//   inlined TransformPoint X row retail's product order (0x00A1300D X*E11 before 0x00A1301B Y*E21; c2 record-id hash order, NOTES).
// Inline-budget: TransformPlane is FABLE_FORCEINLINE in the overlay header: plain inline is called out of line here (887v1144, grade 21).
#include "fable_compiler.h"
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CPHYSICSMESH_NAVVOLUMES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CCONTAINMENTVOLUME
#define FABLE_RETAIL_CPLANE_NORMAL_DISTANCE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPLANE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CPLANE_ACCESSORS
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_SORT_INLINE
#define FABLE_RETAIL_CVECTORMAP_SORT_INLINE_FORCE
#define FABLE_RETAIL_STLP_SORT_INLINE
#define FABLE_RETAIL_STLP_SORT_INLINE_FORCE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVVOLUMES
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CContainmentVolume.h"
#include "engine/CPlane.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"

void CPhysicsMeshObject::GetNavigationLevelVolumes(CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >& volumes) const
{
    const CPhysicsMesh* mesh = Mesh.Get();
    if (mesh)
    {
        const CVectorMap<long, C3DMeshVolume, CKeyPairCompareLess<long, C3DMeshVolume> >& levels = mesh->GetNavigationContainmentVolumes();
        for (const std::pair<long, C3DMeshVolume>* it = levels.begin(); it != levels.end(); ++it)
        {
            long level = it->first;
            const C3DMeshVolume& mesh_volume = it->second;
            long plane_count = mesh_volume.GetConvexVolume()->GetPlaneCount();
            const CPlane* source_planes = mesh_volume.GetConvexVolume()->PeekContainmentPlanes();
            if (plane_count > 0)
            {
                CArray<CPlane> planes;
                planes.reserve(plane_count);
                for (long i = 0; i < plane_count; ++i)
                {
                    const CPlane& source_plane = source_planes[i];
                    CPlane plane = source_plane;
                    PositionMatrix.TransformPlane(plane);
                    planes.push_back(plane);
                }
                CCountedPointer<CContainmentVolume> volume(new CContainmentVolume);
                volume->Initialise(planes.GetAsCArray(), plane_count, 0);
                volumes.Insert(level, volume);
            }
        }
    }
    volumes.Sort();
}
