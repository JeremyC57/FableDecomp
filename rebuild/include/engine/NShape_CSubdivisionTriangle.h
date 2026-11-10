#ifndef FABLE_ENGINE_NSHAPE_CSUBDIVISIONTRIANGLE_H
#define FABLE_ENGINE_NSHAPE_CSUBDIVISIONTRIANGLE_H
#include "engine/NShape_CClosedShapeBase.h"
namespace NShape {
class CSubdivisionTriangle:public CClosedShapeBase {
public:
 virtual ~CSubdivisionTriangle();
 virtual bool IsPosInShape(const C3DVector&)const;
private:
 void GetUpDirection();
 C3DVector UpDirection;
};
}
FABLE_STATIC_ASSERT(sizeof(NShape::CSubdivisionTriangle)==44);
#endif
