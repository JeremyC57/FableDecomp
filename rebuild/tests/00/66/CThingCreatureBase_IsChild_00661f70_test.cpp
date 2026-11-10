// 00661f70 CThingCreatureBase::IsChild -- behaviour test (agentNF6: member spelling)
#include <cstdio>
class CThingCreatureDef { public: char pad[0xC4]; int CreatureType; };
class CThingCreatureBase { public:
    char pad[0xE0];
    CThingCreatureDef* PDef;
    bool IsChild() const { return PDef->CreatureType == 1; }
};
int main() {
    CThingCreatureDef d0; d0.CreatureType = 1;
    CThingCreatureDef d1; d1.CreatureType = 2;
    CThingCreatureBase a; a.PDef = &d0;
    CThingCreatureBase b; b.PDef = &d1;
    bool ok = a.IsChild() && !b.IsChild();
    d0.CreatureType = 0; if (a.IsChild()) ok = false;
    if (ok) { std::printf("00661f70_TEST PASS\n"); return 0; }
    std::printf("00661f70_TEST FAIL\n"); return 1;
}
