#include <cstdio>

class CThingCreatureBase { public: long human; long IsHuman() { return human; } };
class CThingFilter_IsHuman { public: long operator()(CThingCreatureBase* c) { return c->IsHuman() != 0; } };
int main()
{
    CThingCreatureBase c; CThingFilter_IsHuman f;
    c.human = 7; if (f(&c) != 1) { std::printf("006a7b90_TEST FAIL\n"); return 1; }
    c.human = 0; if (f(&c) != 0) { std::printf("006a7b90_TEST FAIL\n"); return 1; }
    std::printf("006a7b90_TEST PASS\n");
    return 0;
}