#ifndef FABLE_ENGINE_CSHADERCONSTANTRANGE_H
#define FABLE_ENGINE_CSHADERCONSTANTRANGE_H
#include "engine/CBaseClass.h"
// Original Ego_r TPI0x40b7d: CBaseClass plus public Offset+4 and Count+8.
class CShaderConstantRange : public CBaseClass {
public:
    virtual ~CShaderConstantRange();
    long Offset;
    unsigned long Count;
};
FABLE_STATIC_ASSERT(sizeof(CShaderConstantRange)==12);
#endif
