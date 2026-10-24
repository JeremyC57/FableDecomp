// Behaviour test for GFStringToFloat(const wchar_t*) (0x009E19B0), modelled on 16-bit characters.
#include <cstdio>

typedef unsigned short WChar;

static unsigned int WideLength(const WChar* text)
{
    unsigned int n = 0;
    while (text[n] != 0) ++n;
    return n;
}

float GFStringToFloatW(const WChar* text)
{
    float integer = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    bool inInteger = true;
    bool negative = false;
    unsigned long length = WideLength(text);
    unsigned long i = 0;
    WChar c = *text;
    while (i < length)
    {
        if (c == '-')
            negative = true;
        else if (c == '.')
            inInteger = false;
        else if (inInteger == true)
            integer = integer * 10.0f + (c - '0');
        else
        {
            fraction = fraction * 10.0f + (c - '0');
            divisor *= 10.0;
        }
        c = *(++i + text);
    }
    fraction /= divisor;
    float result = fraction + integer;
    if (negative == true)
        result *= -1.0f;
    return result;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static float Parse(const char* s)
{
    WChar buffer[32];
    int i = 0;
    for (; s[i] != 0; ++i) buffer[i] = (WChar)(unsigned char)s[i];
    buffer[i] = 0;
    return GFStringToFloatW(buffer);
}

int main()
{
    if (!Near(Parse("12.5"), 12.5f)) { std::printf("009e19b0_TEST FAIL\n"); return 1; }
    if (!Near(Parse("-0.75"), -0.75f)) { std::printf("009e19b0_TEST FAIL\n"); return 1; }
    if (!Near(Parse("42"), 42.0f)) { std::printf("009e19b0_TEST FAIL\n"); return 1; }
    if (!Near(Parse(""), 0.0f)) { std::printf("009e19b0_TEST FAIL\n"); return 1; }
    std::printf("009e19b0_TEST PASS\n");
    return 0;
}