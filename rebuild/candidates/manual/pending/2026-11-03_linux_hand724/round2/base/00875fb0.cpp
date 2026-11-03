// NCameraModes::DrawSquare(CEnginePrimitivePrepareInfo const &,C3DVector const &,C3DVector const &,float) -- draw the camera square from its supplied position, direction and scale.
// Leaf: ?DrawSquare@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@1M@Z
// Grade: DIFFER(1315v1315) -- loop-setup scheduling: retail issues 'xor ebx,ebx / mov [esp+18h],ebx / lea esi,[esp+94h]' between the rotated-axis fstp's, VC7.1 RTM interleaves them two FP stores later; 11 shapes (pt/c1/colour declaration placement, unsigned counter, r_ly_p default-constructed, element reference, c1!=4, named sin/cos locals, folded half-size) leave it or regress. Graded in Codex's selected header context (FABLE_RETAIL_CAMERA_DRAW / FABLE_RETAIL_VECTOR_SPLINE per-feature macros = header debt)
// Flags: /Gr
#define FABLE_RETAIL_CAMERA_DRAW
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_VECTOR_SPLINE
#include <math.h>
#include "engine/GFMaths.h"
#pragma intrinsic(sin,cos)
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
extern const double TWO_PI;
inline double __fastcall GFSin(float angle){return sin(angle*TWO_PI);}
inline double __fastcall GFCos(float angle){return cos(angle*TWO_PI);}
inline double __fastcall GFTanh(double value){return tanh(value);}

namespace NCameraModes {
void __fastcall DrawSquare(const CEnginePrimitivePrepareInfo& info,const C3DVector& centre,const C3DVector& outward_normal,float size)
{
    float period=2.0f;
    long tick_period=(long)(period*CMainGameComponent::GetConstantFPS());
    float theta=(float)(CWorld::GetFrame()%tick_period)/tick_period;
    float scale_factor=0.5f;
    C3DVector lx_p(1.0f,(-1.0f/outward_normal.Y)*outward_normal.X,0.0f);
    lx_p.AccurateNormalise();
    C3DVector ly_p=outward_normal.Cross(lx_p);
    ly_p.AccurateNormalise();
    C3DVector r_lx_p=lx_p*(float)GFCos(theta)+ly_p*(float)GFSin(theta);
    C3DVector r_ly_p=ly_p*(float)GFCos(theta)-lx_p*(float)GFSin(theta);
    C3DVector pt[4];
    for(long c1=0;c1<4;++c1)
    {
        pt[c1].X=((long)(1.25f-GFSqr((1.5f-c1)))*r_lx_p.X+(long)GFTanh((1.5f-c1)*100.0f)*r_ly_p.X)*size*scale_factor;
        pt[c1].Y=((long)(1.25f-GFSqr((1.5f-c1)))*r_lx_p.Y+(long)GFTanh((1.5f-c1)*100.0f)*r_ly_p.Y)*size*scale_factor;
        pt[c1].Z=((long)(1.25f-GFSqr((1.5f-c1)))*r_lx_p.Z+(long)GFTanh((1.5f-c1)*100.0f)*r_ly_p.Z)*size*scale_factor;
        pt[c1]+=centre;
    }
    CRGBColour col(255,0,0,255);
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0,pt[0],pt[1],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0,pt[1],pt[2],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0,pt[2],pt[3],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(0,pt[3],pt[0],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
}
}
