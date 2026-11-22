// NCameraModes::DrawPyramid(const CEnginePrimitivePrepareInfo&, const C3DVector&, float) -- draws an animated pyramid of three wire rings.
// Leaf: ?DrawPyramid@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@M@Z
// Flags: /Gr
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#define FABLE_RETAIL_CRGBCOLOUR
#include "rebuild_abi.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFSqr.h"
#include "engine/GFExp.h"
#include "engine/GFTanh.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#pragma intrinsic(sin, cos, exp)

namespace NCameraModes {
void __fastcall DrawPyramid(const CEnginePrimitivePrepareInfo& info, const C3DVector& centre, float size)
{
    C3DVector pt[4];
    CRGBColour col;
    long number_of_guys = 3;
    long seconds_per_cycle = 3;
    for (long c2 = 0; c2 < number_of_guys; ++c2) {
        unsigned long frames_per_cycle = seconds_per_cycle * CMainGameComponent::GetConstantFPS();
        // Inferred: the floating phase local keeps the retail reciprocal multiply before the frame conversion.
        const float phase = (float)c2 / number_of_guys;
        unsigned long number = (unsigned long)(CWorld::GetFrame() + phase * frames_per_cycle);
        unsigned long dyn_frame = number % frames_per_cycle;
        float scale_factor = (float)dyn_frame / frames_per_cycle;
        float dyn_z = (float)(scale_factor * 1.5f * size);
        float dyn_size = (float)((1.05f - scale_factor) * size);
        float x = scale_factor;
        float k = 11.0f;
        // Bracket: group the red and green fade products to reproduce the pointer setup at 0x0087662D.
        float red = (float)GFExp((-k * GFSqr(x)));
        float green = (float)GFExp((-k * GFSqr((float)(x - 0.5f))));
        float blue = (float)GFExp(-k * GFSqr((float)(x - 1.0f)));
        col = CRGBColour((unsigned char)(red * 255.0f), (unsigned char)(green * 255.0f), (unsigned char)(blue * 255.0f));
        for (long c1 = 0; c1 < 4; ++c1) {
            C3DVector temp_vec;
            temp_vec.X = (long)(1.25f - GFSqr((float)(1.5f - c1))) * dyn_size;
            temp_vec.Y = (long)GFTanh((1.5f - c1) * 100.0f) * dyn_size;
            temp_vec.Z = dyn_z;
            pt[c1].Z = temp_vec.Z;
            pt[c1].X = (float)(temp_vec.X * GFCos(scale_factor) + temp_vec.Y * GFSin(scale_factor));
            // Bracket: group the cosine term to retain the x87 sums and final stores at 0x008766CA.
            pt[c1].Y = (float)(-temp_vec.X * GFSin(scale_factor) + (temp_vec.Y * GFCos(scale_factor)));
            pt[c1] += centre;
        }
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[0], pt[1], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[1], pt[2], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[2], pt[3], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[3], pt[0], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
    }
}
}