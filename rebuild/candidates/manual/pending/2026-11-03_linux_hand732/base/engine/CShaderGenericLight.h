#ifndef FABLE_ENGINE_CSHADERGENERICLIGHT_H
#define FABLE_ENGINE_CSHADERGENERICLIGHT_H
#include "engine/C4DVector.h"
// Original Ego_r TPI0x40b52 and enum0x40b4d; native stride0x38 including owner flags.
class CShaderGenericLight {
public:
    enum EParameterType { PARAMTYPE_COLOUR,PARAMTYPE_DIRECTION,PARAMTYPE_POSITION,PARAMTYPE_ATTENUATION };
    enum { MAXIMUM_SIZE=2 };
    C4DVector Parameters[MAXIMUM_SIZE];
    EParameterType Types[MAXIMUM_SIZE];
    float Attenuation2,AttenuationBase;
    unsigned long Size;
};
FABLE_STATIC_ASSERT(sizeof(CShaderGenericLight)==0x34);
#endif
