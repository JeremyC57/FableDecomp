#ifndef FABLE_ENGINE_CPARSEDITEM_H
#define FABLE_ENGINE_CPARSEDITEM_H
#include "fable_string.h"
enum EParsedItemType {PARSED_ITEM_NULL=0,PARSED_ITEM_SYMBOL=1,PARSED_ITEM_STRING=2,PARSED_ITEM_INTEGER=3,PARSED_ITEM_FLOAT=4};
// Layout: original Type+0, Integer/Float union+4, actual String+8, full size12.
class CParsedItem {
private:EParsedItemType Type;union{long Integer;float Float;};CCharString String;
public:CParsedItem(){} const CCharString& GetString() const;
};
FABLE_STATIC_ASSERT(sizeof(CParsedItem)==12);
#endif
