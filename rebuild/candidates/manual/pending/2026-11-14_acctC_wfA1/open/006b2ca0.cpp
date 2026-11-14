// CEnvironment::UpdateSunAndMoonPositions() -- unless the non-colour values are forced: sun and moon direction from the time of day turned through each body's three theme angles (Scalers 8..10 / 11..13), stored with the diffuse light direction (the sun and moon blended by Scalers[5], held at least DiffuseClampAngle below the horizon)
// Grade: DIFFER(908v910) -- structure found (successor): CMatrix3x4 SetYRotate / SetZRotate (FableWin COMDATs of this TU, new overlay opt-in) + TransformPoint twice per body; residue = x87 scheduling / which vector component goes through a frame temp (retail keeps sin t on the stack and spills cos t; ours the reverse), frame 0x88 v 0x84.
// Leaf: ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2CA0, 910 bytes (`ret`). Identity: Ego_r.pdb 0x006EE710 / FableWin.pdb 0x0244D960
//   ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ; reloc binding of the landed caller CEnvironment::Update 0x006B3FF0 (call 0x006B42A4).
//   Locals (Fable Anniversary PDB, weaker source): sun_vector, moon_vector, diffuse_vector (C3DVector).
#include "fable_compiler.h"
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TICKSTATES
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE
#define FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETYZROTATE
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnvironment.h"
#pragma intrinsic(sin, cos, sqrt)

void CEnvironment::UpdateSunAndMoonPositions()
{
    if (ForcedNonColourValues)
        return;
    const float* scalers = GameTickState.BlendedTheme[0].Scalers;
    CMatrix3x4 rotation;
    C3DVector sun_vector;
    sun_vector.X = (float)cos(GameTickState.Time * 6.2831853071795862);
    sun_vector.Y = (float)sin(GameTickState.Time * 6.2831853071795862);
    sun_vector.Z = 0.0f;
    rotation.SetYRotate((float)(-scalers[8] * 6.2831853071795862));
    rotation.TransformPoint(sun_vector);
    rotation.SetZRotate((float)(scalers[9] * 6.2831853071795862));
    rotation.TransformPoint(sun_vector);
    sun_vector.Z += scalers[10];
    sun_vector.AccurateNormalise();
    C3DVector moon_vector;
    moon_vector.X = (float)cos(GameTickState.Time * 6.2831853071795862);
    moon_vector.Y = (float)sin(GameTickState.Time * 6.2831853071795862);
    moon_vector.Z = 0.0f;
    rotation.SetYRotate((float)(-scalers[11] * 6.2831853071795862));
    rotation.TransformPoint(moon_vector);
    rotation.SetZRotate((float)(scalers[12] * 6.2831853071795862));
    rotation.TransformPoint(moon_vector);
    moon_vector.Z += scalers[13];
    moon_vector.AccurateNormalise();
    GameTickState.SunVector = sun_vector;
    GameTickState.MoonVector = moon_vector;
    C3DVector diffuse_vector = C3DVector(-GameTickState.SunVector.X, -GameTickState.SunVector.Y, -GameTickState.SunVector.Z) * (1.0f - scalers[5]) - GameTickState.MoonVector * scalers[5];
    diffuse_vector.AccurateNormalise();
    float sin_clamp = (float)sin(DiffuseClampAngle * 6.2831853071795862);
    if (-diffuse_vector.Z < sin_clamp) {
        diffuse_vector.Z = -sin_clamp;
        float scale = (float)sqrt((1.0f - sin_clamp * sin_clamp) / (diffuse_vector.Y * diffuse_vector.Y + diffuse_vector.X * diffuse_vector.X));
        diffuse_vector.X *= scale;
        diffuse_vector.Y *= scale;
        diffuse_vector.AccurateNormalise();
    }
    GameTickState.DiffuseVector = diffuse_vector;
}
