// NCameraModes::DrawSquare(CEnginePrimitivePrepareInfo const &,C3DVector const &,C3DVector const &,float) -- draw the camera square from its supplied position, direction and scale.
// Leaf: ?DrawSquare@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@1M@Z
// Grade: DIFFER(1315v1315) -- 6 lines, loop-setup scheduling: retail issues 'xor ebx,ebx / mov [esp+18h],ebx / lea esi,[esp+94h]' between the rotated-axis fstp's, VC7.1 RTM interleaves them two FP stores later; QFE 4035 identical; 21 shapes over two agents (pt/c1/colour declaration placement, unsigned counter, r_ly_p default-constructed, element reference, c1!=4, named sin/cos locals, folded half-size, loop-invariant hoists) leave it or regress
// Agent724b (2026-11-03): the earlier "timeout" was Wine cl.exe spinning after fatal C1083 -- the Codex draft included headers the repo does not have (engine/GFMaths.h, engine/CMainGameComponent.h, engine/CEnginePrimitivePrepareInfo.h). Overlay headers now exist (hdr/engine); the row compiles in ~0.5 s and reproduces Codex's residue. 11 graded attempts this session; the last three moved nothing. Grade with FABLE_HDR_OVERLAY=agent724b/hdr.
// Flags: /Gr
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/GFMaths.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#pragma intrinsic(sin,cos)

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
