#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeCarryingPlateStand::GetHoldingAnimIndex (retail 0x008D6540): the bank index of
// the creature's first "ST_HOLDING_PLATE" animation.
class CAnimationEntry { public: long BankIndex; };
class CThingCreatureBase { public:
    CAnimationEntry Plate;
    CAnimationEntry Other;
    long lastIndex;
    CAnimationEntry* GetAnimFromName(const char* name, long index)
    {
        lastIndex = index;
        return std::strcmp(name, "ST_HOLDING_PLATE") == 0 ? &Plate : &Other;
    }
};
struct CModeCarryingPlateStand {
    CThingCreatureBase* ParentThing;
    long GetHoldingAnimIndex() const
    {
        CThingCreatureBase* creature = ParentThing;
        return creature->GetAnimFromName("ST_HOLDING_PLATE", 0)->BankIndex;
    }
};

int main()
{
    CThingCreatureBase c;
    c.Plate.BankIndex = 1234; c.Other.BankIndex = 9; c.lastIndex = -1;
    CModeCarryingPlateStand m;
    m.ParentThing = &c;
    if (m.GetHoldingAnimIndex() != 1234 || c.lastIndex != 0) { std::printf("008d6540_TEST FAIL\n"); return 1; }
    std::printf("008d6540_TEST PASS\n");
    return 0;
}