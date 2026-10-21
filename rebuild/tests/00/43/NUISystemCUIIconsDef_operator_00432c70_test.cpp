// behaviour test for NUISystem::CUIIconsDef::operator= (0x00432C70): root copy, two dwords, three flag bits (the other
// five bits of the flag byte kept), name, map, then the 32 icon indices; returns *this.
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
struct CUIIconsDef : CParentDefClassBase {
    unsigned long Icons[32];                            // IconFriendRequestReceived .. IconListHighlight
    CUIIconsDef& operator=(const CUIIconsDef& other)
    {
        CParentDefClassBase::operator=(other);
        for (int i = 0; i < 32; ++i) Icons[i] = other.Icons[i];
        return *this;
    }
};
int main()
{
    CUIIconsDef a, b;
    for (int i = 0; i < 6; ++i) { a.m_words[i] = 10 + i; b.m_words[i] = 0; }
    for (int i = 0; i < 32; ++i) { a.Icons[i] = 100 + i; b.Icons[i] = 0; }
    a.PDefManager = 1; a.GlobalIndex = 2; a.Setup = 0; a.Template = 1; a.DefaultValsApplied = 1; a.Other = 3;
    b.PDefManager = 0; b.GlobalIndex = 0; b.Setup = 1; b.Template = 0; b.DefaultValsApplied = 0; b.Other = 21;
    a.InstantiationName.TablePos = 7; a.SubDefInfoMap.m_vector.a = 5; a.SubDefInfoMap.m_vector.b = 6; a.SubDefInfoMap.m_vector.c = 8; a.SubDefInfoMap.m_sorted = true;
    b.SubDefInfoMap.m_sorted = false;
    CUIIconsDef* r = &(b = a);
    int bad = r != &b || g_root != 1 || g_vec != 1;
    if (b.m_words[0] != 10 || b.m_words[5] != 15 || b.PDefManager != 1 || b.GlobalIndex != 2) bad = 1;
    if (b.Setup != 0 || b.Template != 1 || b.DefaultValsApplied != 1 || b.Other != 21) bad = 1;
    if (b.InstantiationName.TablePos != 7 || b.SubDefInfoMap.m_vector.c != 8 || !b.SubDefInfoMap.m_sorted) bad = 1;
    for (int i = 0; i < 32; ++i) if (b.Icons[i] != (unsigned long)(100 + i)) bad = 1;
    if (bad) { std::printf("00432c70_TEST FAIL\n"); return 1; }
    std::printf("00432c70_TEST PASS\n");
    return 0;
}