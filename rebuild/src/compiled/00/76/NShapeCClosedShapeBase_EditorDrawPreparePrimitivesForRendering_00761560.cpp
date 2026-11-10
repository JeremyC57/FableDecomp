// 00761560 NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering -- draw the closed point loop.
// Leaf: ?EditorDrawPreparePrimitivesForRendering@CClosedShapeBase@NShape@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@ABVCRGBColour@@@Z
// Retail:00761560,186 bytes.
// Flags: none
#define FABLE_RETAIL_CIENGINE_SCREENPOS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_DECLARATION
#include "engine/NShape_CClosedShapeBase.h"
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CWorldMap.h"
extern double FABLE_FASTCALL GFTanh(double);
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
void FABLE_FASTCALL NShape::DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& from,const C3DVector& to,const C3DVector& lod_centre,const CRGBColour& colour)
{
 float ave_dist=((from+to)*0.5f-lod_centre).GetAccurateMagnitude();
 const float weight=0.05f;
 const float GRANULARITY=GFTanh(weight*ave_dist)*5.0+0.25;
 C3DVector line=to-from;
 float magnitude=line.GetAccurateMagnitude();
 long max_index=(long)(magnitude/GRANULARITY);
 ++max_index;
 C3DVector start_pos=from;
 start_pos.Z=wmap.GetGroundSizeZAt(start_pos)+0.15f;
 for(long c1=0;c1!=max_index;++c1){
  C3DVector end_pos=(c1==max_index-1)?to:from+line*GRANULARITY*(float)(c1+1)/magnitude;
  end_pos.Z=wmap.GetGroundSizeZAt(end_pos)+0.15f;
  info.Engine->AddPrimitive(CEnginePrimitiveLine(0,start_pos,end_pos,colour,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
  start_pos=end_pos;
 }
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& lod_centre,const CRGBColour& colour)
{
 if(!Points.empty()){
  std::vector<C3DVector>::const_iterator it=Points.begin();
  C3DVector start_pos=*it++;
  while(it!=Points.begin()){
   C3DVector end_pos;
   if(it==Points.end()){
    it=Points.begin();
    end_pos=*it;
   }else end_pos=*it++;
   DrawPrimitiveLineOverLandscape(info,wmap,start_pos,end_pos,lod_centre,colour);
   start_pos=end_pos;
  }
 }
}