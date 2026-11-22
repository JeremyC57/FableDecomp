#ifndef FABLE_RETAIL_PASS27_CCONTAINERREWARDHERODEF_H
#define FABLE_RETAIL_PASS27_CCONTAINERREWARDHERODEF_H
#ifdef FABLE_RETAIL_REWARD_PICKUP_PASS27
#include "engine/CSubDefClassBase.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
class CContainerRewardHeroDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName();
    CArray<CDefIndex> ObjectFamilies;
};
FABLE_STATIC_ASSERT(offsetof(CContainerRewardHeroDef,ObjectFamilies)==0x28);
#endif
#endif
