#ifndef FABLE_ENGINE_CTVERTEXRHWCOLSPECTEX1_RENDER2D_H
#define FABLE_ENGINE_CTVERTEXRHWCOLSPECTEX1_RENDER2D_H
#include "rebuild_abi.h"
class C3DVector;class C2DVector;class CRGBColour;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CTVertexRHWColSpecTex1Base {
public: float X;
public: float Y;
public: float Z;
public: float RHW;
public: unsigned long DiffuseColour;
public: unsigned long SpecularColour;
public: float U1;
public: float V1;
};
template<class T> class CGenericVertex:public T {
public: void SetX(float);
public: void SetY(float);
public: void SetZ(float);
public: void SetXYZ(float,float,float);
public: void SetPosition(const C3DVector &);
public: void SetNormal(const C3DVector &);
public: void SetRHW(float);
public: void SetColour(unsigned long,unsigned long);
public: void SetDiffuseColour(unsigned long);
public: void SetSpecularColour(unsigned long);
public: void SetU1(float);
public: void SetV1(float);
public: void SetU1V1(float,float);
public: void SetBlendingWeight(unsigned long,float);
public: void SetBlendingIndex(unsigned long,unsigned long);
public: float GetX() const;
public: float GetY() const;
public: float GetZ() const;
public: float GetRHW() const;
public: unsigned long GetDiffuseColour() const;
public: unsigned long GetSpecularColour() const;
public: float GetU1() const;
public: float GetV1() const;
public: C3DVector GetPosition() const;
public: C3DVector GetNormal() const;
public: float GetBlendingWeight(unsigned long) const;
public: unsigned long GetBlendingIndex(unsigned long) const;
public: void SetRHWFromZ();
};
class CTVertexRHWColSpecTex1:public CGenericVertex<CTVertexRHWColSpecTex1Base> {};
FABLE_STATIC_ASSERT(sizeof(CTVertexRHWColSpecTex1)==32);
#endif
