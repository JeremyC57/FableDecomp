#ifndef FABLE_ENGINE_CENGINEPRIMITIVEWEATHERMASK_H
#define FABLE_ENGINE_CENGINEPRIMITIVEWEATHERMASK_H
// New header (chunk 734, row 0x004A1CB0 CWorld::PreparePrimitivesForEngine). Ego_r.pdb CEnginePrimitiveWeatherMask (sizeof 0x6C,
// struct_layouts_egor.tsv; no STL member, so the PDB offsets hold in retail): CEnginePrimitive base {Type +0, InterpolationSet +4,
// ThemeChannelDesc +8}, BSphere +0x0C, BBox +0x1C, InvTransform +0x34, Scale +0x64, Flags +0x68. The constructor
// ??0CEnginePrimitiveWeatherMask@@QAE@ABVC3DBoundingSphere@@ABVC3DBoundingBox@@ABVCMatrix3x4@@MKW4EEngineInterpolationSet@@@Z (Ego_r)
// is out of line in retail at 0x0049D000 (landed, `ret 18h`; primitive type 0x1F).
#include "engine/CEnginePrimitive.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CMatrix3x4.h"
class CEnginePrimitiveWeatherMask : public CEnginePrimitive {
public:
    CEnginePrimitiveWeatherMask(const C3DBoundingSphere& bsphere, const C3DBoundingBox& bbox, const CMatrix3x4& inv_transform,
                                float scale, unsigned long flags, EEngineInterpolationSet interpolation_set);   // 0x0049D000
    C3DBoundingSphere BSphere;                               // +0x0C
    C3DBoundingBox    BBox;                                  // +0x1C
    CMatrix3x4        InvTransform;                          // +0x34
    float             Scale;                                 // +0x64
    unsigned long     Flags;                                 // +0x68
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveWeatherMask) == 0x6C);
#endif
