// Behaviour test for CWParsedItem::InitAsFloat (0x00A0CA90), g++ model: '-' sign, digits, '.', fraction digits over a wide span; type 4.
#include <cstdio>
#include <cmath>
struct Item { int Type; float Float; };
static void InitAsFloat(Item& it, const unsigned short* text, unsigned long length)
{
    float divisor = 1.0f, integerPart = 0.0f, fraction = 0.0f; bool negative = false, beforePoint = true;
    it.Float = 0.0f;
    unsigned short c = *text;
    for (unsigned long i = 0; i < length; ) {
        if (c == '-') negative = true;
        else if (c == '.') beforePoint = false;
        else { long digit = c - '0'; if (beforePoint) integerPart = integerPart * 10.0f + digit; else { fraction = fraction * 10.0 + digit; divisor = divisor * 10.0; } }
        c = text[++i];
    }
    it.Type = 4;
    it.Float = integerPart + fraction / divisor;
    if (negative) it.Float = it.Float * -1.0;
}
int main()
{
    unsigned short a[] = { '1', '2', '.', '5', 0 }, b[] = { '-', '3', '.', '2', '5', 'x', 0 }, c[] = { '7', 0 };
    Item it;
    InitAsFloat(it, a, 4); int ok = it.Type == 4 && std::fabs(it.Float - 12.5f) < 1e-5;
    InitAsFloat(it, b, 5); ok = ok && std::fabs(it.Float + 3.25f) < 1e-5;
    InitAsFloat(it, c, 1); ok = ok && it.Float == 7.0f;
    InitAsFloat(it, a, 0); ok = ok && it.Float == 0.0f;
    std::printf(ok ? "00a0ca90_TEST PASS\n" : "00a0ca90_TEST FAIL\n");
    return ok ? 0 : 1;
}
