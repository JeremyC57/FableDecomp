#ifndef FABLE_ENGINE_CSHADERLIGHT_H
#define FABLE_ENGINE_CSHADERLIGHT_H
#include "engine/CBaseClassNonCopyable.h"
// Both original PDB class records: four bytes, public CBaseClassNonCopyable base.
class CShaderGenericLight;
class CShaderLight:public CBaseClassNonCopyable {
public:
    virtual void ConvertToGenericLight(CShaderGenericLight& light) const=0;
};
FABLE_STATIC_ASSERT(sizeof(CShaderLight)==4);
#endif
