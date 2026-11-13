#ifndef FABLE_ENGINE_CSKYDEF_H
#define FABLE_ENGINE_CSKYDEF_H
// New header (chunk 2048, row 0x00B64550 CEngineSkyRenderer::RenderSunFlare): the retail CSkyDef members RenderSunFlare reads. Ego_r.pdb
// layout (struct_layouts_egor.tsv) less 4 (the retail CDefClassBase is 0x25 packed, so CParentDefClassBase-derived members start at 0x3C, as
// CLensFlareElementDef's Radius): SunRadius +0x3C .. TwinkleMax +0x70, FlareElements (STLport vector, 12 bytes) +0x74. Retail evidence:
// RenderSunFlare reads FlareElements' start / finish +0x74 / +0x78 (stride 0x4C = sizeof CLensFlareElementDef), SunFlareRadius +0x4C,
// SunFlareTexture +0x50.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CSkyDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "stlport/fable_containers.h"
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CLensFlareElementDef.h"
class CSkyDef : public CParentDefClassBase {
public:
    long  SunRadius;                                                     // +0x3C
    long  SunTexture;                                                    // +0x40
    long  MoonRadius;                                                    // +0x44
    long  MoonTexture;                                                   // +0x48
    long  SunFlareRadius;                                                // +0x4C
    long  SunFlareTexture;                                               // +0x50
    long  StarTexture;                                                   // +0x54
    long  StarChartTextureSize;                                          // +0x58
    float StarSize;                                                      // +0x5C
    long  StarChartFilter;                                               // +0x60
    long  TwinkleInterval;                                               // +0x64
    float TwinkleSpeed;                                                  // +0x68
    float TwinkleMin;                                                    // +0x6C
    float TwinkleMax;                                                    // +0x70
    std::vector<CLensFlareElementDef> FlareElements;                     // +0x74
};
#endif
#endif
