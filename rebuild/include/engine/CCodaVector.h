#ifndef FABLE_CCODAVECTOR_H
#define FABLE_CCODAVECTOR_H
#include "engine/CodaEnvironment.h"
#ifdef FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include <math.h>   // Magnitude: the C++ inline sqrtf (SDK release COMDAT _sqrtf)
#endif
// Original SDK CodeView: complete vector is the public _CVECTOR base.
#ifdef FABLE_RETAIL_PASS22_CCODA3D_CREATE
class CCodaVector:public _CVECTOR {public:
 CCodaVector(){}
 CCodaVector(float a,float b,float c){x=a;y=b;z=c;}
 CCodaVector(const _CVECTOR& v){x=v.x;y=v.y;z=v.z;}
 CCodaVector& operator=(const CCodaVector& v){x=v.x;y=v.y;z=v.z;return *this;}
#ifdef FABLE_RETAIL_CCODAVECTOR_ENV_SCALE_PASS29
 // GameCODA CodeView Scale(float) and the debug CodaEnvironment.obj component writes at offsets 0, 4 and 8.
 void Scale(float fScale){x *= fScale; y *= fScale; z *= fScale;}
#endif
 void Set(float a,float b,float c){x=a;y=b;z=c;}
 void Zero(){x=y=z=0.0f;}
#ifdef FABLE_RETAIL_CCODA3DSOURCE_COMMIT
 // Retail opt-in (hand3c, row 0x00C41FB0 CCoda3DSource::Commit): SDK 2.10 CodeView ?IsZero@ (QBE_NXZ), ?MagnitudeSquare@ /
 // ?Magnitude@ (QBEMXZ), ??H / ??G (QBE?AV0@ABV0@@Z) of CCodaVector: Dbg COMDAT bodies (Magnitude calls sqrtf); the Toolkit twin
 // CCtkVector (CtkVector.h) has the same IsZero / MagnitudeSquared / operator+ / operator-.
 bool IsZero()const{return x == 0.0f && y == 0.0f && z == 0.0f;}
 float MagnitudeSquare()const{return x * x + y * y + z * z;}
 float Magnitude()const{return sqrtf(MagnitudeSquare());}
 CCodaVector operator+(const CCodaVector& v)const{return CCodaVector(x + v.x, y + v.y, z + v.z);}
 CCodaVector operator-(const CCodaVector& v)const{return CCodaVector(x - v.x, y - v.y, z - v.z);}
#endif
#ifdef FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
 // GameCODA 2.10 CodaListener.obj (CODAApi.lib / CODAApiDbg.lib): Normalize 0x00C402E0 out of line (returns the old magnitude);
 // Dot and OrthogonalRight (0x00C40370, the COMDAT copy) are header inlines, bodies as the SDK's Toolkit CCtkVector (CtkVector.h).
 float Normalize();
 float Dot(const CCodaVector& v)const
  { return x * v.x + y * v.y + z * v.z; }
#ifdef FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE
 // NF95 opt-in (row 0x00C40370, the out-of-line COMDAT copy of OrthogonalRight): declaration only; the row defines the same body.
 void OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop);
#else
 void OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop)
 {
  x = vTop.y * vFront.z - vFront.y * vTop.z;
  y = vTop.z * vFront.x - vFront.z * vTop.x;
  z = vTop.x * vFront.y - vFront.x * vTop.y;
 }
#endif
#endif
};
#else
class CCodaVector:public _CVECTOR {
#ifdef FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
public:
 // GameCODA 2.10 CodaListener.obj (CODAApi.lib / CODAApiDbg.lib): Normalize 0x00C402E0 out of line (returns the old magnitude);
 // Dot and OrthogonalRight (0x00C40370, the COMDAT copy) are header inlines, bodies as the SDK's Toolkit CCtkVector (CtkVector.h).
 float Normalize();
 float Dot(const CCodaVector& v)const
  { return x * v.x + y * v.y + z * v.z; }
#ifdef FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE
 // NF95 opt-in (row 0x00C40370, the out-of-line COMDAT copy of OrthogonalRight): declaration only; the row defines the same body.
 void OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop);
#else
 void OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop)
 {
  x = vTop.y * vFront.z - vFront.y * vTop.z;
  y = vTop.z * vFront.x - vFront.z * vTop.x;
  z = vTop.x * vFront.y - vFront.x * vTop.y;
 }
#endif
#endif
};
#endif
#endif
