#define FABLE_RETAIL_CWIDESTRING
// CWParsedItem::GetString() const @ 0x00A0CB80
// Leaf: ?GetString@CWParsedItem@@QBEABVCWideString@@XZ
#define FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
#include "engine/CWParsedItem.h"

const CWideString& CWParsedItem::GetString() const
{
    return String;
}
#include <assert.h>
#include <stdio.h>
#include <string.h>

CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
int main()
{
    CWParsedItem first, second;
    const CWParsedItem& view = first;
    const CWideString* result = &view.GetString();
    assert(reinterpret_cast<const char*>(result) == reinterpret_cast<const char*>(&first) + 8);
    assert(reinterpret_cast<const char*>(&second.GetString()) == reinterpret_cast<const char*>(&second) + 8);
    assert(result != &second.GetString());
    assert(result == &first.GetString());
    puts("00a0cb80_TEST PASS");
    return 0;
}
