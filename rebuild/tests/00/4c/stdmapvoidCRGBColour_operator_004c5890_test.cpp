// Behaviour test for std::map<void*, CRGBColour>::operator[] (0x004C5890): a model of the row -- an existing key returns its
// colour, a missing key is inserted with a default colour and the same slot is returned on the next lookup.
#include <stdio.h>
#include <map>
struct Colour { unsigned char B, G, R, A; };
int main()
{
    std::map<void*, Colour> tints;
    int a = 0, b = 0;
    Colour red = { 0, 0, 255, 255 };
    tints[&a] = red;
    int ok = tints[&a].R == 255 && tints.size() == 1;
    Colour& slot = tints[&b];
    ok = ok && tints.size() == 2;
    slot.G = 7;
    ok = ok && tints[&b].G == 7 && tints[&a].R == 255 && tints.size() == 2;
    printf(ok ? "004c5890_TEST PASS\n" : "004c5890_TEST FAIL\n");
    return ok ? 0 : 1;
}