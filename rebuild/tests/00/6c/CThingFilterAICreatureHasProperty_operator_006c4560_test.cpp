// Behaviour test for CThingFilter_AICreatureHasProperty::operator() (slot 1) (0x006C4560)
#include <cstdio>
#include <cstring>

class CThingAICreature { public: long m_type; virtual ~CThingAICreature() { } virtual long GetType() const { return m_type; } };
class CThingFilter_AICreatureHasProperty { public:
    long m_property;
    bool operator()(const CThingAICreature& creature) const { return creature.GetType() == m_property; }
};
#define CHECK(c) if (!(c)) { std::printf("006c4560_TEST FAIL\n"); return 1; }
int main()
{
    CThingAICreature c; c.m_type = 3;
    CThingFilter_AICreatureHasProperty f = { 3 };
    CHECK(f(c));
    c.m_type = 4;
    CHECK(!f(c));
    std::printf("006c4560_TEST PASS\n");
    return 0;
}