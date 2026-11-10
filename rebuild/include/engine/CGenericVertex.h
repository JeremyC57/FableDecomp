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
#endif
