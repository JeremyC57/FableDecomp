// Behaviour test for 004d5476 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
enum EHeroExperienceStatCategory { CAT0 = 0, CAT1 = 1, CAT2 = 2 };
class CTCShotDefinitionThrow {
public:
    virtual ~CTCShotDefinitionThrow() { }
    virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory& c) { c = CAT1; return true; }
};
int main() {
    CTCShotDefinitionThrow s; EHeroExperienceStatCategory c = CAT2;
    CTCShotDefinitionThrow* p = &s;
    if (!p->GetStatToModifyOnHit(c) || c != CAT1) { std::printf("004d5476_TEST FAIL\n"); return 1; }
    { std::printf("004d5476_TEST PASS\n"); return 0; }
}