#ifndef FABLE_CCODAVECTOR_H
#define FABLE_CCODAVECTOR_H
#include "engine/CodaEnvironment.h"
// Original SDK CodeView: complete vector is the public _CVECTOR base.
#ifdef FABLE_RETAIL_PASS22_CCODA3D_CREATE
class CCodaVector:public _CVECTOR {public:
 CCodaVector(){}
 CCodaVector(float a,float b,float c){x=a;y=b;z=c;}
 CCodaVector(const _CVECTOR& v){x=v.x;y=v.y;z=v.z;}
 CCodaVector& operator=(const CCodaVector& v){x=v.x;y=v.y;z=v.z;return *this;}
 void Set(float a,float b,float c){x=a;y=b;z=c;}
 void Zero(){x=y=z=0.0f;}
};
#else
class CCodaVector:public _CVECTOR {};
#endif
#endif
