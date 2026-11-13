#ifndef FABLE_ENGINE_CGENERICVERTEX_H
#define FABLE_ENGINE_CGENERICVERTEX_H
class C3DVector;
// Original complete sea instantiation: Ego_r0x5d36a, FableWin0x6a524.
// All28 original nonvirtual members retained as declarations; copy operations implicit.
template<class Base> class CGenericVertex : public Base {
public:
    void SetX(float);
    void SetY(float);
    void SetZ(float);
    void SetXYZ(float,float,float);
    void SetPosition(const C3DVector&);
    void SetNormal(const C3DVector&);
    void SetRHW(float);
    void SetColour(unsigned long,unsigned long);
    void SetDiffuseColour(unsigned long);
    void SetSpecularColour(unsigned long);
    void SetU1(float);
    void SetV1(float);
    void SetU1V1(float,float);
    void SetBlendingWeight(unsigned long,float);
    void SetBlendingIndex(unsigned long,unsigned long);
    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetRHW() const;
    unsigned long GetDiffuseColour() const;
    unsigned long GetSpecularColour() const;
    float GetU1() const;
    float GetV1() const;
    C3DVector GetPosition() const;
    C3DVector GetNormal() const;
    float GetBlendingWeight(unsigned long) const;
    unsigned long GetBlendingIndex(unsigned long) const;
    void SetRHWFromZ();
};
#ifdef FABLE_RETAIL_CGENERICVERTEX_INLINE
// Retail opt-in (chunk 2005, row 0x00B51570): the FableWin.pdb inline setters (no out-of-line copy in retail): plain member stores.
template<class Base> inline void CGenericVertex<Base>::SetX(float x) { this->X = x; }
template<class Base> inline void CGenericVertex<Base>::SetY(float y) { this->Y = y; }
template<class Base> inline void CGenericVertex<Base>::SetZ(float z) { this->Z = z; }
template<class Base> inline void CGenericVertex<Base>::SetXYZ(float x, float y, float z) { this->X = x; this->Y = y; this->Z = z; }
template<class Base> inline void CGenericVertex<Base>::SetU1(float u) { this->U1 = u; }
template<class Base> inline void CGenericVertex<Base>::SetV1(float v) { this->V1 = v; }
template<class Base> inline void CGenericVertex<Base>::SetU1V1(float u, float v) { this->U1 = u; this->V1 = v; }
#endif
#endif
