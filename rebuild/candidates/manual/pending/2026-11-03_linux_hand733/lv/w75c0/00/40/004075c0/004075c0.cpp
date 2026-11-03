// CUserProfileManager::RenderUIBackground(float alpha) -- draw the captured back buffer behind the front-end UI through the engine's colour filter: saturation, contrast, a tint built from the misc-things def's filter tint plus the scaled global diffuse and ambient colours (lifted to the def's minimum), and brightness, each faded in from the unfiltered value by alpha
// Leaf: ?RenderUIBackground@CUserProfileManager@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004075C0, 1194 bytes, thiscall, `ret 4`.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (static __fastcall) and its virtual GetMiscThingsDef (vtable +0xA8, a
//   CDefPointer<CUIMiscThingsDef> by value, released after each use), the CIEngine virtuals PeekGlobalDiffuseColour (+0x14C, slot
//   83) / PeekGlobalAmbientColour (+0x150, slot 84) / RenderColorFilteredQuad (+0x148, slot 82) through CMainGameComponent::Get()
//   ->GetDisplayEngine().Get3DEngine() (PDisplayEngine +0x28, Engine3D +0x2C). Data: CUIMiscThingsDef BackBufferFilterSaturation
//   +0x238 .. MinimumFilterColor +0x25C; PBackBufferTexture +0x138.
// Statics: the guarded C4DVector (1, 1, 1, 1) at 0x013B7D38 (guard bit 0x013B7D48) and three zero floats 0x013B7D34 / 0x013B7D30 /
//   0x013B7D2C, read only here (no writer in the image): function-local statics, the unfiltered values every filter term fades
//   in from. Their names are in neither PDB name list (descriptive spellings: noFilterColour / noFilterSaturation /
//   noFilterContrast / noFilterBrightness).
// Shape: FableWin.pdb lists the inline helpers this function instantiates (0x01886220..0x018863E0): CRGBFloatColour::operator*=(float),
//   C4DVector(float x4), C4DVector::operator+ / operator- / operator*(float). Each filter term is `(def value - unfiltered) * alpha
//   + unfiltered`; the tint channel is `TintX * tintScale + diffuse.X + ambient.X` (diffuse added first: with ambient first VC7.1
//   swaps the two colours' frame slots), the alpha channel of the tint is the constant 1.0f.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "fable_game.h"
#include "fable_user_profile.h"
#include "engine/CDefPointer.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"

void CUserProfileManager::RenderUIBackground(float alpha)
{
    static C4DVector noFilterColour(1.0f, 1.0f, 1.0f, 1.0f);
    static float noFilterSaturation;
    static float noFilterContrast;
    static float noFilterBrightness;
    float saturation = (NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterSaturation - noFilterSaturation) * alpha + noFilterSaturation;
    float contrast = (NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterContrast - noFilterContrast) * alpha + noFilterContrast;
    CRGBFloatColour diffuse = CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().PeekGlobalDiffuseColour();
    CRGBFloatColour ambient = CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().PeekGlobalAmbientColour();
    diffuse *= NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferDiffuseScale;
    ambient *= NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferAmbientScale;
    float tintScale = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterTintScale;
    C4DVector colour(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterTintR * tintScale + diffuse.R + ambient.R,
                     NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterTintG * tintScale + diffuse.G + ambient.G,
                     NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterTintB * tintScale + diffuse.B + ambient.B,
                     1.0f);
    float minimum = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->MinimumFilterColor;
    if (colour.X < minimum || colour.Y < minimum || colour.Z < minimum) {
        colour.X += minimum;
        colour.Y += minimum;
        colour.Z += minimum;
    }
    C4DVector filter = (colour - noFilterColour) * alpha + noFilterColour;
    float brightness = (NUISystem::CManager::GetInstance()->GetMiscThingsDef()->BackBufferFilterBrightness - noFilterBrightness) * alpha + noFilterBrightness;
    if (PBackBufferTexture)
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().RenderColorFilteredQuad(saturation, contrast, filter, brightness, 1.0f, PBackBufferTexture);
}