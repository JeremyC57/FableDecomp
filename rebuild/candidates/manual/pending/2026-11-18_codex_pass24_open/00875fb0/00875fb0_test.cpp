#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_NORMALISE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "engine/C3DVector.h"
#include "engine/GFTanh.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include <cassert>
#include <cmath>
#include <cstdio>

#ifndef _MSC_VER
#define __fastcall
#endif

C3DVector& C3DVector::AccurateNormalise()
{
    *this *= 1.0f/std::sqrt(Dot(*this));
    return *this;
}

enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_SOLID };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_ADD };
struct CRGBColour
{
    unsigned char red,green,blue,alpha;
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
        : red(r),green(g),blue(b),alpha(a) { }
};
struct CEnginePrimitiveLine
{
    C3DVector first,second;
    CEnginePrimitiveLine(long id,const C3DVector& a,const C3DVector& b,const CRGBColour& col,float width,
        EEnginePrimitiveBlendMode blend,long state,bool active,EEngineInterpolationSet interpolation,EEnginePrimitiveBlendOp op)
        : first(a), second(b)
    {
        assert(id==0 && width==0.05f && state==0 && active);
        assert(col.red==255 && col.green==0 && col.blue==0 && col.alpha==255);
        assert(blend==ENGINE_PRIMITIVE_BLEND_SOLID && interpolation==ENGINE_INTERPOLATION_SET_DEFAULT && op==ENGINE_PRIMITIVE_BLEND_OP_ADD);
    }
};
struct RenderFixture
{
    C3DVector first[4],second[4];
    unsigned emitted;
    RenderFixture() : emitted(0) { }
    void AddPrimitive(const CEnginePrimitiveLine& line)
    { assert(emitted<4); first[emitted]=line.first; second[emitted++]=line.second; }
};
struct CEnginePrimitivePrepareInfo { RenderFixture& Engine; };
struct CMainGameComponent
{
    static long fps;
    static long GetConstantFPS() { return fps; }
};
long CMainGameComponent::fps;
struct CWorld
{
    static long frame;
    static long GetFrame() { return frame; }
};
long CWorld::frame;

// The selected implementation is reproduced verbatim for this standalone fixture.
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

int main()
{
    const C3DVector centre(2, -3, 5);
    const C3DVector normal(0, 1, 0);
    const float sx[4] = {-1, 1, 1, -1}, sy[4] = {1, 1, -1, -1};
    for (long fps = 20; fps <= 60; fps += 20) {
        for (long frame = 0; frame < 2 * fps + 5; ++frame) {
            RenderFixture fixture;
            CEnginePrimitivePrepareInfo info={fixture};
            CMainGameComponent::fps=fps;
            CWorld::frame=frame;
            NCameraModes::DrawSquare(info,centre,normal,4.0f);
            assert(fixture.emitted == 4);
            const double angle = (double(frame % (2 * fps)) / (2 * fps)) * double(6.2831855f);
            for (unsigned i = 0; i < 4; ++i) {
                const double x = centre.X + 2.0 * (sx[i] * std::cos(angle) - sy[i] * std::sin(angle));
                const double z = centre.Z - 2.0 * (sx[i] * std::sin(angle) + sy[i] * std::cos(angle));
                assert(std::fabs(fixture.first[i].X - x) < 0.00001);
                assert(fixture.first[i].Y == centre.Y);
                assert(std::fabs(fixture.first[i].Z - z) < 0.00001);
                assert(fixture.second[i].X == fixture.first[(i + 1) % 4].X);
                assert(fixture.second[i].Z == fixture.first[(i + 1) % 4].Z);
            }
        }
    }
    std::puts("00875fb0_TEST PASS");
    return 0;
}
