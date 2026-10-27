// Test for 0x009F76A0 CFourierAnalysis::CFourierAnalysis: the empty constructor leaves the object's storage untouched.
#include <stdio.h>
#include <string.h>
#include <new>
class CFourierAnalysis { public: CFourierAnalysis() { } };
int main()
{
    static unsigned char buf[4]; memset(buf, 0x5A, sizeof(buf));
    new (buf) CFourierAnalysis;
    bool ok = buf[0] == 0x5A;
    puts(ok ? "009f76a0_TEST PASS" : "009f76a0_TEST FAIL");
    return ok ? 0 : 1;
}