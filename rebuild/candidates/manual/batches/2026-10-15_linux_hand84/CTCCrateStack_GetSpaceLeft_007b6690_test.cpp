#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
struct CTCCrateStack {
    CBaseIntelligentPointer FirstCrate; CBaseIntelligentPointer SecondCrate; long CrateDefIndex;
    long GetSpaceLeft() const
    {
        if (SecondCrate.GetPItem())
            return 0;
        return FirstCrate.GetPItem() ? 1 : 2;
    }
};
int main()
{
    CBaseObject a = { 1 }, b = { 2 };
    CTCCrateStack s; s.FirstCrate.m_pItem = 0; s.SecondCrate.m_pItem = 0; s.CrateDefIndex = 0;
    if (s.GetSpaceLeft() != 2) { std::printf("007b6690_TEST FAIL\n"); return 1; }
    s.FirstCrate.m_pItem = &a;
    if (s.GetSpaceLeft() != 1) { std::printf("007b6690_TEST FAIL\n"); return 1; }
    s.SecondCrate.m_pItem = &b;
    if (s.GetSpaceLeft() != 0) { std::printf("007b6690_TEST FAIL\n"); return 1; }
    std::printf("007b6690_TEST PASS\n");
    return 0;
}
