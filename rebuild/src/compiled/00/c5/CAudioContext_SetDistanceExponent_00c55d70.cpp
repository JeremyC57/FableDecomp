// CAudioContext::SetDistanceExponent(long exponent) -- slot 7 of libpk's OpenAL context: select model 2 (+0x108) and store the factor 2^-exponent (+0xF8)
// Leaf: ?SetDistanceExponent@CAudioContext@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55D70, 40 bytes, thiscall `ret 4`.
// Identity: slot 7 of the CAudioContext vtable 0x012B8C18 (the landed destructor 0x00C56370 stores it; slot 0 the scalar
//   deleting destructor 0x00C565D0). Descriptive (libpk, no PDB/RTTI): the method name and the two fields' roles are
//   inferred. Constant 0x01244480 = 2.0 (double); pow is the /Oi intrinsic (`call _CIpow`, 0x00BFEB40).
// Shape: both pow operands are loaded before the constant store; `-(double)exponent` is `fild ; fchs`.
#include <math.h>

struct CAudioContext {                          // vtable 0x012B8C18
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void SetDistanceExponent(long exponent);    // slot 7
    unsigned char _pad04[0xf8 - 4];
    float m_distanceFactor;                     // +0xf8
    unsigned char _padFC[0x108 - 0xfc];
    long m_distanceModel;                       // +0x108
};

void CAudioContext::SetDistanceExponent(long exponent)
{
    m_distanceModel = 2;
    m_distanceFactor = (float)pow(2.0, -(double)exponent);
}