#ifndef FABLE_ENGINE_C3DBOUNDINGSPHERE_H
#define FABLE_ENGINE_C3DBOUNDINGSPHERE_H
#include "engine/C3DVector.h"
// Raw PDB full size16 and public Center+0, Radius+12; native primitive offsets agree.
class C3DBoundingSphere {
public:
#ifdef FABLE_RETAIL_MESH_PARTITION
 C3DBoundingSphere():Center(0.0f,0.0f,0.0f),Radius(0.0f){}
 void AccurateBuildFromPoints(const std::vector<C3DVector>&,float);
 float GetRadius()const{return Radius;}
#endif
    C3DVector Center;
    float Radius;
};
#endif
