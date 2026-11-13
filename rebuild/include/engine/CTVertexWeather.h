#ifndef FABLE_ENGINE_CTVERTEXWEATHER_H
#define FABLE_ENGINE_CTVERTEXWEATHER_H
// New header (chunk 2005, row 0x00B51570 CEngineWeatherRenderer::BuildBuffers): Ego_r.pdb CTVertexWeatherBase (0x18 bytes: X +0, Y +4,
// Z +8, U1 +0xC, V1 +0x10, ConstantIndex +0x14 ushort, Pad +0x16 ushort) and CTVertexWeather = CGenericVertex<CTVertexWeatherBase>
// (ghidra_out/struct_layouts_egor.tsv `CGenericVertex<CTVertexWeatherBase>` 24 bytes, `CTVertexWeather` 24 bytes, no own members);
// retail's weather vertex buffers are created with stride 0x18 and BuildBuffers fills these offsets.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CGenericVertex.h"
class CTVertexWeatherBase {
public:
    float          X;                                    // +0x00
    float          Y;                                    // +0x04
    float          Z;                                    // +0x08
    float          U1;                                   // +0x0c
    float          V1;                                   // +0x10
    unsigned short ConstantIndex;                        // +0x14
    unsigned short Pad;                                  // +0x16
};
class CTVertexWeather : public CGenericVertex<CTVertexWeatherBase> {
};
FABLE_STATIC_ASSERT(sizeof(CTVertexWeather) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTVertexWeatherBase, ConstantIndex) == 0x14);
#endif
