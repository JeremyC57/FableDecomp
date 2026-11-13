// CEnvironment::Update(float time) -- the per-frame environment update: remember the previous tick state, run the spell-effect
// interpolator, stamp the tick time, then for each of the six priority channels process its theme transitions and blend its
// theme into the tick state (colours unless ForcedColours, scalars and texture sets unless ForcedNonColourValues); keep the
// rain-splash particle alive while it rains (create it at the render camera and attach it to the camera) or kill it once the
// rain stops; then the sun/moon, wind and lightning updates; on the very first update the previous tick state is the current one.
// Leaf: ?Update@CEnvironment@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B3FF0, 758 bytes; thiscall, `ret 4`. No /GX (no SEH frame despite the theme local's dtor).
// Identity: Ego_r.pdb (0x006EF9E0) / FableWin.pdb (0x0244E500) ?Update@CEnvironment@@QAEXM@Z; callers CWorld::UpdateFrame 0x004A5A40
//   and CEnvironment::RenderUpdate 0x006B42F0.
// Callees: CEnvironment::CEnvironmentGameTickState::operator= 0x006B63C0, CEnvironmentSpellEffectInterpolator::FrameUpdate 0x006B8640,
//   NEnvironmentInternal::CBlendedEnvironmentTheme ctor 0x008864A0 / dtor 0x006B5D80, CEnvironmentThemePriorityStack::ProcessTransitions
//   0x00887B00 / CalculateTheme 0x008889C0, vector<long>::operator= 0x00454886, vector<float>::operator= 0x0045EF2D (the implicit
//   CIDrawEnvironmentTextureSet assignment), CBaseIntelligentPointer ctor 0x00A01B10 / dtor 0x00A01C10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90, CAIGameCameraBase::GetRenderCamera (virtual slot 1), CTCDParticleEmitter::Create 0x006E0880 /
//   AttachToCamera 0x006E0DB0, CThing::Kill 0x004C9B80, CEnvironment::UpdateSunAndMoonPositions 0x006B2CA0 / UpdateWind 0x006B3030 /
//   LightingFlash 0x006B3B80.
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0, DELTA_NOTIONAL_ZERO 0x0129BA3C, CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: the rain threshold is the shared extern const DELTA_NOTIONAL_ZERO (not a literal: with a literal VC7.1 threads the
//   `rain = 0` path straight into the kill branch); GetRainStrength (0x006B2BD0, same TU) is expanded; the CCamera by-value return
//   temporary is the top 0x38 bytes of the 0x244 frame.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CENVIRONMENT_THEMESTACKS
#define FABLE_RETAIL_CENVIRONMENT_UPDATE
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FRAMEUPDATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_ENVIRONMENTDEF
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CThing.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
#include "engine/ThingAlive.h"

extern const float DELTA_NOTIONAL_ZERO;    // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

// Same-TU body of ?GetRainStrength@CEnvironment@@QBEMXZ (0x006B2BD0, `fld [ecx+1C4h]`), spelled on the PDB member chain: +0x1C4 =
// GameTickState (+0x54) .BlendedTheme[0] .Scalers (+0x134) [15]. The one spelling for every row of this TU: LightingFlash
// (0x006B3B80) and the 0x006B2BD0 row itself also grade on it (hand-back lf/).
float CEnvironment::GetRainStrength() const
{
    return GameTickState.BlendedTheme[0].Scalers[15];
}

void CEnvironment::Update(float time)
{
    PrevGameTickState = GameTickState;
    SpellEffectInterpolator_ptr->FrameUpdate();
    GameTickState.Time = time;
    NEnvironmentInternal::CBlendedEnvironmentTheme theme;
    for (long i = 0; i < 6; ++i) {
        PriorityStackChannels[i].ProcessTransitions();
        PriorityStackChannels[i].CalculateTheme(theme, time, ThemeSet);
        if (!ForcedColours)
            memcpy(GameTickState.BlendedTheme[i].Colours, theme.Colours, sizeof(theme.Colours));
        if (!ForcedNonColourValues) {
            for (long s = 0; s < 30; ++s)
                GameTickState.BlendedTheme[i].Scalers[s] = theme.Scalers[s];
            for (long t = 0; t < 3; ++t)
                GameTickState.BlendedTheme[i].TextureSets[t] = theme.TextureSets[t];
        }
    }
    float rain = GetRainStrength();
    if (!RainEnabled)
        rain = 0.0f;
    if (rain > DELTA_NOTIONAL_ZERO && !IS_THING_ALIVE(PRainSplashParticle)) {
        C3DVector position = CMainGameComponent::Get()->GetWorld().GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos();
        PRainSplashParticle = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(DefinitionManager->PeekEnvironmentDef().RainSplashParticle, position, false));
        if (IS_THING_ALIVE(PRainSplashParticle)) {
            CTCDParticleEmitter* emitter = 0;
            PRainSplashParticle->GetTCDriver(&emitter);
            emitter->AttachToCamera(8, 0.0f);
        }
    } else if (rain <= DELTA_NOTIONAL_ZERO && IS_THING_ALIVE(PRainSplashParticle)) {
        PRainSplashParticle->Kill(false);
    }
    UpdateSunAndMoonPositions();
    UpdateWind();
    LightingFlash();
    if (!PassedFirstUpdate) {
        PrevGameTickState = GameTickState;
        PassedFirstUpdate = true;
    }
}