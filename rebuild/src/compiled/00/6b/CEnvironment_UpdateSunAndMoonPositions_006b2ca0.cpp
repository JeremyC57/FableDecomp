// CEnvironment::UpdateSunAndMoonPositions() -- unless the non-colour values are forced: sun and moon direction from the time of day turned through each body's three theme angles (Scalers 8..10 / 11..13), stored with the diffuse light direction (the sun and moon blended by Scalers[5], held at least DiffuseClampAngle below the horizon)
// Leaf: ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2CA0, 910 bytes (`ret`). Identity: Ego_r.pdb 0x006EE710 / FableWin.pdb 0x0244D960
//   ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ; reloc binding of the landed caller CEnvironment::Update 0x006B3FF0 (call 0x006B42A4).
//   Locals (Fable Anniversary PDB, weaker source): sun_vector, moon_vector, diffuse_vector (C3DVector).
//   Spellings: EVIDENCED by retail bytes -- GFCos / GFSin (constant 0x0124F2B8), two E11 / E33 stores of one CSE'd cosine, unary minus on the stored SunVector (reads through the copy's address in a general-purpose reg), X*X + Y*Y order. EVIDENCED by a twin -- Scalers read in place (landed CEnvironment::Update 0x006B3FF0: `GameTickState.BlendedTheme[0].Scalers[15]`, "the one spelling for every row of this TU").
// FITTED (chosen by grading, no behaviour change; owner acceptance test device a): (1) C3DVector(x, y, z) constructor for the two vectors (record leaves of the factor order); (2) the X argument written `((float)GFCos(t))`: the parenthesised narrowing keeps one PAREN tuple that feeds the constructor binding, so the list scheduler issues it ahead of the xx copy store and the store lands after fsin as retail 0x006B2CCF / 0x006B2DDE (the plain argument leaves an unused, low-priority PAREN and the store goes before fsin); (3) overlay opt-in FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF_PAREN5 (E31 = -E13 entry form; 3 + 2 PAREN tuples per SetYRotate + SetZRotate pair place the 81-tuple scheduling cuts: the sun / moon `lea ecx,[vector]` right before AccurateNormalise, 0x006B2DB9 / 0x006B2EBA).
#include "fable_compiler.h"
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TICKSTATES
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE
#define FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF_PAREN5
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#include <math.h>
#include "rebuild_abi.h"
#include "engine/GFCos.h"
#include "engine/GFSin.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnvironment.h"
#pragma intrinsic(sin, cos, sqrt)

void CEnvironment::UpdateSunAndMoonPositions()
{
    if (ForcedNonColourValues)
        return;
    CMatrix3x4 rotation;
    C3DVector sun_vector(((float)GFCos(GameTickState.Time)), GFSin(GameTickState.Time), 0.0f);
    rotation.SetYRotate(-GameTickState.BlendedTheme[0].Scalers[8]);
    rotation.TransformPoint(sun_vector);
    rotation.SetZRotate(GameTickState.BlendedTheme[0].Scalers[9]);
    rotation.TransformPoint(sun_vector);
    sun_vector.Z += GameTickState.BlendedTheme[0].Scalers[10];
    sun_vector.AccurateNormalise();
    C3DVector moon_vector(((float)GFCos(GameTickState.Time)), GFSin(GameTickState.Time), 0.0f);
    rotation.SetYRotate(-GameTickState.BlendedTheme[0].Scalers[11]);
    rotation.TransformPoint(moon_vector);
    rotation.SetZRotate(GameTickState.BlendedTheme[0].Scalers[12]);
    rotation.TransformPoint(moon_vector);
    moon_vector.Z += GameTickState.BlendedTheme[0].Scalers[13];
    moon_vector.AccurateNormalise();
    GameTickState.SunVector = sun_vector;
    GameTickState.MoonVector = moon_vector;
    C3DVector diffuse_vector = -GameTickState.SunVector * (1.0f - GameTickState.BlendedTheme[0].Scalers[5]) - GameTickState.MoonVector * GameTickState.BlendedTheme[0].Scalers[5];
    diffuse_vector.AccurateNormalise();
    float sin_clamp = GFSin(DiffuseClampAngle);
    if (-diffuse_vector.Z < sin_clamp) {
        diffuse_vector.Z = -sin_clamp;
        float scale = (float)sqrt((1.0f - sin_clamp * sin_clamp) / (diffuse_vector.X * diffuse_vector.X + diffuse_vector.Y * diffuse_vector.Y));
        diffuse_vector.X *= scale;
        diffuse_vector.Y *= scale;
        diffuse_vector.AccurateNormalise();
    }
    GameTickState.DiffuseVector = diffuse_vector;
}