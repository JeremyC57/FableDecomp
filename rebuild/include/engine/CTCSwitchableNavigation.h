#ifndef FABLE_ENGINE_CTCSWITCHABLENAVIGATION_H
#define FABLE_ENGINE_CTCSWITCHABLENAVIGATION_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "engine/CTCBase.h"
class CTCSwitchableNavigation:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return (ETCInterfaceType)121;}
 void SetAsBlockingNavigation(bool);
private:long MapIndex;
};
FABLE_STATIC_ASSERT(sizeof(CTCSwitchableNavigation)==16);
#endif
