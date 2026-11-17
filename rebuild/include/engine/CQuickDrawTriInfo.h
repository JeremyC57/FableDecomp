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
#ifdef FABLE_RETAIL_CQUICKDRAWTRIINFO_COPY
 // Retail opt-in (agentNF41, row 0x009DB700 CRenderManager2D::AddText): the user copy constructor Ego_r.pdb ??0CQuickDrawTriInfo@@QAE@ABV0@@Z
 // (0x00B321E0; retail 0x009DE890, landed) and the user operator= (retail 0x009DE6F0, landed; same TU), so the empty default constructor
 // is spelled out (AddText default-constructs a record: CWideString ctor 0x0099AED0 called, CTexture's inline). Declarations only.
 CQuickDrawTriInfo(){}
 CQuickDrawTriInfo(const CQuickDrawTriInfo&);               // 0x009DE890
 CQuickDrawTriInfo& operator=(const CQuickDrawTriInfo&);    // 0x009DE6F0
#endif
};
FABLE_STATIC_ASSERT_AT(21, sizeof(CQuickDrawTriInfo)==60);
#endif
