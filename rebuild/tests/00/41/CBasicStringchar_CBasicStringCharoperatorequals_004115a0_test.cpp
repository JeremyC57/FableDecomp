// Test for 0x004115A0 CBasicString<char>::operator==(const char*) on the shared header (fable_string.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#include "fable_string.h"
#include <stdio.h>

struct TestString : CBasicString<char> {
    explicit TestString(char* text) { PData = text; StringLength = 0; DataLength = 0; UseFastExtend = false; }
};

int main()
{
    char buf1[] = "hello";
    TestString s1(buf1);
    bool r1 = (s1 == "hello");
    bool r2 = (s1 == "world");
    bool r3 = (s1 == "hell");
    bool r4 = (s1 == "hello!");
    char emptybuf[] = "";
    TestString s2(emptybuf);
    bool r5 = (s2 == "");
    bool r6 = (s2 == "x");
    if (r1 == true && r2 == false && r3 == false && r4 == false && r5 == true && r6 == false)
        printf("PASS_OP_EQ_004115A0\n");
    else
        printf("FAIL %d %d %d %d %d %d\n", r1, r2, r3, r4, r5, r6);
    return 0;
}
