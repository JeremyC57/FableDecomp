// CWParsedItem::GetString() const @ 0x00A0CB80
// Flags: /O2 /Oy
// Leaf: ?GetString@CWParsedItem@@QBEABVCWideString@@XZ
#define FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
#include "engine/CWParsedItem.h"

const CWideString& CWParsedItem::GetString() const
{
    return String;
}
