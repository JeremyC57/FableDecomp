#ifdef FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#ifndef FABLE_ENGINE_REPEATEDSTATICMESHSUBSECTION_H
#define FABLE_ENGINE_REPEATEDSTATICMESHSUBSECTION_H
namespace NEnginePrimitiveManagerRepeatedStaticMeshes {
// Original PDB U type80; native structure-of-arrays offsets0/10/20/30/40/44/48/4C.
struct CSubsectionElement {
    float BoundingSphereCenterX[4],BoundingSphereCenterY[4],BoundingSphereCenterZ[4],BoundingSphereRadius[4];
    unsigned char ObjectCount[4],StartIndex[4],SubSectionOffset[4],Pad[4];
};
}
#endif
#endif
