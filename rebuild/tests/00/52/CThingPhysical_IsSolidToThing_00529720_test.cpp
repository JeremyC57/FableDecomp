// Behaviour test for 00529720 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CThingPhysical {
public:
    virtual ~CThingPhysical() { }
    virtual bool IsSolidToThing(const CThingPhysical* thing) const { return thing != this; }
};
int main() {
    CThingPhysical a, b; const CThingPhysical* pa = &a;
    if (pa->IsSolidToThing(&a) || !pa->IsSolidToThing(&b) || !pa->IsSolidToThing(0)) { std::printf("00529720_TEST FAIL\n"); return 1; }
    { std::printf("00529720_TEST PASS\n"); return 0; }
}