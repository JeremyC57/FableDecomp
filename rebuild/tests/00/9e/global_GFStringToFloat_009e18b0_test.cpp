// Behaviour test for GFStringToFloat(const char*) (0x009E18B0).
#include <cstdio>
#include <cstring>

float GFStringToFloat(const char* text)
{
    float integer = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    bool inInteger = true;
    bool negative = false;
    unsigned long length = std::strlen(text);
    unsigned long i = 0;
    char c = *text;
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

int main()
{
    if (!Near(GFStringToFloat("12.5"), 12.5f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }
    if (!Near(GFStringToFloat("-3.25"), -3.25f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }
    if (!Near(GFStringToFloat("7"), 7.0f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }
    if (!Near(GFStringToFloat(""), 0.0f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }
    if (!Near(GFStringToFloat(".05"), 0.05f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }
    if (!Near(GFStringToFloat("1-0"), -10.0f)) { std::printf("009e18b0_TEST FAIL\n"); return 1; }   // '-' anywhere negates
    std::printf("009e18b0_TEST PASS\n");
    return 0;
}