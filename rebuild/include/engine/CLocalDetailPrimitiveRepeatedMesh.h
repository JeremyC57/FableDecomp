#ifdef FABLE_RETAIL_REPEATED_MESH_LOAD_PASS30
#ifndef FABLE_ENGINE_CLOCALDETAILPRIMITIVEREPEATEDMESH_H
#define FABLE_ENGINE_CLOCALDETAILPRIMITIVEREPEATEDMESH_H
#include "engine/CLocalDetailPrimitiveMeshZSpriteBatch.h"
#include "engine/C4DVector.h"
#include "engine/RepeatedStaticMeshSubsection.h"

// Ego_r TPI 0x5b54f; matching fields and virtual overrides in FableWin.
class CLocalDetailPrimitiveRepeatedMesh : public CLocalDetailPrimitiveBase {
public:
    CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType*, CLocalDetailCacheMap*);
    virtual ~CLocalDetailPrimitiveRepeatedMesh();
    virtual void AddObjectsToPrimitiveRenderer();
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const;
    virtual bool Load(CDataInputStream& stream);
    void SetupWindAnimation();
    virtual void OnPostRelocation();
    virtual void RelocateData(long, void*, void*, long);
private:
    float MaxObjectScale;
    long ObjectCount;
    long SubSectionsCount;
    NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement* SubSections;
    C4DVector* ObjectMatricies;
    C4DVector* ObjectOffsets;
    unsigned char* WindDelayArray;
    float* LandscapeNormalArray;
    CEnginePrimitiveHandle PrimitiveHandle;
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveRepeatedMesh) == 0x60);
FABLE_STATIC_ASSERT(sizeof(NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement) == 0x50);
#endif
#endif
