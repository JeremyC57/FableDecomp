// CEnvironment::RenderUpdate(float frame_fraction) -- blend the previous and current game tick states into the interpolated draw params for this render frame
// Leaf: ?RenderUpdate@CEnvironment@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B42F0, 1543 bytes (`sub esp,2B4h` .. `ret 4`); no flags.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?RenderUpdate@CEnvironment@@QAEXM@Z, and the landed atexit stub 0x01229390 (`_$E2`)
//   whose only registrant is this body (0x006B43C5) for the function-local static
//   ?interpolated_theme@?4??RenderUpdate@CEnvironment@@QAEXM@Z@4VCBlendedEnvironmentTheme@NEnvironmentInternal@@A (0x013BA900, guard
//   bit 0 of 0x013BAAF4), which this TU emits under the same name.
// What it does: on the first call runs Update(0); clamps the frame fraction to [0, 1] in place; interpolates the previous and current
//   game tick states into InterpolatedEnvironmentParams (+0x1894): Time, the six theme channels (each blended by
//   CBlendedEnvironmentTheme::Blend into the static, then copied colour by colour, scaler by scaler and texture set by texture set
//   into ThemeChannels[channel], resized to 6 first), wind / theme-wind direction and speed, and the sun / moon / diffuse vectors
//   (accurately renormalised); then fetches this frame's radial-blur and displacement parameters from the spell-effect interface
//   into the params' ScreenEffectParams and hands its screen-filter parameters to the 3D engine as a colour filter.
// Callees: CEnvironment::Update 0x006B3FF0; CThemeChannel ctor 0x006B5FB0 / dtor 0x006B5B80 and vector<CThemeChannel>::resize(n, x)
//   0x006B6D90 (STLport resize(n) expands to resize(n, T()) with the value temporary at [esp+0D4h]); CBlendedEnvironmentTheme ctor
//   0x008864A0 + atexit 0x004012BC; CBlendedEnvironmentTheme::Blend 0x00887B70; vector<long> / vector<float>::operator= 0x00454886 /
//   0x0045EF2D (the implicit CIDrawEnvironmentTextureSet::operator=); C3DVector::AccurateNormalise 0x00A14440;
//   CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedRadialBlurParameters / DisplacementParameters /
//   ScreenFilterParameters 0x008857E0 / 0x00885900 / 0x008859F0; CIEngine::SetScreenColourFilter (slot 61, `call [eax+0F4h]`) on
//   CMainGameComponent::PMainGameComponent (0x013B86A0) -> PDisplayEngine (+0x28) -> Engine3D (+0x2C).
// Shape (measured, see NOTES.md):
//   - braced `if (!PassedFirstUpdate) { Update(0.0f); }` + the inline GFLimit clamp give the static VC7.1 scope ?4? (an if / else-if
//     clamp gives ?8?); GFLimit's body is retail's `fcomp 0.0 ; test ah,5 ; jp` / `fcomp 1.0 ; test ah,41h ; jne` with stores back
//     into the argument slot.
//   - GFLimit(frame_fraction, 0.0f, 1) is the FableWin.pdb instance ??$GFLimit@MMH@@YIXAAMABMABH@Z (GFLimit<float,int,float> 0, 1.0f
//     gives the same bytes; both are FableWin-only instances). Same instructions as (0.0f, 1.0f), but it changes which c2 symbol
//     record the scaler loop's induction variable gets: with the int bound the IV takes a reused, lower record (id 1733) than the
//     per-store `_M_start` reload temporaries (2376..), and the smaller id is emitted as the SIB index -> retail's `[edx+ecx+d]`
//     (0x006B4455..0x006B44A4, 9 stores; the disp-0 store is `[ecx+edx]` either way). With (0.0f, 1.0f) the IV is a fresh record
//     (2570) above the temporaries (2373..2568) -> `[ecx+edx+d]` (c2 SIB trace, tools/c2h/sib.py; the r3005 record-id rule).
//   - ThemeChannels[...] through begin() (FABLE_RETAIL_CARRAY): retail's `mov eax,[esi] ; add eax,[esp+2Ch] ; lea ebp,[eax+edi+1A8h]`
//     in the texture-set loop; the colour copy is CRGBFloatColour's implicit struct copy (dest address once per element), the scaler
//     copy re-reads `_M_start` per store (a float store may alias it), unrolled x10 by VC7.1.
//   - GetSpellEffectInterface() (FableWin inline accessor) rather than the member read: retail's `lea edx,[esp+38h]` ahead of the
//     `rep movsd` and the edx / ecx roles in the displacement copy (0x006B484E..0x006B4889).
//   - The screen-filter result is built in place (copy-initialised local at [esp+44h]) and copied member by member into the
//     CEngineColourFilterSettings local at [esp+64h] (whose constructor's default stores are dead and dropped).
// Headers (overlay hdr/, see hdr/README.md): engine/CEnvironment.h FABLE_RETAIL_CENVIRONMENT_TICKSTATES (typed tick states and params) +
//   _SPELLEFFECTINTERFACE; engine/CIDrawEnvironmentParams.h (pending c2030 copy) + FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS;
//   engine/CIDrawEnvironmentScreenEffectParams.h (new); engine/CEnvironmentSpellEffectInterface.h
//   FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_RENDERFRAME.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE
#define FABLE_RETAIL_CENVIRONMENT_TICKSTATES
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_RENDERFRAME
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterface.h"
#include "engine/CEngineColourFilterSettings.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/GFLimit.h"

