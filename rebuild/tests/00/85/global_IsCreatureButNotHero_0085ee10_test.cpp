#include <cstdio>
#include <cstring>
class CDefString { public:
    const char* m_text;
    bool operator!=(const char* text) const { return std::strcmp(m_text, text) != 0; }
};
class CThing { public:
    unsigned char m_type;
    const char* m_defName;
    CDefString GetDefName() const { CDefString d; d.m_text = m_defName; return d; }
};
bool IsCreatureButNotHero(CThing* thing)
{
    if (thing->m_type == 2 && thing->GetDefName() != "CREATURE_HERO")
        return true;
    return false;
}
int main()
{
    CThing hero; hero.m_type = 2; hero.m_defName = "CREATURE_HERO";
    CThing guard; guard.m_type = 2; guard.m_defName = "CREATURE_GUARD";
    CThing chest; chest.m_type = 5; chest.m_defName = "OBJECT_CHEST";
    if (IsCreatureButNotHero(&hero) || !IsCreatureButNotHero(&guard) || IsCreatureButNotHero(&chest)) { std::printf("0085ee10_TEST FAIL\n"); return 1; }
    std::printf("0085ee10_TEST PASS\n");
    return 0;
}