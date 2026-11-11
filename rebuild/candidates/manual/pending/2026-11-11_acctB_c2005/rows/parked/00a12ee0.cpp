// CPhysicsMeshObject::GetNavigationLevelVolumes(CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >& volumes) const -- the mesh's navigation-level containment volumes moved into world space: every plane's normal rotated and renormalised, its point transformed, one new counted volume per level added to the map, the map sorted
// Grade: DIFFER(1118v1144) -- retail keeps a second outer induction pointer it+8 in [esp+14h] (frame 0x64 vs 0x60, 0x00A12F08), x87 load order of the plane point (0x00A12F73..0x00A12F9B) and the AccurateNormalise/TransformPoint interleave (0x00A12FFF..0x00A13037)
// Leaf: ?GetNavigationLevelVolumes@CPhysicsMeshObject@@QBEXAAV?$CVectorMap@JV?$CCountedPointer@VCContainmentVolume@@@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPHYSICSMESH_NAVVOLUMES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CCONTAINMENTVOLUME
#define FABLE_RETAIL_CPLANE_NORMAL_DISTANCE
#define FABLE_RETAIL_CMATRIX3X4
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
    if (Mesh.operator->())
    {
        const CVectorMap<long, C3DMeshVolume, CKeyPairCompareLess<long, C3DMeshVolume> >& levels = Mesh->GetNavigationContainmentVolumes();
        for (const std::pair<long, C3DMeshVolume>* it = levels.begin(); it != levels.end(); ++it)
        {
            long level = it->first;
            const CContainmentVolume* source = it->second.GetConvexVolume();
            long plane_count = source->GetPlaneCount();
            const CPlane* source_planes = source->PeekContainmentPlanes();
            if (plane_count > 0)
            {
                CArray<CPlane> planes;
                planes.reserve(plane_count);
                for (long i = 0; i < plane_count; ++i)
                {
                    CPlane plane = source_planes[i];
                    C3DVector point;
                    point.X = plane.Distance * plane.Normal.X;
                    point.Y = plane.Distance * plane.Normal.Y;
                    point.Z = plane.Normal.Z * plane.Distance;
                    C3DVector normal = plane.Normal;
                    PositionMatrix.TransformVector(normal);
                    normal = normal.AccurateNormalise();
                    PositionMatrix.TransformPoint(point);
                    plane.Initialise(normal, normal.Dot(point));
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
