#ifndef FABLE_ENGINE_CAISTATEGROUP_BEETLETOGGLEHOVER_H
#define FABLE_ENGINE_CAISTATEGROUP_BEETLETOGGLEHOVER_H
#include "engine/CAIStateGroupBase.h"
// Original complete Ego_r0x53d0a, native IsLanding+14,size24.
class CAIStateGroup_BeetleToggleHover:public CAIStateGroupBase {
protected:
 virtual bool IsRunnable();
 virtual void Update();
public:
 static CAIStateGroupBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL GetName();
private:
 bool IsLanding;
};
#endif
