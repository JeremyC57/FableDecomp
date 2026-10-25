// Behaviour test for 0x00F27060 CHeroSoulsCreatureDef copy constructor.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    CCharString() : PData("") {}
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) {}
    CCharString& operator=(const CCharString& o) { PData = o.PData; return *this; }
};

static int g_baseCopies = 0;
class CDefClassBase {
public:
    unsigned long GlobalIndex;
    virtual ~CDefClassBase() {}
    CDefClassBase() : GlobalIndex(0) {}
    CDefClassBase(const CDefClassBase& o) : GlobalIndex(o.GlobalIndex) { ++g_baseCopies; }
    CDefClassBase& operator=(const CDefClassBase& o) { GlobalIndex = o.GlobalIndex; return *this; }
};

class CHeroSoulsCreatureDef : public CDefClassBase {
public:
    CCharString CreatureType;
    CCharString Location;
    long        NumCreatures;
    CHeroSoulsCreatureDef() : NumCreatures(0) {}
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other)
        : CDefClassBase(other), CreatureType(other.CreatureType), Location(other.Location), NumCreatures(other.NumCreatures) {}
};

int main()
{
    CHeroSoulsCreatureDef a;
    a.GlobalIndex = 77; a.CreatureType = CCharString("CREATURE_HOBBE"); a.Location = CCharString("HOBBE_CAVE"); a.NumCreatures = 5;
    CHeroSoulsCreatureDef b(a);
    if (g_baseCopies != 1 || b.GlobalIndex != 77 || std::strcmp(b.CreatureType.PData, "CREATURE_HOBBE") != 0 ||
        std::strcmp(b.Location.PData, "HOBBE_CAVE") != 0 || b.NumCreatures != 5) {
        std::printf("00f27060_TEST FAIL\n");
        return 1;
    }
    std::printf("00f27060_TEST PASS\n");
    return 0;
}