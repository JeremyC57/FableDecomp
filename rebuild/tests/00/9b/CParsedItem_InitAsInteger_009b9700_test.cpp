// Behaviour test for CParsedItem::InitAsInteger (0x009B9700): a model of the row -- the first `length` characters are folded as decimal
// digits, a '-' anywhere negates the result, and the type becomes integer (3) even for a zero length.
#include <stdio.h>
struct Item { long Type; long Integer;
    void InitAsInteger(const char* text, unsigned long length) {
        Integer = 0; bool negative = false;
        for (unsigned long i = 0; i < length; ++i) { if (text[i] == '-') negative = true; else Integer = Integer * 10 + text[i] - '0'; }
        Type = 3; if (negative == true) Integer = -Integer; } };
int main()
{
    Item it = { 0, 99 };
    it.InitAsInteger("1234xyz", 4); int ok = it.Type == 3 && it.Integer == 1234;
    it.InitAsInteger("-42", 3); ok = ok && it.Integer == -42;
    it.InitAsInteger("7", 0); ok = ok && it.Integer == 0 && it.Type == 3;
    printf(ok ? "009b9700_TEST PASS\n" : "009b9700_TEST FAIL\n");
    return ok ? 0 : 1;
}