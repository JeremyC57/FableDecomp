// Behaviour test for CThingFilter_SolidToThing::operator() (0x00661640): links the reconstructed object.
#include <cstdio>
class CThing { public: int x; };
class CTCSolidToThing;
class CThingFilter_SolidToThing {
public:
    CThing*          SourceThing;
    CTCSolidToThing* PTCSolidToThing;
    bool operator()(const CThing& thing) const;
};
int main()
{
    CThing a, b;
    CThingFilter_SolidToThing f; f.SourceThing = &a; f.PTCSolidToThing = 0;
    const CThingFilter_SolidToThing& cf = f;
    if (cf(a) || !cf(b)) { std::printf("00661640_TEST FAIL\n"); return 1; }
    f.SourceThing = 0;
    if (!cf(a)) { std::printf("00661640_TEST FAIL\n"); return 1; }
    std::printf("00661640_TEST PASS\n");
    return 0;
}
