#include <cstdio>
class CBaseObject { public: int id; };
struct CBaseIntelligentPointer { CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
class CTCCrateStack { public:
    CBaseIntelligentPointer FirstCrate; CBaseIntelligentPointer SecondCrate; long CrateDefIndex;
    CBaseObject* GetNextCrate() const
    {
        CBaseObject* crate = 0;
        if (SecondCrate.GetPItem())
            crate = SecondCrate.GetPItem();
        else if (FirstCrate.GetPItem())
            crate = FirstCrate.GetPItem();
        return crate;
    }
};
int main()
{
    CBaseObject a = { 1 }, b = { 2 };
    CTCCrateStack s; s.FirstCrate.m_pItem = 0; s.SecondCrate.m_pItem = 0; s.CrateDefIndex = 0;
    if (s.GetNextCrate() != 0) { std::printf("007b6750_TEST FAIL\n"); return 1; }
    s.FirstCrate.m_pItem = &a;
    if (s.GetNextCrate() != &a) { std::printf("007b6750_TEST FAIL\n"); return 1; }
    s.SecondCrate.m_pItem = &b;
    if (s.GetNextCrate() != &b) { std::printf("007b6750_TEST FAIL\n"); return 1; }
    std::printf("007b6750_TEST PASS\n");
    return 0;
}