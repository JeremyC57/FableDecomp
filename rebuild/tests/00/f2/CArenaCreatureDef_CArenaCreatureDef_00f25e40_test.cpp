// Behaviour test for 0x00F25E40 CArenaCreatureDef::~CArenaCreatureDef(): the implicit destructor destroys HUDType, then
// CreatureType (reverse declaration order), then the base chain down to CResource.
#include <cstdio>

static int g_log[8];
static int g_count = 0;

class CResource {
public:
    virtual ~CResource() { g_log[g_count++] = 100; }
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }
};
class CDefClassBase : public CDefPointeeBase {
public:
    virtual ~CDefClassBase() { }
    void* PDefManager;
    unsigned int GlobalIndex;
};
class CCharString {
public:
    int Tag;
    explicit CCharString(int tag) : Tag(tag) { }
    ~CCharString() { g_log[g_count++] = Tag; }
};
class CArenaCreatureDef : public CDefClassBase {
public:
    CArenaCreatureDef() : CreatureType(1), NumCreatures(0), HUDType(2), DeathScore(0) { }
    CCharString CreatureType;
    long        NumCreatures;
    CCharString HUDType;
    long        DeathScore;
};

int main()
{
    {
        CArenaCreatureDef def;
        def.NumCreatures = 3;
        def.DeathScore = 50;
    }
    if (g_count != 3 || g_log[0] != 2 || g_log[1] != 1 || g_log[2] != 100) {
        std::printf("00f25e40_TEST FAIL\n");
        return 1;
    }
    CDefClassBase* base = new CArenaCreatureDef();
    g_count = 0;
    delete base;
    if (g_count != 3 || g_log[0] != 2 || g_log[2] != 100) {
        std::printf("00f25e40_TEST FAIL\n");
        return 1;
    }
    std::printf("00f25e40_TEST PASS\n");
    return 0;
}