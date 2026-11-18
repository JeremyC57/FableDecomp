#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/C3DVector.h"
#include <cassert>
#include <cmath>
#include <math.h>
#include <cstdio>

struct CRGBColour
{
    unsigned char R, G, B, A;
};

struct CWorldMap
{
    mutable unsigned samples;
    CWorldMap() : samples(0) { }
    float GetGroundSizeZAt(const C3DVector& point) const
    {
        ++samples;
        return 0.5f * point.X - 0.25f * point.Y;
    }
};

enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_NORMAL };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_NORMAL };

struct CEnginePrimitiveLine
{
    C3DVector from, to;
    CRGBColour colour;
    float width;
    CEnginePrimitiveLine() { }
    CEnginePrimitiveLine(long id, const C3DVector& start, const C3DVector& end,
                         const CRGBColour& input_colour, float input_width,
                         EEnginePrimitiveBlendMode blend, long flags, bool draw,
                         EEngineInterpolationSet interpolation, EEnginePrimitiveBlendOp operation)
        : from(start), to(end), colour(input_colour), width(input_width)
    {
        assert(id==0 && blend==ENGINE_PRIMITIVE_BLEND_NORMAL && flags==0 && draw);
        assert(interpolation==ENGINE_INTERPOLATION_SET_0 && operation==ENGINE_PRIMITIVE_BLEND_OP_NORMAL);
    }
};

struct EngineFixture
{
    CEnginePrimitiveLine lines[512];
    unsigned count;
    EngineFixture() : count(0) { }
    void AddPrimitive(const CEnginePrimitiveLine& line)
    {
        assert(count < sizeof lines / sizeof *lines);
        lines[count++] = line;
    }
};

struct CEnginePrimitivePrepareInfo
{
    EngineFixture* Engine;
};

namespace NShape {
void FABLE_FASTCALL DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& from,const C3DVector& to,const C3DVector& lod_centre,const CRGBColour& colour);
}

// The selected implementation is reproduced verbatim for this standalone fixture.
void FABLE_FASTCALL NShape::DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& from,const C3DVector& to,const C3DVector& lod_centre,const CRGBColour& colour)
{
 float ave_dist=((from+to)*0.5f-lod_centre).GetAccurateMagnitude();
 const float weight=tanh((ave_dist*0.05f));
 const float GRANULARITY=(float)(weight*5.0+0.25);
 C3DVector line=to-from;
 float magnitude=line.GetAccurateMagnitude();
 long max_index=(long)(magnitude/GRANULARITY)+1;
 C3DVector start_pos=from;
 start_pos.Z=wmap.GetGroundSizeZAt(start_pos)+0.15f;
 for(long c1=0;c1!=max_index;++c1){
  C3DVector end_pos=(c1==max_index-1)?to:from+line*GRANULARITY*(float)(c1+1)/magnitude;
  end_pos.Z=wmap.GetGroundSizeZAt(end_pos)+0.15f;
  info.Engine->AddPrimitive(CEnginePrimitiveLine(0,start_pos,end_pos,colour,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
  start_pos=end_pos;
 }
}

static void Check(bool descending, float distance, const C3DVector& lod)
{
    const C3DVector from(3.5f, -2.0f, 7.0f);
    const C3DVector to(3.5f + (descending ? -distance : distance), -2.0f, 7.0f);
    const CRGBColour colour = { 97, 113, 221, 68 };
    EngineFixture engine;
    CWorldMap wmap;
    CEnginePrimitivePrepareInfo info = { &engine };
    NShape::DrawPrimitiveLineOverLandscape(info, wmap, from, to, lod, colour);

    const double dx = (from.X + to.X) * 0.5 - lod.X;
    const double dy = from.Y - lod.Y;
    const double dz = from.Z - lod.Z;
    const double spacing = 0.25 + 5.0 * std::tanh(std::sqrt(dx * dx + dy * dy + dz * dz) * 0.05);
    const unsigned segments = static_cast<unsigned>(distance / spacing) + 1;
    assert(engine.count == segments && wmap.samples == segments + 1);

    for (unsigned i = 0; i < segments; ++i) {
        const CEnginePrimitiveLine& line = engine.lines[i];
        const double end_x = i + 1 == segments ? to.X :
                             from.X + (descending ? -1 : 1) * spacing * (i + 1);
        const float start_x = i == 0 ? from.X : engine.lines[i - 1].to.X;
        assert(line.from.X == start_x);
        assert(std::fabs(line.to.X - end_x) < 0.0001);
        assert(line.from.Y == -2.0f && line.to.Y == -2.0f);
        assert(std::fabs(line.from.Z - (0.5 * start_x + 0.65)) < 0.0001);
        assert(std::fabs(line.to.Z - (0.5 * end_x + 0.65)) < 0.0001);
        assert(line.colour.R == colour.R && line.colour.G == colour.G &&
               line.colour.B == colour.B && line.colour.A == colour.A);
        assert(line.width == 0.05f);
    }
}

int main()
{
    const float distances[] = { 0.0f, 0.125f, 1.5f, 5.5f, 13.0f, 27.5f };
    for (unsigned i = 0; i < sizeof distances / sizeof *distances; ++i) {
        Check(false, distances[i], C3DVector(3.5f, -2.0f, 7.0f));
        Check(true, distances[i], C3DVector(3.5f, -2.0f, 7.0f));
        Check(false, distances[i], C3DVector(19.0f, -20.0f, 100.0f));
        Check(true, distances[i], C3DVector(19.0f, -20.0f, 100.0f));
    }
    std::puts("00761280_TEST PASS");
    return 0;
}
