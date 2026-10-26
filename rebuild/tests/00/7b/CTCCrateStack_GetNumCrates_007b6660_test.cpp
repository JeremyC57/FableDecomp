#include <cstdio>
class CBaseObject { public: int id; };
struct CBaseIntelligentPointer { CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
class CTCCrateStack { public:
    CBaseIntelligentPointer FirstCrate; CBaseIntelligentPointer SecondCrate; long CrateDefIndex;
    long GetNumCrates() const
    {
        if (SecondCrate.GetPItem())
            return 2;
        return FirstCrate.GetPItem() != 0;
    }
};
int main()
{
    CBaseObject a = { 1 }, b = { 2 };
    CTCCrateStack s; s.FirstCrate.m_pItem = 0; s.SecondCrate.m_pItem = 0; s.CrateDefIndex = 0;
    if (s.GetNumCrates() != 0) { std::printf("007b6660_TEST FAIL\n"); return 1; }
    s.FirstCrate.m_pItem = &a;
    if (s.GetNumCrates() != 1) { std::printf("007b6660_TEST FAIL\n"); return 1; }
    s.SecondCrate.m_pItem = &b;
    if (s.GetNumCrates() != 2) { std::printf("007b6660_TEST FAIL\n"); return 1; }
    std::printf("007b6660_TEST PASS\n");
    return 0;
}