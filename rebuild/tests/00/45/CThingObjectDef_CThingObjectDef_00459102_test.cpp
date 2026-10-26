// behaviour test for 0x00459102 CThingObjectDef::CThingObjectDef(): after the CThingBaseDef base is built the def's Graphic reads bank 0, animation step 0, render size 0, no additive alpha and type 0, and the object's dynamic type is the derived def
#include <cstdio>
#include <cstring>
#include <new>
class CBase { public: CBase() : Ready(1) { } virtual ~CBase() { } virtual int Kind() const { return 0; } int Ready; };
class CEngineGraphic { public: CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { } unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
class CDerived : public CBase { public: CDerived() { } virtual int Kind() const { return 1; } CEngineGraphic Graphic; int Other; };
int main()
{
    int fail = 0;
    static double storage[16];
    std::memset(storage, 0x5A, sizeof(storage));
    CDerived* d = new (storage) CDerived();
    if (d->Ready != 1) fail |= 1;
    if (d->Graphic.BankIndex != 0 || d->Graphic.AnimStep != 0.0f || d->Graphic.RenderSizeX != 0.0f || d->Graphic.AdditiveAlpha || d->Graphic.Type != 0) fail |= 2;
    CBase* b = d;
    if (b->Kind() != 1) fail |= 4;
    if (fail) { std::printf("00459102_TEST FAIL %d\n", fail); return 1; }
    std::printf("00459102_TEST PASS\n");
    return 0;
}