// NShape::DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info, const CWorldMap& wmap, const C3DVector& from, const C3DVector& to, const C3DVector& lod_centre, const CRGBColour& colour) -- draws from->to as a chain of ground-hugging segments (each vertex 0.15 above the landscape), segment length tanh(ave_dist*0.05)*5+0.25 from the midpoint's distance to lod_centre
// Grade: DIFFER(733v733) -- one store: max_index's home store `mov [esp+20h],edi` scheduled at 0x00761374 (retail 0x0076138B, after the GetGroundSizeZAt argument push); every other byte matches
// Leaf: ?DrawPrimitiveLineOverLandscape@NShape@@YIXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@22ABVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00761280, 733 bytes, __fastcall, `ret 10h`.
// Ego_d.pdb (tc_geometric_shapes.obj): params info, wmap, from, to, lod_centre, colour; locals start_pos, ave_dist, magnitude, const float weight, line, long max_index, const float GRANULARITY; { long c1 { end_pos } }. FITTED: which of weight / GRANULARITY holds which value (landed 0x00761560 has the swapped naming); the (float) cast on GRANULARITY (moves the store one instruction).
#define FABLE_RETAIL_CIENGINE_SCREENPOS
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/NShape_CClosedShapeBase.h"
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CWorldMap.h"
#include <math.h>
void FABLE_FASTCALL NShape::DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& from,const C3DVector& to,const C3DVector& lod_centre,const CRGBColour& colour)
{
 float ave_dist=((from+to)*0.5f-lod_centre).GetAccurateMagnitude();
 const float weight=tanh(ave_dist*0.05f);
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
