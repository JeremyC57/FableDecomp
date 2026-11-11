// Behaviour test for 0x006A4B30 CThingDrawInformation::CThingDrawInformation(): DrawString is default-constructed once and
// BarOn / StringOn start false (RelativeSize, BarPercentage and Colour are left as they were).
#include <cstdio>
#include <new>
static int g_ctors;
class CCharString { public: const char* p; CCharString() : p("") { ++g_ctors; } };
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour() { } };
class CThingDrawInformation {
public:
    CCharString DrawString; float RelativeSize; float BarPercentage; CRGBColour Colour; bool BarOn; bool StringOn;
    CThingDrawInformation() : BarOn(false), StringOn(false) { }
};
int main()
{
    unsigned char buf[sizeof(CThingDrawInformation)];
    for (unsigned i = 0; i < sizeof buf; ++i) buf[i] = 0xCD;
    CThingDrawInformation* d = new (buf) CThingDrawInformation;
    if (d->BarOn || d->StringOn || g_ctors != 1 || d->Colour.R != 0xCD) { std::printf("006a4b30_TEST FAIL\n"); return 1; }
    std::printf("006a4b30_TEST PASS\n");
    return 0;
}
