#ifndef FABLE_ENGINE_NSHAPE_CBASICSHAPE_H
#define FABLE_ENGINE_NSHAPE_CBASICSHAPE_H
#include "engine/NShape_CClosedShapeBase.h"
#include "engine/CCountedPointer.h"
namespace NShape {
class CSubdivisionTriangle;
// Both original complete types: CClosedShapeBase and own vector at+20, native44 bytes.
class CBasicShape:public CClosedShapeBase {
public:
 CBasicShape(CTCShapeManager&,const std::vector<C3DVector>&);
 virtual ~CBasicShape();
 virtual bool IsPosInShape(const C3DVector&)const;
 virtual void UpdateSubdivisions();
private:std::vector<CCountedPointer<CSubdivisionTriangle> > Subdivisions;
};
}
FABLE_STATIC_ASSERT(sizeof(NShape::CBasicShape)==44);
#endif
