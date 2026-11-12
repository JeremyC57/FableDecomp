#include <cstdio>
#include <cstring>
// Behaviour test for CArenaCreatureDef::CArenaCreatureDef(const CArenaCreatureDef&) @ 0x00F25DD0 (agentFAM08: the landed test with
// the placeholder class and its m_first.. members respelled: test-only Model* types with the Ego_r.pdb member names)
static int g_stringCopies = 0, g_baseCopies = 0;
class ModelCharString {
public:
    const char* m_pText;
    ModelCharString(const char* text) : m_pText(text) {}
    ModelCharString(const ModelCharString& other) : m_pText(other.m_pText) { ++g_stringCopies; }
};
class ModelDefClassBase {
public:
    long m_globalIndex;
    ModelDefClassBase() : m_globalIndex(0) {}
    virtual ~ModelDefClassBase() {}
    ModelDefClassBase(const ModelDefClassBase& other) : m_globalIndex(other.m_globalIndex) { ++g_baseCopies; }
};
class ModelArenaCreatureDef : public ModelDefClassBase {
public:
    ModelCharString CreatureType; unsigned long NumCreatures; ModelCharString HUDType; unsigned long DeathScore;
    ModelArenaCreatureDef() : CreatureType("A"), NumCreatures(0), HUDType("B"), DeathScore(0) {}
    ModelArenaCreatureDef(const ModelArenaCreatureDef& other)
        : ModelDefClassBase(other), CreatureType(other.CreatureType), NumCreatures(other.NumCreatures), HUDType(other.HUDType), DeathScore(other.DeathScore) {}
};
int main()
{
    ModelArenaCreatureDef a; a.m_globalIndex = 17; a.NumCreatures = 5; a.DeathScore = 6;
    ModelArenaCreatureDef b(a);
    if (b.m_globalIndex != 17 || std::strcmp(b.CreatureType.m_pText, "A") != 0 || std::strcmp(b.HUDType.m_pText, "B") != 0 ||
        b.NumCreatures != 5 || b.DeathScore != 6 || g_stringCopies != 2 || g_baseCopies != 1) { std::printf("00f25dd0_TEST FAIL\n"); return 1; }
    std::printf("00f25dd0_TEST PASS\n");
    return 0;
}