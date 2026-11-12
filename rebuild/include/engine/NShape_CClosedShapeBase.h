#ifndef FABLE_ENGINE_NSHAPE_CCLOSEDSHAPEBASE_H
#define FABLE_ENGINE_NSHAPE_CCLOSEDSHAPEBASE_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CTCShapeManager;
class CPersistContext;
class C2DCoordI;
class CEnginePrimitivePrepareInfo;
class CWorldMap;
class CRGBColour;
namespace NShape {
enum EShape;
class CShape {
public:
 CShape(CTCShapeManager&,const std::vector<C3DVector>&);
 virtual ~CShape();
 virtual EShape GetType()const=0;
 virtual void OnSerialise(CPersistContext&,long,const C2DCoordI&);
 virtual void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const CRGBColour&)=0;
 virtual bool IsPosInShape(const C3DVector&)const=0;
 virtual void UpdateSubdivisions();
#ifdef FABLE_RETAIL_NSHAPE_CSHAPE_GETSTARTPOINT
 // Retail opt-in (wf3, row 0x00761980): ?GetStartPoint@CShape@NShape@@QBE_NAAVC3DVector@@@Z (FableWin.pdb 0x01818274 / 0x025D6620; no
 // out-of-line copy in Ego_r.pdb or retail). Inline in retail: expanded in CTCShapeManager::DrawShapes 0x007619F9 and
 // CTCShapeManager::GetPrincipalVector 0x0076091D (`cmp begin,end ; je` then the 12-byte block copy of the first point into the
 // caller's vector through the reference). Body inferred from those two sites. No layout change.
 bool GetStartPoint(C3DVector& pos)const{if(Points.empty())return false;pos=*Points.begin();return true;}
#endif
#ifdef FABLE_RETAIL_NSHAPE_CSHAPE_GETSTARTPOINT_DECLARATION
 // Retail opt-in (wf3 attempt 2, row 0x00761980): the same member as a declaration only, for a TU that carries the body itself.
 bool GetStartPoint(C3DVector& pos)const;
#endif
#ifdef FABLE_RETAIL_NSHAPE_CSHAPE_EDITORDRAWHIGHLIGHTPOINTS
 // Retail opt-in (wf3, row 0x00761980): ?EditorDrawHighlightPoints@CShape@NShape@@QAEXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@ABVCRGBColour@@@Z
 // 0x007617A0 (Ego_r.pdb + FableWin.pdb). Non-virtual declaration only: no layout change.
 void EditorDrawHighlightPoints(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const CRGBColour&);   // 0x007617A0
#endif
 CTCShapeManager& Owner;
 std::vector<C3DVector> Points;
 std::vector<std::vector<C3DVector>::iterator> SelectedPoints;
};
class CClosedShapeBase:public CShape {
public:
 CClosedShapeBase(CTCShapeManager&,const std::vector<C3DVector>&);
 virtual ~CClosedShapeBase();
 virtual EShape GetType()const;
 virtual void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const CRGBColour&);
};
void FABLE_FASTCALL DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const C3DVector&,const C3DVector&,const CRGBColour&);
}
FABLE_STATIC_ASSERT(sizeof(NShape::CShape)==32);
FABLE_STATIC_ASSERT(sizeof(NShape::CClosedShapeBase)==32);
#endif
