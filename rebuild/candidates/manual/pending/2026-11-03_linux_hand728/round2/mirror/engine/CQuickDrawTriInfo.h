#ifndef FABLE_ENGINE_CQUICKDRAWTRIINFO_H
#define FABLE_ENGINE_CQUICKDRAWTRIINFO_H
#ifndef FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE
#endif
#include "rebuild_abi.h"
#include "engine/C2DBoxF.h"
#include "engine/CTexture.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class CFontBank;
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
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
