// behaviour test for CFrontEndDef::operator= (0x00432E60): root copy, two dwords, three flag bits, name, map, front-end fields
#include <cstdio>
static int g_root = 0, g_vec = 0;
struct CDefRootBase { unsigned int m_words[6]; CDefRootBase& operator=(const CDefRootBase& o) { ++g_root; for (int i = 0; i < 6; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CDefClassBase : CDefRootBase {
    unsigned int PDefManager; unsigned int GlobalIndex;
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1; unsigned char Other : 5;
    CDefClassBase& operator=(const CDefClassBase& o)
    { CDefRootBase::operator=(o); PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; Setup = o.Setup; Template = o.Template; DefaultValsApplied = o.DefaultValsApplied; return *this; }
};
struct CVec { unsigned int a, b, c; CVec& operator=(const CVec& o) { ++g_vec; a = o.a; b = o.b; c = o.c; return *this; } };
struct CKeyPairCompareLess {};
struct CVectorMap { CVec m_vector; CKeyPairCompareLess m_compare; bool m_sorted; };
struct CDefString { int TablePos; };
struct CParentDefClassBase : CDefClassBase { CDefString InstantiationName; CVectorMap SubDefInfoMap; };
struct CFrontEndDef : CParentDefClassBase {
    CVec vAttractModeMovie; int ErrorMessageBackgroundGraphic, ButtonABigGraphic, ButtonBBigGraphic;
    CFrontEndDef& operator=(const CFrontEndDef& other)
    {
        CParentDefClassBase::operator=(other);
        vAttractModeMovie = other.vAttractModeMovie;
        ErrorMessageBackgroundGraphic = other.ErrorMessageBackgroundGraphic;
        ButtonABigGraphic = other.ButtonABigGraphic;
        ButtonBBigGraphic = other.ButtonBBigGraphic;
        return *this;
    }
};
int main()
{
    CFrontEndDef a, b;
    for (int i = 0; i < 6; ++i) { a.m_words[i] = 10 + i; b.m_words[i] = 0; }
    a.PDefManager = 1; a.GlobalIndex = 2; a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1; a.Other = 3;
    b.PDefManager = 0; b.GlobalIndex = 0; b.Setup = 0; b.Template = 1; b.DefaultValsApplied = 0; b.Other = 9;
    a.InstantiationName.TablePos = 7; a.SubDefInfoMap.m_vector.a = 5; a.SubDefInfoMap.m_vector.b = 6; a.SubDefInfoMap.m_vector.c = 7; a.SubDefInfoMap.m_sorted = true;
    b.SubDefInfoMap.m_sorted = false;
    a.vAttractModeMovie.a = 1; a.vAttractModeMovie.b = 2; a.vAttractModeMovie.c = 3;
    a.ErrorMessageBackgroundGraphic = 11; a.ButtonABigGraphic = 12; a.ButtonBBigGraphic = 13;
    CFrontEndDef* r = &(b = a);
    int bad = r != &b || g_root != 1 || g_vec != 2;
    if (b.m_words[5] != 15 || b.PDefManager != 1 || b.GlobalIndex != 2 || b.Setup != 1 || b.Template != 0 || b.DefaultValsApplied != 1 || b.Other != 9) bad = 1;
    if (b.InstantiationName.TablePos != 7 || b.SubDefInfoMap.m_vector.c != 7 || !b.SubDefInfoMap.m_sorted) bad = 1;
    if (b.vAttractModeMovie.b != 2 || b.ErrorMessageBackgroundGraphic != 11 || b.ButtonABigGraphic != 12 || b.ButtonBBigGraphic != 13) bad = 1;
    if (bad) { std::printf("00432e60_TEST FAIL\n"); return 1; }
    std::printf("00432e60_TEST PASS\n");
    return 0;
}