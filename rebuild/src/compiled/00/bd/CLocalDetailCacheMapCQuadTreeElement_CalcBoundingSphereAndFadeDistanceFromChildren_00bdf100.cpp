// CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren() -- gather the non-empty bounding
// spheres of the four children and of the element's own object-cache-group collections into the function-local static
// `sphere_list` (tracking the largest fade distance and OR-ing the primitive-type masks), then fit the element's bounding
// sphere around them (radius 0 when there are none).
// Retail 0x00BDF100, 610 bytes, __fastcall, plain `ret` (Ego_r.pdb / FableWin.pdb
// ?CalcBoundingSphereAndFadeDistanceFromChildren@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ).
// Callees: atexit 0x004012BC (dynamic atexit destructor for 'sphere_list' 0x0122ADB0), vector<C3DBoundingSphere>::resize(n)
// 0x00BE0140 (out of line), vector<C3DBoundingSphere>::_M_insert_overflow 0x00BDFE70 (called at the child site; VC7.1
// auto-inlined the same STLport body at the collection site), malloc 0x00BFEA0E, free 0x00BFEA14,
// C3DBoundingSphere::BuildFromSubSpheres 0x00ADDBE0. Static storage 0x0143B978, guard 0x0143B984 bit 0.
// Shape: shared headers only (CLocalDetailCacheMap.h nested CQuadTreeElement / CObjectCacheGroupCollection, C3DBoundingSphere.h
// with the PDB's memberwise copy ctor, STLport vector from fable_containers.h with resize(n) declaration-only).
// Leaf: ?CalcBoundingSphereAndFadeDistanceFromChildren@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ
#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_QUADTREE
#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_CALCBOUNDINGSPHERE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_COPY
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "engine/CLocalDetailCacheMap.h"

void CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren()
{
    static std::vector<C3DBoundingSphere> sphere_list;
    sphere_list.resize(0);
    MaxFadeDistance = 0;
    PrimitiveTypeMask = 0;
    for (int i = 0; i < 4; ++i) {
        if (Children[i] && Children[i]->BoundingSphere.Radius > 0.0f) {
            sphere_list.push_back(Children[i]->BoundingSphere);
            if (MaxFadeDistance < Children[i]->MaxFadeDistance)
                MaxFadeDistance = Children[i]->MaxFadeDistance;
            PrimitiveTypeMask |= Children[i]->PrimitiveTypeMask;
        }
    }
    for (CObjectCacheGroupCollection* collection = ObjectCacheGroupCollections; collection;
         collection = collection->Next) {
        if (collection->BoundingSphere.Radius > 0.0f) {
            if (MaxFadeDistance < collection->MaxFadeDistance)
                MaxFadeDistance = collection->MaxFadeDistance;
            PrimitiveTypeMask |= collection->PrimitiveTypeMask;
            sphere_list.push_back(collection->BoundingSphere);
        }
    }
    if (sphere_list.size() > 0)
        BoundingSphere.BuildFromSubSpheres(&sphere_list[0], sphere_list.size());
    else
        BoundingSphere.Radius = 0;
}