void CEnvironment::RenderUpdate(float frame_fraction)
{
    if (!PassedFirstUpdate)
    {
        Update(0.0f);
    }

    GFLimit(frame_fraction, 0.0f, 1);

    float one_minus_frame_fraction = 1.0f - frame_fraction;

    InterpolatedEnvironmentParams.Time = PrevGameTickState.Time * one_minus_frame_fraction + GameTickState.Time * frame_fraction;

    InterpolatedEnvironmentParams.ThemeChannels.resize(6);
    static NEnvironmentInternal::CBlendedEnvironmentTheme interpolated_theme;
    for (long channel = 0; channel < 6; ++channel)
    {
        PrevGameTickState.BlendedTheme[channel].Blend(interpolated_theme, GameTickState.BlendedTheme[channel], frame_fraction);
        long i;
        for (i = 0; i < 19; ++i)
            InterpolatedEnvironmentParams.ThemeChannels[channel].Colours[i] = interpolated_theme.Colours[i];
        for (i = 0; i < 30; ++i)
            InterpolatedEnvironmentParams.ThemeChannels[channel].Scalers[i] = interpolated_theme.Scalers[i];
        for (i = 0; i < 3; ++i)
            InterpolatedEnvironmentParams.ThemeChannels[channel].TextureSets[i] = interpolated_theme.TextureSets[i];
    }

    InterpolatedEnvironmentParams.WindDirection = PrevGameTickState.WindDirection * one_minus_frame_fraction + GameTickState.WindDirection * frame_fraction;
    InterpolatedEnvironmentParams.WindSpeed = PrevGameTickState.WindSpeed * one_minus_frame_fraction + GameTickState.WindSpeed * frame_fraction;
    InterpolatedEnvironmentParams.ThemeWindDirection = PrevGameTickState.ThemeWindDirection * one_minus_frame_fraction + GameTickState.ThemeWindDirection * frame_fraction;
    InterpolatedEnvironmentParams.ThemeWindSpeed = PrevGameTickState.ThemeWindSpeed * one_minus_frame_fraction + GameTickState.ThemeWindSpeed * frame_fraction;
    InterpolatedEnvironmentParams.SunVector = PrevGameTickState.SunVector * one_minus_frame_fraction + GameTickState.SunVector * frame_fraction;
    InterpolatedEnvironmentParams.MoonVector = PrevGameTickState.MoonVector * one_minus_frame_fraction + GameTickState.MoonVector * frame_fraction;
    InterpolatedEnvironmentParams.DiffuseVector = PrevGameTickState.DiffuseVector * one_minus_frame_fraction + GameTickState.DiffuseVector * frame_fraction;
    InterpolatedEnvironmentParams.SunVector.AccurateNormalise();
    InterpolatedEnvironmentParams.MoonVector.AccurateNormalise();
    InterpolatedEnvironmentParams.DiffuseVector.AccurateNormalise();

    InterpolatedEnvironmentParams.ScreenEffectParams.RadialBlur = GetSpellEffectInterface().GetRenderFrameInterpolatedRadialBlurParameters(frame_fraction);
    InterpolatedEnvironmentParams.ScreenEffectParams.Displacement = GetSpellEffectInterface().GetRenderFrameInterpolatedDisplacementParameters(frame_fraction);
    CIDrawEnvironmentScreenFilterParameters screen_filter = GetSpellEffectInterface().GetRenderFrameInterpolatedScreenFilterParameters(frame_fraction);
    CEngineColourFilterSettings colour_filter;
    colour_filter.Alpha = screen_filter.Alpha;
    colour_filter.Saturation = screen_filter.Saturation;
    colour_filter.Contrast = screen_filter.Contrast;
    colour_filter.Brightness = screen_filter.Brightness;
    colour_filter.Tint = screen_filter.Tint;
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetScreenColourFilter(colour_filter);
}

