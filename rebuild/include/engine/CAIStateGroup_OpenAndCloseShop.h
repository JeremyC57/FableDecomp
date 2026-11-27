#ifndef FABLE_ENGINE_CAISTATEGROUP_OPENANDCLOSESHOP_H
#define FABLE_ENGINE_CAISTATEGROUP_OPENANDCLOSESHOP_H
#ifdef FABLE_RETAIL_OPEN_CLOSE_SHOP_PASS29
#include "engine/CAIStateGroupBase.h"
class CAIStateGroup_OpenAndCloseShop : public CAIStateGroupBase {
    bool GoToInsideFace;
    bool Lock;
public:
    virtual void Update();
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_OpenAndCloseShop)==24);
#endif
#endif
