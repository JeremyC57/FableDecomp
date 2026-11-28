// CEngineSkyRenderer::RenderSprite(const CTexture*, const C3DVector&, const C3DVector&, const C3DVector&, CRGBColour)
// Leaf: ?RenderSprite@CEngineSkyRenderer@@AAEXPBVCTexture@@ABVC3DVector@@11VCRGBColour@@@Z
// Flags: /GS
// The inline body in CEngineSkyRenderer.h is shared with RenderMoon, which provides its pick-any COMDAT.
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_MOON_PASS30
#define FABLE_RETAIL_CENGINESKYRENDERER_MOON_SPRITE_BODY_PASS30
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#include <math.h>
#include "fable_string.h"
#include "engine/CEngineSkyRenderer.h"

// Fidelity: the private instantiation requests the existing inline member for standalone object builds.
namespace {
typedef void (CEngineSkyRenderer::*Owner)(const CTexture*, const C3DVector&, const C3DVector&, const C3DVector&, CRGBColour);
template<Owner Method>
#if defined(__GNUC__) || defined(__clang__)
__attribute__((used))
#endif
static Owner EmitOriginalMember() { return Method; }
#if defined(_MSC_VER) && _MSC_VER == 1310 && !defined(__clang__)
template Owner EmitOriginalMember<static_cast<Owner>(&CEngineSkyRenderer::RenderSprite)>();
#else
template Owner EmitOriginalMember<&CEngineSkyRenderer::RenderSprite>();
#endif
}
