#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H
// Layout: original complete PDB fields; native sequence stride/offsets agree.
class CMatrix3x4 {public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 CMatrix3x4(){}
#ifdef FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM
 // Original lib_matrix.hpp in-place overload; FableWin locals xx/yy/zz.
 void TransformPoint(C3DVector& point) const {
  const float xx=point.X;
  const float yy=point.Y;
  const float zz=point.Z;
  point.X=E11*xx+(E21*yy+E31*zz)+E41;
  point.Y=E12*xx;
  point.Y+=E32*zz;
  point.Y+=E22*yy;
  point.Y+=E42;
  point.Z=E13*xx;
  point.Z+=E33*zz;
  point.Z+=E23*yy;
  point.Z+=E43;
 }
 void TransformPoint(const C3DVector& pSource,C3DVector& destination)const {
  destination.X=E11*pSource.X+(E21*pSource.Y+E31*pSource.Z)+E41;
  destination.Y=E12*pSource.X;
  destination.Y+=E32*pSource.Z;
  destination.Y+=E22*pSource.Y;
  destination.Y+=E42;
  destination.Z=E13*pSource.X;
  destination.Z+=E33*pSource.Z;
  destination.Z+=E23*pSource.Y;
  destination.Z+=E43;
 }
#endif
};
#endif
