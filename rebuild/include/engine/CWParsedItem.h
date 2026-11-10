#ifndef FABLE_ENGINE_CWPARSEDITEM_H
#define FABLE_ENGINE_CWPARSEDITEM_H
#include "fable_string.h"
enum EParsedItemType;
// Original Ego_r4ebc7/FableWin CWParsedItem: private enum,anonymous union,real CWideString.
class CWParsedItem {
public:
 void InitAsInteger(const wchar_t*,unsigned long);
 void InitAsFloat(const wchar_t*,unsigned long);
private:
 EParsedItemType Type;
 union {long Integer;float Float;};
 CWideString String;
};
FABLE_STATIC_ASSERT(sizeof(CWParsedItem)==12);
#endif
