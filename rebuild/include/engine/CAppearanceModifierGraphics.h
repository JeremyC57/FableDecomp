#ifndef FABLE_ENGINE_CAPPEARANCEMODIFIERGRAPHICS_H
#define FABLE_ENGINE_CAPPEARANCEMODIFIERGRAPHICS_H
#ifdef FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#include "engine/CArray.h"
#include "engine/EEngineGraphicType.h"
#include "engine/EHeroMorphType.h"
class CAppearanceModifierGraphics {
public:
    struct CEntry {
        EEngineGraphicType Type;
        EHeroMorphType MorphType;
        long BankIndex;
        float RenderSizeX, StartMorphStrength, EndMorphStrength;
    };
    long GetNoGraphics() const { return Graphics.size(); }
    const CEntry& GetGraphic(long n) const { return Graphics[n]; }
protected:
    CArray<CEntry> Graphics;
};
#endif
#endif
