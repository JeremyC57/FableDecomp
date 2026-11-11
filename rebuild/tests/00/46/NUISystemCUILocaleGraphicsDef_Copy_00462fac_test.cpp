// Behaviour test for NUISystem::CUILocaleGraphicsDef::Copy (0x00462FAC): the virtual clone hook assigns the source def over this def.
#include <stdio.h>
#include <map>
#include <vector>
class CDefClassBase { public:
    int baseField;
    virtual ~CDefClassBase() { }
    virtual void Copy(const CDefClassBase* def) = 0;
};
namespace NUISystem {
class CUILocaleGraphicsDef : public CDefClassBase { public:
    std::map<int, unsigned long> WorldMapRegionName;
    std::vector<unsigned long> HelpScreenGraphics;
    unsigned long HelpRingPic;
    virtual void Copy(const CDefClassBase* def);
};
}
void NUISystem::CUILocaleGraphicsDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUILocaleGraphicsDef*>(def);
}
int main(void)
{
    NUISystem::CUILocaleGraphicsDef a, b;
    a.baseField = 5; a.WorldMapRegionName[3] = 30; a.WorldMapRegionName[9] = 90; a.HelpScreenGraphics.push_back(11); a.HelpRingPic = 77;
    b.baseField = 0; b.HelpScreenGraphics.push_back(1); b.HelpScreenGraphics.push_back(2); b.HelpRingPic = 0;
    CDefClassBase* p = &b;
    p->Copy(&a);
    if (b.baseField != 5 || b.WorldMapRegionName.size() != 2 || b.WorldMapRegionName[9] != 90 || b.HelpScreenGraphics.size() != 1 ||
        b.HelpScreenGraphics[0] != 11 || b.HelpRingPic != 77) { printf("00462fac_TEST FAIL\n"); return 1; }
    printf("00462fac_TEST PASS\n");
    return 0;
}