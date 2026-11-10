// 00662000 CThingCreatureBase::GetCreatureType -- behaviour test (agentNF6: member spelling)
#include <cstdio>
enum ECreatureType { CREATURE_TYPE_TEST = 1234 };
class CThingCreatureDef { public: char pad[0xC4]; ECreatureType CreatureType; };
class CThingCreatureBase { public:
    char pad[0xE0];
    CThingCreatureDef* PDef;
    ECreatureType GetCreatureType() const { return PDef->CreatureType; }
};
int main() {
    CThingCreatureDef d; d.CreatureType = CREATURE_TYPE_TEST;
    CThingCreatureBase c; c.PDef = &d;
    if (c.GetCreatureType() != CREATURE_TYPE_TEST) { std::printf("00662000_TEST FAIL\n"); return 1; }
    std::printf("00662000_TEST PASS\n"); return 0;
}
