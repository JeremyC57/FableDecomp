// Behaviour test for 0x007C53F0 CTCGhostSword::IsThingGhostSword: a thing matches only the positive def index of
// "GHOSTLY_SWORD".
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char text[32];
    CCharString(const char* t, long length = -1) { (void)length; std::strcpy(text, t); }
    ~CCharString() { text[0] = 0; }
};

class CGameDefinitionManager {
public:
    long swordIndex;
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetDefGlobalIndexFromName(const CCharString& name)
    {
        if (std::strcmp(name.text, "GHOSTLY_SWORD") == 0) return swordIndex;
        return 7;
    }
};

class CThing {
public:
    unsigned short DefGlobalIndex;
};

class CTCGhostSword {
public:
    static bool IsThingGhostSword(const CThing* thing)
    {
        long index;
        {
            CCharString name("GHOSTLY_SWORD");
            index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
        }
        if (index > 0 && thing->DefGlobalIndex == index)
            return true;
        return false;
    }
};

int main()
{
    CThing sword; sword.DefGlobalIndex = 42;
    CThing other; other.DefGlobalIndex = 7;
    CThing none; none.DefGlobalIndex = 0;
    CGameDefinitionManager::Get().swordIndex = 42;
    bool ok = CTCGhostSword::IsThingGhostSword(&sword) && !CTCGhostSword::IsThingGhostSword(&other);
    CGameDefinitionManager::Get().swordIndex = 0;          // def missing: index 0 never matches
    ok = ok && !CTCGhostSword::IsThingGhostSword(&none);
    CGameDefinitionManager::Get().swordIndex = -1;
    ok = ok && !CTCGhostSword::IsThingGhostSword(&none);
    if (!ok) { std::printf("007c53f0_TEST FAIL\n"); return 1; }
    std::printf("007c53f0_TEST PASS\n");
    return 0;
}