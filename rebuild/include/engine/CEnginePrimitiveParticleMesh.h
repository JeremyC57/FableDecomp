#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEMESH_H
// New header (chunk F1, row 0x00BC2480 CEngineInternalPrimitiveParticleMesh::Update): Ego_r.pdb CEnginePrimitiveParticleMesh, sizeof 0x40:
// the CEnginePrimitive base (0xC), WorldPos +0x0C, Size +0x18, RightHandedSet +0x24, Colour +0x3C (struct_layouts_egor.tsv; retail reads
// the same offsets at 0x00BC248E..0x00BC259A). Needs FABLE_RETAIL_CRIGHTHANDEDSET for the set's accessors.
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CRGBColour.h"
class CEnginePrimitiveParticleMesh : public CEnginePrimitive {
public:
    C3DVector       WorldPos;           // +0x0c
    C3DVector       Size;               // +0x18
    CRightHandedSet RightHandedSet;     // +0x24
    CRGBColour      Colour;             // +0x3c
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleMesh) == 0x40);
#endif
