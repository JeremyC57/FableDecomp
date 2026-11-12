#ifndef FABLE_ENGINE_CTVERTEXRHWCOLSPECTEX1_H
#define FABLE_ENGINE_CTVERTEXRHWCOLSPECTEX1_H
#include "rebuild_abi.h"
class C3DVector;class C2DVector;class CRGBColour;
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
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
#ifdef FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
// Retail opt-in (chunk Q20, row 0x009DDC60 CRenderManager2D::Draw2DBoxWithZ): FableWin.pdb
// ?SetRHWFromZ@?$CGenericVertex@VCTVertexRHWColSpecTex1Base@@@@QAEXXZ (debug copy 0x01AE1FD0, 139 bytes), inline in release: the
// 2D TU's screen RHW float 0x0139652C (landed spelling g_ScreenRHW) for Z in [0, 0.01), its negation for Z in [-0.01, 0), else 1/Z
// (retail 0x009DDCCE..0x009DDD70, expanded once per vertex).
#include "fable_compiler.h"
extern float g_ScreenRHW;                                                // 0x0139652C
template<class T> FABLE_FORCEINLINE void CGenericVertex<T>::SetRHWFromZ()
{
    if (this->Z >= 0.0f && this->Z < 0.01f)
        this->RHW = g_ScreenRHW;
    else if (this->Z >= -0.01f && this->Z < 0.0f)
        this->RHW = -g_ScreenRHW;
    else
        this->RHW = 1.0f / this->Z;
}
#endif
FABLE_STATIC_ASSERT(sizeof(CTVertexRHWColSpecTex1)==32);
#endif
