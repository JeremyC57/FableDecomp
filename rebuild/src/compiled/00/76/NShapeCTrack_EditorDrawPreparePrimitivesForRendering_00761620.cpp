// 00761620 NShape::CTrack::EditorDrawPreparePrimitivesForRendering,288bytes.
// Name/signature recovered from Ego_r PDB; RTTI CTrack slot3.
// For each segment draw previous->next and previous->ground-projected previous.
// Layout: no CTrack generated header (RTTI .?AVCTrack@NShape@@ : NShape::CShape@0); the Points vector (+0x08) is CShape's.
// CEnginePrimitiveLine is 0x3c (constructor 0x004C3090); info.Engine at +0x34.
// NF108 (2026-11-19, q4701): moved onto the shared headers: engine/NShape_CClosedShapeBase.h (CShape: Owner +0x04, Points +0x08),
//   engine/CEnginePrimitiveLine.h (the PDB constructor ??0CEnginePrimitiveLine@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MW4EEnginePrimitiveBlendMode@@K_N
//   W4EEngineInterpolationSet@@W4EEnginePrimitiveBlendOp@@@Z, Ego_r.pdb + FableWin.pdb), engine/C3DVector.h, engine/CRGBColour.h,
//   engine/CEnginePrimitivePrepareInfo.h + engine/CIEngine.h (AddPrimitive, slot 20) and engine/CWorldMap.h (GetGroundSizeZAt 0x0063A300,
//   ?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z, both PDBs). The row's own symbol is the PDB virtual
//   ?EditorDrawPreparePrimitivesForRendering@CTrack@NShape@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@ABVCRGBColour@@@Z.
// Leaf: ?EditorDrawPreparePrimitivesForRendering@CTrack@NShape@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@ABVCRGBColour@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/NShape_CClosedShapeBase.h"
namespace NShape {
class CTrack : public CShape { public:
 virtual void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const CRGBColour&);
};
void CTrack::EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info,const CWorldMap& world,const C3DVector&,const CRGBColour& color) {
 if(Points.size()>1) {
  std::vector<C3DVector>::iterator it=Points.begin();
  C3DVector previous=*it;
  ++it;
  for(;it!=Points.end();++it) {
   C3DVector ground;ground.X=previous.X;ground.Y=previous.Y;ground.Z=world.GetGroundSizeZAt(previous);
   info.Engine->AddPrimitive(CEnginePrimitiveLine(0,previous,*it,color,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
   info.Engine->AddPrimitive(CEnginePrimitiveLine(0,previous,ground,color,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));
   previous=*it;
  }
 }
}
}
