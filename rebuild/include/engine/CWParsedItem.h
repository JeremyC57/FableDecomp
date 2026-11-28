#ifndef FABLE_ENGINE_CWPARSEDITEM_H
#define FABLE_ENGINE_CWPARSEDITEM_H
#include "fable_string.h"
enum EParsedItemType;
// Original Ego_r4ebc7/FableWin CWParsedItem: private enum,anonymous union,real CWideString.
class CWParsedItem {
public:
 void InitAsInteger(const wchar_t*,unsigned long);
 void InitAsFloat(const wchar_t*,unsigned long);
#ifdef FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
 // Ego_r 00B776E0, FableWin 030F45C0. The retail method is 00A0CB80.
 const CWideString& GetString() const;
#endif
private:
 EParsedItemType Type;
 union {long Integer;float Float;};
 CWideString String;
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CWParsedItem)==12);
#endif
