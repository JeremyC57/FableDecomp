#ifndef FABLE_ENGINE_CBLURREDATTITUDESDEF_H
#define FABLE_ENGINE_CBLURREDATTITUDESDEF_H
#ifdef FABLE_RETAIL_BLURRED_ATTITUDES_PASS29
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"
#include "engine/EOpinionAttitudeType.h"
class CBlurredAttitudesDef : public CParentDefClassBase {
public:
    CBlurredAttitudesDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<EOpinionAttitudeType> Attitudes;
};
FABLE_STATIC_ASSERT(sizeof(CBlurredAttitudesDef) == 0x48);
#endif
#endif
