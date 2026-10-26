#include <cstdio>
#include <cmath>
// Behaviour test: CTCVillageMember::GetPVillage -- null when the village pointer is empty, else the village thing's CTCVillage component.
#define CHECK(c) if (!(c)) { std::printf("007ee540_TEST FAIL\n"); return 1; }
class CTCVillage { public: int id; };
class CThing { public: CTCVillage* Village; void GetTC(CTCVillage** out) { *out = Village; } };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
class CTCVillageMember { public:
    CIntelligentPointer PVillage;
    CTCVillage* GetPVillage() const
    {
        if (PVillage.GetPItem() != 0) {
            CTCVillage* village;
            PVillage.GetPItem()->GetTC(&village);
            return village;
        }
        return 0;
    }
};
int main()
{
    CTCVillage v; v.id = 7; CThing t; t.Village = &v;
    CTCVillageMember m; m.PVillage.Item = 0;
    CHECK(m.GetPVillage() == 0)
    m.PVillage.Item = &t;
    CHECK(m.GetPVillage() == &v)
    std::printf("007ee540_TEST PASS\n");
    return 0;
}