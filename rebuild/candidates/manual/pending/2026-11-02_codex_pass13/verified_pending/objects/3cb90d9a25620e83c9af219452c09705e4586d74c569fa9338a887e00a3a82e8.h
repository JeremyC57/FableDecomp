#ifndef FABLE_ENGINE_CQUICKDRAWTRIINFO_RENDER2D_H
#define FABLE_ENGINE_CQUICKDRAWTRIINFO_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/C2DBoxF.h"
#include "engine/CTexture.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class CFontBank;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CQuickDrawTriInfo { public:
 unsigned char Line:1;unsigned char IsText:1;
 C2DBoxF Window;CWideString String;CTexture UploadedTexture;
 union {unsigned long BlendMode;float StringX;};
 union {float ZAverage;float StringY;};
 float StringZ,StringScale;unsigned long StringFlags;
 CRGBColour Colour;const CFontBank* PFont;
};
FABLE_STATIC_ASSERT(sizeof(CQuickDrawTriInfo)==60);
#endif
