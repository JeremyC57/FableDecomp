#ifndef FABLE_NBBBMATHS_CSPHERE_H
#define FABLE_NBBBMATHS_CSPHERE_H
// Retail layout (chunk 793): Ego_r.pdb CSphere (struct_layouts_egor.tsv: size 16, float Radius +0, C3DVector Base +4), class-keyed
// in namespace NBBBMaths (Ego_r/FableWin ??0CSphere@NBBBMaths@@QAE@ABVC3DVector@@M@Z). Retail CSphere::GetFirstIntersectionWithTriangle
// 0x00A58B50 reads Radius at [this+0] and Base at [this+4..0xC].
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace NBBBMaths {
class CSphere {
public:
 CSphere(const C3DVector& base,float radius):Radius(radius),Base(base){}
 bool GetFirstIntersectionWithFiniteLine(const C3DVector&,const C3DVector&,C3DVector&,C3DVector&)const;
 bool GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&,C3DVector&,C3DVector&)const;
private:
 float Radius;
 C3DVector Base;
};
}
#endif
