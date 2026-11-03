// NCameraModes::DrawPyramid(CEnginePrimitivePrepareInfo const &,C3DVector const &,float) -- draw three rising, shrinking, colour-cycling squares (an animated pyramid) over a camera position.
// Leaf: ?DrawPyramid@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@M@Z
// Grade: DIFFER(1329v1340) -- 131 lines, 11 bytes short, all frame layout + two x87 spots: (1) retail puts the inner-loop x temp at the frame top [esp+164h] (ours [esp+20h]) and the tick_period/ftol temp at [esp+20h]/[esp+5Ch], so every stack offset shifts by 4-16; (2) Y = cos*y - x*sin: retail keeps y on the x87 stack (fld qword [cos]; fmul st(2) ... fstp st(0)), VC7.1 RTM consumes it (fxch; fmul qword [cos]); (3) pt[c1]+=pos: retail loads pos.Y then adds pt.Y (fld [eax+4]; fadd [esi-10h]), ours the reverse; (4) the four CEnginePrimitiveLine temporaries sit ascending at 74h/B0h/ECh/128h in retail. 25 graded attempts (loop shapes, member vs local temporaries, named double cos/sin, unsigned/long tick_period, operand orders, separate x/y/z stores, line arrays vs named lines); the last ten moved nothing
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
#pragma intrinsic(sin,cos,exp)

namespace NCameraModes {
void __fastcall DrawPyramid(const CEnginePrimitivePrepareInfo& info,const C3DVector& pos,float size)
{
    for(long c2=0;c2<3;++c2)
    {
        unsigned long tick_period=3*CMainGameComponent::GetConstantFPS();
        float period=(float)tick_period;
        float phase=c2*(1.0f/3.0f);
        float theta=(float)((unsigned long)(CWorld::GetFrame()+phase*period)%tick_period)/period;
        float height=size*theta*1.5f;
        float radius=(1.05f-theta)*size;
        float d_mid=theta-0.5f;
        float d_end=theta-1.0f;
        CRGBColour col((unsigned char)((float)GFExp(-11.0f*GFSqr(theta))*255.0f),
                       (unsigned char)((float)GFExp(-11.0f*GFSqr(d_mid))*255.0f),
                       (unsigned char)((float)GFExp(-11.0f*GFSqr(d_end))*255.0f),255);
        double cos_theta=GFCos(theta);
        double sin_theta=GFSin(theta);
        C3DVector pt[4];
        for(long c1=0;c1<4;++c1)
        {
            float x=(long)(1.25f-GFSqr(1.5f-c1))*radius;
            float y=(long)GFTanh((1.5f-c1)*100.0f)*radius;
            pt[c1].X=(float)(x*cos_theta+y*sin_theta);
            pt[c1].Y=(float)(cos_theta*y-x*sin_theta);
            pt[c1].Z=height;
            pt[c1]+=pos;
        }
        CEnginePrimitiveLine line1(0,pt[0],pt[1],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
        info.Engine.AddPrimitive(line1);
        CEnginePrimitiveLine line2(0,pt[1],pt[2],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
        info.Engine.AddPrimitive(line2);
        CEnginePrimitiveLine line3(0,pt[2],pt[3],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
        info.Engine.AddPrimitive(line3);
        CEnginePrimitiveLine line4(0,pt[3],pt[0],col,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
        info.Engine.AddPrimitive(line4);
    }
}
}
