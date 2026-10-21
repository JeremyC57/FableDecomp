// behaviour test for 0x006A7120
#include <stdio.h>
#include <string.h>
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingTypeTag;                          // +0x10
};
class CThingCreatureBase : public CThing { };
class CThingFilter_IsInterestingToCreature {
public:
    CThingCreatureBase* SourceCreature;                  // +0x00
    bool operator()(const CThing& thing) const;
};
bool CThingFilter_IsInterestingToCreature::operator()(const CThing& thing) const
{
    return &thing != SourceCreature && (thing.ThingTypeTag == 1 || thing.ThingTypeTag == 2);
}

int main()
{
    CThingCreatureBase self, other; memset(&self, 0, sizeof(self)); memset(&other, 0, sizeof(other));
    CThingFilter_IsInterestingToCreature f; f.SourceCreature = &self;
    self.ThingTypeTag = 2; other.ThingTypeTag = 2;
    int ok = !f(self) && f(other);
    other.ThingTypeTag = 1; ok &= f(other);
    other.ThingTypeTag = 3; ok &= !f(other);
    printf(ok ? "006a7120_TEST PASS\n" : "006a7120_TEST FAIL\n");
    return ok ? 0 : 1;
}