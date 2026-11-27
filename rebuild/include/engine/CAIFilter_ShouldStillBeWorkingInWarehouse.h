#ifndef FABLE_ENGINE_CAIFILTER_SHOULDSTILLBEWORKINGINWAREHOUSE_H
#define FABLE_ENGINE_CAIFILTER_SHOULDSTILLBEWORKINGINWAREHOUSE_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
class CThingAICreature;
class CVillageTaskCarryCrate;
class CTCVillage;
class CAIFilter_ShouldStillBeWorkingInWarehouse : public CAIStateGroupBase::CAIFilterBase {
public:
    CAIFilter_ShouldStillBeWorkingInWarehouse(CIntelligentPointer<CVillageTaskCarryCrate>, CIntelligentPointer<CThingAICreature>,const CTCVillage*);
    virtual bool operator()() const;
private:
    const CTCVillage* PTCVillage;
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;
    CIntelligentPointer<CThingAICreature> PCreature;
};
FABLE_STATIC_ASSERT(sizeof(CAIFilter_ShouldStillBeWorkingInWarehouse)==0x18);
#endif
