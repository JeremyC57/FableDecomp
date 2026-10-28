// Behaviour test for 0x00F27190 CHeroSoulsCreatureDef::operator=.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    CCharString() : PData("") {}
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) {}
    const CCharString& operator=(const CCharString& o) { PData = o.PData; return *this; }
};

static int g_baseAssigns = 0;
class CDefClassBase {
public:
    unsigned long GlobalIndex;
    virtual ~CDefClassBase() {}
    CDefClassBase() : GlobalIndex(0) {}
    CDefClassBase(const CDefClassBase& o) : GlobalIndex(o.GlobalIndex) {}
    CDefClassBase& operator=(const CDefClassBase& o) { GlobalIndex = o.GlobalIndex; ++g_baseAssigns; return *this; }
};

class CHeroSoulsCreatureDef : public CDefClassBase {
public:
    CCharString CreatureType;
    CCharString Location;
    long        NumCreatures;
    CHeroSoulsCreatureDef() : NumCreatures(0) {}
    CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef& other)
    {
        CDefClassBase::operator=(other);
        CreatureType = other.CreatureType;
        Location = other.Location;
        NumCreatures = other.NumCreatures;
        return *this;
    }
};

int main()
{
    CHeroSoulsCreatureDef a, b;
    a.GlobalIndex = 12; a.CreatureType = CCharString("CREATURE_BANDIT"); a.Location = CCharString("BANDIT_CAMP"); a.NumCreatures = 9;
    CHeroSoulsCreatureDef* r = &(b = a);
    if (r != &b || g_baseAssigns != 1 || b.GlobalIndex != 12 || std::strcmp(b.CreatureType.PData, "CREATURE_BANDIT") != 0 ||
        std::strcmp(b.Location.PData, "BANDIT_CAMP") != 0 || b.NumCreatures != 9) {
        std::printf("00f27190_TEST FAIL\n");
        return 1;
    }
    std::printf("00f27190_TEST PASS\n");
    return 0;
}