#ifndef FABLE_ENGINE_CTCWILLRESPONSE_H
#define FABLE_ENGINE_CTCWILLRESPONSE_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CWillResponseDef;
class CTCWillResponse:public CTCBase {
public:
 static void FABLE_FASTCALL SetThingIsForcePushable(CThing*,bool);
 static void FABLE_FASTCALL SetThingAsNotBloodyWillableAtAll(CThing*,bool);
private:
 CDefPointer<const CWillResponseDef> PDef;
 bool IsForcePushable:1;bool IsLifeDrainable:1;bool IsLightningable:1;bool IsNotBloodyWillable:1;bool IsAbleToBeLifted:1;bool IsEpicPowerExplodable:1;bool IsEpicSpellable:1;
};
FABLE_STATIC_ASSERT(sizeof(CTCWillResponse)==20);
#endif
