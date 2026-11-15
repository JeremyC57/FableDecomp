#ifndef FABLE_ENGINE_CTCCARRIEDACTIONUSEFISHINGROD_H
#define FABLE_ENGINE_CTCCARRIEDACTIONUSEFISHINGROD_H
#ifdef FABLE_RETAIL_CTCCARRIEDACTIONUSEFISHINGROD_PASS22
#include "engine/CTCActionUseBase.h"
class CThingCreatureBase;
class CTCCarriedActionUseBase:public CTCActionUseBase {public:virtual void OnActionUseInHand(CThing&);};
class CTCCarriedActionUseFishingRod:public CTCCarriedActionUseBase {
public:static bool FABLE_FASTCALL IsThingAbleToFish(const CThingCreatureBase&);
};
FABLE_STATIC_ASSERT(sizeof(CTCCarriedActionUseFishingRod)==20);
#endif
#endif
// Retail opt-in (Codex pass22): both original bases,neither adds data; exact original static member and const reference.
