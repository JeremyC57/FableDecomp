#ifndef FABLE_ENGINE_KID_WATCH_HOPSCOTCH_TYPED_H
#define FABLE_ENGINE_KID_WATCH_HOPSCOTCH_TYPED_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CAIStateGroup_KidWatchHopscotch : public CAIStateGroupBase {
public:
    static CAIStateGroupBase* FABLE_FASTCALL Alloc();
    static CCharString FABLE_FASTCALL GetName();
protected:
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    virtual bool IsRunnable();
    virtual void OnActivate();
    virtual void Update();
private:
    CIntelligentPointer<CThing> PHopscotchBoard;
    C3DVector HopscotchCentre;
    float HopscotchRadius;
    long CreationFrame;
    long StartFrame;
    long FramesPerMin;
    bool Fatigued;
    long ActivationCount;
    float ActivationFrequency;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidWatchHopscotch)==0x44);
#endif
