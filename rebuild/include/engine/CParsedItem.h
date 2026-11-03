#ifndef FABLE_ENGINE_CPARSEDITEM_H
#define FABLE_ENGINE_CPARSEDITEM_H
#include "fable_string.h"
enum EParsedItemType {PARSED_ITEM_NULL=0,PARSED_ITEM_SYMBOL=1,PARSED_ITEM_STRING=2,PARSED_ITEM_INTEGER=3,PARSED_ITEM_FLOAT=4};
// Layout: original Type+0, Integer/Float union+4, actual String+8, full size12.
class CParsedItem {
private:EParsedItemType Type;union{long Integer;float Float;};CCharString String;
#ifdef FABLE_RETAIL_CPARSEDITEM
// Retail class branch (Codex pass 13, row 0x009BB110): Ego_r TPI 0xC77F / FableWin 0xC4A2 mark the ctor, copy and dtor
// compiler-generated (attrs 0x103), so the retail class declares none of them.
public:const CCharString& GetString() const;
#else
public:CParsedItem(){} const CCharString& GetString() const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CParsedItem)==12);
#endif
