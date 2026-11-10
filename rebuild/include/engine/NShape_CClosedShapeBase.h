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
