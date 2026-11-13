#ifndef FABLE_ENGINE_CTCWILLRESPONSE_H
#define FABLE_ENGINE_CTCWILLRESPONSE_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CWillResponseDef;
class CTCWillResponse:public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCWILLRESPONSE_THINGQUERIES
 // Retail opt-in (row 0x00795300): Ego_r.pdb / FableWin.pdb ?IsThingNotBloodyWillableAtAll@CTCWillResponse@@SI_NPBVCThing@@@Z
 // (0x007E45D0), ?IsThingEpicSpellable@..@@SI_NPBVCThing@@@Z (0x007E46D0), ?IsThingEpicPowerExplodable@..@@SI_NPBVCThing@@@Z (0x007E4680).
 static bool FABLE_FASTCALL IsThingNotBloodyWillableAtAll(const CThing*);   // 0x007E45D0
 static bool FABLE_FASTCALL IsThingEpicSpellable(const CThing*);            // 0x007E46D0
 static bool FABLE_FASTCALL IsThingEpicPowerExplodable(const CThing*);      // 0x007E4680
#endif
 static void FABLE_FASTCALL SetThingIsForcePushable(CThing*,bool);
 static void FABLE_FASTCALL SetThingAsNotBloodyWillableAtAll(CThing*,bool);
private:
 CDefPointer<const CWillResponseDef> PDef;
 bool IsForcePushable:1;bool IsLifeDrainable:1;bool IsLightningable:1;bool IsNotBloodyWillable:1;bool IsAbleToBeLifted:1;bool IsEpicPowerExplodable:1;bool IsEpicSpellable:1;
};
FABLE_STATIC_ASSERT(sizeof(CTCWillResponse)==20);
#endif
