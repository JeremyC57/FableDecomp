#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
// New header (chunk 724b): CEnginePrimitivePrepareInfo, Ego_r sizeof 0x74 (struct_layouts_egor.tsv): Interpolate
// (CInterpolationInfo, 0x34) +0, Engine +0x34 (`CIEngine&`; the layout dump renders the reference as `CIEngine *`), Edit2DEngine
// +0x38, Camera (CCamera, 0x38) +0x3C. Retail: NCameraModes::DrawSquare 0x00875FB0 reads Engine at +0x34 (`mov ecx,[edi+34h]`) and
// calls AddPrimitive through it. Class key V (pdb_class_keys). Interpolate and Camera stay opaque until a row reads them.
// Needs FABLE_RETAIL_CIENGINE for the AddPrimitive slot.
#include "rebuild_abi.h"
#include "engine/CIEngine.h"
class CEnginePrimitivePrepareInfo {
public:
    unsigned char Interpolate[0x34];   // const CInterpolationInfo +0x00
    CIEngine& Engine;                  // +0x34
    bool Edit2DEngine;                 // +0x38
    unsigned char _pad_0x39[3];
    unsigned char Camera[0x38];        // CCamera +0x3C
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitivePrepareInfo) == 0x74);
#endif
