// Behaviour test for CParsedItem::InitAsFloat (0x009B9770): a model of the row -- digits before '.' build the whole part, digits
// after it the fraction (divided by 10^n at the end), a '-' anywhere negates the result.
#include <stdio.h>
#include <math.h>
static float Parse(const char* text, unsigned long length)
{
    float whole = 0.0f, fraction = 0.0f, divisor = 1.0f;
    bool negative = false, inWhole = true;
    for (unsigned long i = 0; i < length; ++i) {
        char c = text[i];
        if (c == '-') negative = true;
        else if (c == '.') inWhole = false;
        else {
            long digit = c - '0';
            if (inWhole) whole = digit + whole * 10.0f;
            else { fraction = fraction * 10.0 + digit; divisor = divisor * 10.0; }
        }
    }
    float result = whole + fraction / divisor;
    if (negative) result = result * -1.0;
    return result;
}
int main()
{
    int ok = fabs(Parse("12.5", 4) - 12.5f) < 1e-5 && fabs(Parse("-3.25", 5) + 3.25f) < 1e-5 && Parse("", 0) == 0.0f
          && fabs(Parse("7.125xyz", 5) - 7.125f) < 1e-5;
    printf(ok ? "009b9770_TEST PASS\n" : "009b9770_TEST FAIL\n");
    return ok ? 0 : 1;
}
