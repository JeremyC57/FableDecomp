#ifndef FABLE_ENGINE_CSHOPITEM_H
#define FABLE_ENGINE_CSHOPITEM_H
#include "rebuild_abi.h"
// Original Ego_r TPI0x2d8e9, class V, and native0077603b copies seven words.
class CShopItem {
public:
    CShopItem();
    float Quantity,PriceMultiplier,MaxStock;
    long RestockPeriod;
    float RestockPeriodRandomness,SalesPerDay,SalesPerDayRandomness;
};
FABLE_STATIC_ASSERT(sizeof(CShopItem)==28);
#endif
