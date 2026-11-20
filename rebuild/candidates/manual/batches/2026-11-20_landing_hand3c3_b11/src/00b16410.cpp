// NParticleEngine::CPSCOrbit::COrbitState::Update(const COrbitSettings&, CMatrix3x4&, float) -- advance the orbit angle by the per-tick rotate speed, shift the from / to / next position ring, compute the next orbit offset (a squeezed circle, with an optional cycling or expanding radius) in the plane picked by the orbit type, transform it by the parent matrix (or only translate it), pre-rotate the matrix about the plane normal by the orbit angle, move it to the new point and take the half-difference tangent
// Leaf: ?Update@COrbitState@CPSCOrbit@NParticleEngine@@QAEXABVCOrbitSettings@23@AAVCMatrix3x4@@M@Z
// Flags: /d2inlT230
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16410, 1671 bytes, `ret 0Ch`.
// Retail-asm: 0x00B164AA `fld dword ptr [esp+1Ch]` / 0x00B164AE `fistp dword ptr [esp+10h]` (and the same fld / fistp pair at
//   0x00B164FA, 0x00B16552, 0x00B165A2, 0x00B16617, 0x00B16664, 0x00B16710, 0x00B16786): the shared engine/GFFloatToLongNear.h helper
//   inside the eight GFFastCos / GFFastSin expansions (engine/GFFastCos.h, engine/GFFastSin.h); no asm in this file.
// Identity: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?Update@COrbitState@CPSCOrbit@NParticleEngine@@QAEXABVCOrbitSettings@23@AAVCMatrix3x4@@M@Z;
//   parameters and locals as Ego_d.pdb lists them (settings, parent_transform, time_secs; next_pos_2d (C2DVector), orient; radius;
//   temp_x, temp_y, cos_a, sin_a), statements as its line table (lines 54..143, one statement per entry: e.g. the 26-byte condition
//   lines = one bool call GFIsNonZero(..), the 31-byte lines = NextPosition.Initialise(x, y, z), 8 bytes = next_pos_2d.Clear(), 11 / 10
//   bytes = the Type masks, 19 bytes = Pre?Rotate(-NextAngle), 15 = SetTranslateOnly; the 270-byte entry is the debug ASSERT of the
//   first switch's default and has no retail code).
// Callees: NParticleEngine::GetOneOverTickRate 0x00AEAA00 (FableWin / Ego_d ?GetOneOverTickRate@NParticleEngine@@YIMXZ) is the one call.
//   Expanded in place: GFIsNonZero / GFFloatAbs (?GFIsNonZero@@YI_NM@Z, ?GFFloatAbs@@YIMM@Z; ?DELTA_NOTIONAL_ZERO@@3MB 0x0129BA3C),
//   GFFastCos / GFFastSin (?GFastCosTable@@3PAMA 0x013CD550), C2DVector::Clear, C3DVector::Initialise / operator+= / operator- /
//   operator*, CMatrix3x4::TransformPoint / GetTranslation / PreZRotate / PreYRotate / PreXRotate / SetTranslateOnly
//   (GFSin / GFCos on the double TWO_PI 0x0124F2B8).
// Layout: COrbitSettings::Type is one long in both PDBs (no bitfield): the low two bits are the orbit plane, bit 2 says whether the
//   orbit point goes through the whole parent matrix or is only translated (`and edi,3` 0x00B167EF, `test byte ptr [ecx],4` 0x00B1682A).
// Inferred: (1) /d2inlT230: with VC7.1's default inline threshold the plain-inline shared helpers are called out of line (842 bytes);
//   retail expands every one. Window measured: T229 and below differ (T200 1601, T212 1544, T225 / T228 / T229 1614 bytes), T230,
//   T231, T234, T237, T250, T300, T400, T800, T2000, T9999 are all the same object (measured on this text and on the one before the TransformPoint choice). (2) TransformPoint in its plain z, y, x sum text
//   (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS): the default parenthesised text emits the same TransformPoint instructions here
//   but orders two commutative sums of the PreYRotate expansion the other way (0x00B16967 / 0x00B1698B, 1669 bytes, 66 instructions
//   off). (3) The last PreXRotate case has no `break` (Ego_d line 137 is followed directly by line 139; same code either way).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_CLEAR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_CMATRIX3X4_PREROTATE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "fable_compiler.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/GFFastCos.h"
#include "engine/GFFastSin.h"
#include "engine/GFIsNonZero.h"
#include "engine/NParticleEngine_CPSCOrbit.h"

namespace NParticleEngine {

float FABLE_FASTCALL GetOneOverTickRate();      // 0x00AEAA00

void CPSCOrbit::COrbitState::Update(const COrbitSettings& settings, CMatrix3x4& parent_transform, float time_secs)
{
    NextAngle += (settings.RotateSpeed + RotateSpeedRandomValue) * GetOneOverTickRate();
    FromPosition = ToPosition;
    ToPosition = NextPosition;
    C2DVector next_pos_2d;
    if (settings.Enabled)
    {
        if (GFIsNonZero(settings.SqueezeAngle))
        {
            float temp_x = GFFastCos(NextAngle + settings.SqueezeAngle);
            float temp_y = GFFastSin(NextAngle + settings.SqueezeAngle) * settings.SqueezeScale;
            float cos_a = GFFastCos(-settings.SqueezeAngle);
            float sin_a = GFFastSin(-settings.SqueezeAngle);
            next_pos_2d.X = temp_x * cos_a + temp_y * sin_a;
            next_pos_2d.Y = temp_y * cos_a - temp_x * sin_a;
        }
        else
        {
            next_pos_2d.X = GFFastCos(NextAngle);
            next_pos_2d.Y = GFFastSin(NextAngle) * settings.SqueezeScale;
        }
        float radius = settings.Radius;
        if (GFIsNonZero(settings.Expand))
        {
            if (GFIsNonZero(settings.CycleTime))
            {
                if (settings.CycleFlag)
                {
                    radius += (1.0f - GFFastCos(time_secs / settings.CycleTime)) * settings.Expand * 0.5f;
                }
                else
                {
                    if (time_secs > settings.CycleTime) time_secs = settings.CycleTime;
                    radius += GFFastSin(time_secs / settings.CycleTime * 0.25f) * settings.Expand;
                }
            }
        }
        next_pos_2d.X *= radius;
        next_pos_2d.Y *= radius;
    }
    else
    {
        next_pos_2d.Clear();
    }
    long orient = settings.Type & 3;
    switch (orient)
    {
    case 0:
        NextPosition.Initialise(next_pos_2d.X, next_pos_2d.Y, 0.0f);
        break;
    case 1:
        NextPosition.Initialise(next_pos_2d.X, 0.0f, next_pos_2d.Y);
        break;
    case 2:
        NextPosition.Initialise(0.0f, next_pos_2d.X, next_pos_2d.Y);
        break;
    }
    if (settings.Type & 4)
    {
        parent_transform.TransformPoint(NextPosition);
    }
    else
    {
        NextPosition += parent_transform.GetTranslation();
    }
    switch (orient)
    {
    case 0:
        parent_transform.PreZRotate(-NextAngle);
        break;
    case 1:
        parent_transform.PreYRotate(-NextAngle);
        break;
    case 2:
        parent_transform.PreXRotate(-NextAngle);
    }
    parent_transform.SetTranslateOnly(NextPosition);
    FromTangent = ToTangent;
    ToTangent = (NextPosition - FromPosition) * 0.5f;
}

} // namespace NParticleEngine
