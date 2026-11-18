#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMORPHEDSTATICMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMORPHEDSTATICMESH_H
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMORPHEDSTATICMESH_EYEDRAW_PASS24
#include "engine/CEnginePrimitiveStaticMesh.h"
#include "engine/CEngineTextureMorph.h"
#include "stlport/fable_map.h"

// Original Ego_r and FableWin shape with retail STLport vector and set sizes.
class CEnginePrimitiveMorphedStaticMesh : public CEnginePrimitiveStaticMesh {
public:
    std::vector<CEngineTextureMorph> TextureMorphs;                  // +0x4C
    std::set<CCharString> HiddenMaterialNames;                      // +0x58
    CEnginePrimitiveMorphedStaticMesh();                            // 0x007554C0
    ~CEnginePrimitiveMorphedStaticMesh();                           // 0x00755520
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveMorphedStaticMesh) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveMorphedStaticMesh, TextureMorphs) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveMorphedStaticMesh, HiddenMaterialNames) == 0x58);
#endif
#endif
