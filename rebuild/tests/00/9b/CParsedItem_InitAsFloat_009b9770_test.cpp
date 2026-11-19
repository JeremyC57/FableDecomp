// Behaviour test for 0x009B9770 CParsedItem::InitAsFloat: the row's defines, includes and body verbatim, on a zeroed item image. Digits
// before '.' build the whole part, after it the fraction (divided by 10^n), a '-' anywhere negates, only `size` characters are read
// (the text need not be terminated there), size 0 gives 0, and the item becomes a float.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#include "engine/CParsedItem.h"

void CParsedItem::InitAsFloat(const char* string, unsigned long size)
{
    char ch;
    float top = 0.0f;
    float bottom = 0.0f;
    float bottom_divide = 1.0f;
    bool doing_top = true;
    bool is_negative = false;
    unsigned long c1;
    Float = 0.0f;
    if (false) {} else for (c1 = 0, ch = string[c1]; c1 < size; ++c1, ch = string[c1]) {
        if (ch == '-')
            is_negative = true;
        else if (ch == '.')
            doing_top = false;
        else if (doing_top == true) {
            top *= 10.0f;
            top += ch - '0';
        } else {
            bottom *= 10.0;
            bottom += ch - '0';
            bottom_divide *= 10.0;
        }
    }
    Float = bottom / bottom_divide + top;
    if (is_negative == true)
        Float *= -1.0;
    Type = PARSED_ITEM_FLOAT;
}

static double g_item[sizeof(CParsedItem) / 8 + 1];
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static float Parse(const char* s, unsigned long n)
{
    memset(g_item, 0, sizeof(g_item));
    CParsedItem* item = reinterpret_cast<CParsedItem*>(g_item);
    item->InitAsFloat(s, n);
    if (!item->IsFloat()) { printf("FAIL type\n"); g_fail = 1; }
    return item->GetFloat();
}
static int Near(float a, float b) { return fabs(a - b) <= 1e-4f * (fabs(b) + 1.0f); }
int main()
{
    check(Near(Parse("12.25", 5), 12.25f), "whole and fraction");
    check(Near(Parse("-3.5", 4), -3.5f), "leading minus");
    check(Near(Parse("7-", 2), -7.0f), "a minus anywhere negates");
    check(Near(Parse("1234", 4), 1234.0f), "whole only");
    check(Near(Parse(".0625", 5), 0.0625f), "fraction only");
    check(Near(Parse("98.76xyz", 3), 98.0f), "only size characters read (98.)");
    check(Near(Parse("98.76", 4), 98.7f), "size cuts the fraction");
    check(Parse("5", 0) == 0.0f, "size 0 -> 0");
    check(Near(Parse("0.5", 3), 0.5f), "leading zero");
    if (!g_fail) printf("009b9770_TEST PASS\n");
    return g_fail;
}