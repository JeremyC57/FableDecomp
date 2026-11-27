#ifndef FABLE_NCREATUREMODE_EXPLOSIVEFORCEPUSHED_H
#define FABLE_NCREATUREMODE_EXPLOSIVEFORCEPUSHED_H
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeExplosiveForcePushed: public CTCCreatureModeManager::CModeBase {
public:
    CModeExplosiveForcePushed(CThing& parent,CThing& explosion,const CCharString& filter): CModeBase(parent),Filter(filter),PExplosion(&explosion) {}
    virtual EMode GetType()const {return MODE_EXPLOSIVE_FORCE_PUSHED;}
    virtual const CAnimFilterBase& GetAnimFilter()const {return Filter;}
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void FrameUpdate();
    virtual void AnimUpdate(CAnimateThing&);
private:
    CAnimFilterOfGroup Filter;
    CIntelligentPointer<CThing> PExplosion;
};
}
#endif
