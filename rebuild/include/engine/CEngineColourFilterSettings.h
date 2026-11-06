#ifndef FABLE_ENGINE_CENGINECOLOURFILTERSETTINGS_H
#define FABLE_ENGINE_CENGINECOLOURFILTERSETTINGS_H
// Retail layout (new, chunk 792): CEngineColourFilterSettings, Ego_r.pdb class (V-keyed, pdb_class_keys.tsv), 0x20 bytes:
// Alpha +0x00, Saturation +0x04, Contrast +0x08, Brightness +0x0C, CRGBFloatColour Tint +0x10 (struct_layouts_egor.tsv). No STLport
// member, so Ego_r = retail; CEngineScreenEffectColourFilter::Settings (+0x10) is read at these offsets by ApplyColourFilter 0x00B5A3F0
// (Alpha +0x10, Saturation +0x14, Contrast +0x18, Brightness +0x1C, Tint +0x20..+0x2C). Constructor ??0CEngineColourFilterSettings@@QAE@XZ
// (Ego_r.pdb name set; inline in retail: the landed CEngineScreenEffectColourFilter constructor 0x00B5A1D0 stores 0, 1, 0, 0, 1, 1, 1, 1).
#include "rebuild_abi.h"
#include "engine/CRGBFloatColour.h"
class CEngineColourFilterSettings {
public:
    CEngineColourFilterSettings() : Alpha(0.0f), Saturation(1.0f), Contrast(0.0f), Brightness(0.0f), Tint(1.0f, 1.0f, 1.0f, 1.0f) { }
    float           Alpha;                      // +0x00
    float           Saturation;                 // +0x04
    float           Contrast;                   // +0x08
    float           Brightness;                 // +0x0c
    CRGBFloatColour Tint;                       // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CEngineColourFilterSettings) == 0x20);
#endif // FABLE_ENGINE_CENGINECOLOURFILTERSETTINGS_H
