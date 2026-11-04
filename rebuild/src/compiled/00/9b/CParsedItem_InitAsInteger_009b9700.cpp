// CParsedItem::InitAsInteger(const char* text, unsigned long length) -- parse `length` characters as a decimal integer (a '-' anywhere makes it negative) and set the type to integer
// Leaf: ?InitAsInteger@CParsedItem@@QAEXPBDK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9700, 103 bytes, `ret 8`; Ego_r.pdb ?InitAsInteger@CParsedItem@@QAEXPBDK@Z. No callees.
// Layout: engine/CParsedItem.h (Ego_r.pdb, 12 bytes): Type +0 (PARSED_ITEM_INTEGER = 3), the Integer/Float union +4, String +8.
// Shape: the landed CWParsedItem::InitAsInteger (0x00A0CA20) form -- an entry test around a do/while and the Type store written
// out on each exit (retail threads the zero-length path to its own Type store; one shared tail swaps the zero register).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#include "engine/CParsedItem.h"

void CParsedItem::InitAsInteger(const char* text, unsigned long length)
{
    unsigned long i = 0;
    Integer = 0;
    bool negative = false;
    char c = text[0];
    if (i < length) {
        do {
            if (c == '-')
                negative = true;
            else
                Integer = Integer * 10 + c - '0';
            c = text[++i];
        } while (i < length);
        if (negative == true) {
            Type = PARSED_ITEM_INTEGER;
            Integer = -Integer;
        } else
            Type = PARSED_ITEM_INTEGER;
    } else
        Type = PARSED_ITEM_INTEGER;
}