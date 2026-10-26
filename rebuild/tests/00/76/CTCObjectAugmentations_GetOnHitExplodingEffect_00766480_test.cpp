// Behaviour test for CTCObjectAugmentations::GetOnHitExplodingEffect @ 0x00766480: combined beats fire beats lightning, else 0.
#include <cstdio>
class CObjectAugmentationsDef { public: long Fire, Lightning, FireAndLightning; };
class CTCObjectAugmentations { public:
    const CObjectAugmentationsDef* PDef;
    long m_types;
    bool HasCombinationOfAugmentations(long types) const { return (m_types & types) == types; }
    bool HasAugmentationOfType(long type) const { return (m_types & type) != 0; }
    long GetOnHitExplodingEffect() const
    {
        long effect = 0;
        if (HasCombinationOfAugmentations(0xc)) effect = PDef->FireAndLightning;
        else if (HasAugmentationOfType(4)) effect = PDef->Fire;
        else if (HasAugmentationOfType(8)) effect = PDef->Lightning;
        return effect;
    }
};
int main()
{
    CObjectAugmentationsDef d = { 11, 22, 33 };
    CTCObjectAugmentations a; a.PDef = &d;
    a.m_types = 0;   if (a.GetOnHitExplodingEffect() != 0)  { std::printf("00766480_TEST FAIL\n"); return 1; }
    a.m_types = 4;   if (a.GetOnHitExplodingEffect() != 11) { std::printf("00766480_TEST FAIL\n"); return 1; }
    a.m_types = 8;   if (a.GetOnHitExplodingEffect() != 22) { std::printf("00766480_TEST FAIL\n"); return 1; }
    a.m_types = 0xc; if (a.GetOnHitExplodingEffect() != 33) { std::printf("00766480_TEST FAIL\n"); return 1; }
    std::printf("00766480_TEST PASS\n");
    return 0;
}