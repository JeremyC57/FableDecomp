// NCameraModes::DrawSquare(const CEnginePrimitivePrepareInfo& info, const C3DVector& centre, const C3DVector& outward_normal, float size) -- draw a red square of side `size` around `centre`, in the plane facing `outward_normal`, spinning once every two seconds.
// Grade: DIFFER(1315v1315) -- The loop initializer is scheduled after two float stores where retail places it between them at 0x008760C0.
// Leaf: ?DrawSquare@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@1M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00875FB0, 1315 bytes, __fastcall `ret 8`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFTanh.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CIEngine.h"
namespace NCameraModes {
void __fastcall DrawSquare(const CEnginePrimitivePrepareInfo& info, const C3DVector& centre, const C3DVector& outward_normal, float size)
{
    float period = 2.0f;
    long tick_period = (long)(period * CMainGameComponent::GetConstantFPS());
    float theta = (float)(CWorld::GetFrame() % tick_period) / tick_period;
    float scale_factor = 0.5f;
    C3DVector lx_p(1.0f, (-1.0f / outward_normal.Y) * outward_normal.X, 0.0f);
    lx_p.AccurateNormalise();
    C3DVector ly_p = outward_normal.Cross(lx_p);
    ly_p.AccurateNormalise();
    C3DVector r_lx_p = lx_p * (float)GFCos(theta) + ly_p * (float)GFSin(theta);
    C3DVector r_ly_p = ly_p * (float)GFCos(theta) - lx_p * (float)GFSin(theta);
    C3DVector pt[4];
    for (long c1 = 0; c1 < 4; ++c1) {
        pt[c1].X = ((long)(1.25f - GFSqr(1.5f - c1)) * r_lx_p.X + (long)GFTanh((1.5f - c1) * 100.0f) * r_ly_p.X) * size * scale_factor;
        pt[c1].Y = ((long)(1.25f - GFSqr(1.5f - c1)) * r_lx_p.Y + (long)GFTanh((1.5f - c1) * 100.0f) * r_ly_p.Y) * size * scale_factor;
        pt[c1].Z = ((long)(1.25f - GFSqr(1.5f - c1)) * r_lx_p.Z + (long)GFTanh((1.5f - c1) * 100.0f) * r_ly_p.Z) * size * scale_factor;
        pt[c1] += centre;
    }
    CRGBColour col(255, 0, 0, 255);
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[0], pt[1], col, 0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[1], pt[2], col, 0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[2], pt[3], col, 0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[3], pt[0], col, 0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
}
}
