#ifndef FABLE_NBBBMATHS_CCYLINDER_H
#define FABLE_NBBBMATHS_CCYLINDER_H
// Complete original type: raw PDB field list 0x3f23c, size28 and native offsets0/4/16.
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace NBBBMaths {
class CCylinder {
public:
 CCylinder(const C3DVector& bottom,const C3DVector& top,float radius):Radius(radius),AxisBottom(bottom),AxisTop(top){}
 bool GetFirstIntersectionWithFiniteLine(const C3DVector&,const C3DVector&,C3DVector&,C3DVector&)const;
 bool GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&,C3DVector&,C3DVector&)const;
private:
 float Radius;
 C3DVector AxisBottom;
 C3DVector AxisTop;
};
}
#endif
