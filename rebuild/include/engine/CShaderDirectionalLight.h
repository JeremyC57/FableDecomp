#ifndef FABLE_ENGINE_CSHADERDIRECTIONALLIGHT_H
#define FABLE_ENGINE_CSHADERDIRECTIONALLIGHT_H
#include "engine/CShaderLight.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
// Both original PDB records:60bytes; native constructor00B49950.
class CShaderDirectionalLight:public CShaderLight {
public:
    CShaderDirectionalLight(const C3DVector& direction,const CRGBFloatColour& colour,const CRGBFloatColour& back_colour);
    virtual void ConvertToGenericLight(CShaderGenericLight& light) const;
    C3DVector Direction,Position;
    CRGBFloatColour Colour,BackColour;
};
FABLE_STATIC_ASSERT(sizeof(CShaderDirectionalLight)==60);
FABLE_STATIC_ASSERT(offsetof(CShaderDirectionalLight,Direction)==4);
FABLE_STATIC_ASSERT(offsetof(CShaderDirectionalLight,Colour)==28);
#endif
