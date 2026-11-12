// Behaviour test for 0x006D8F10 CHeroExperienceDef::Transfer: a copy of the model (member offsets as in retail, the three inline
// transfer bodies, the fourteen transfers in retail order) driven in both binary modes; checks the callee sequence, the member
// addresses handed over, the CheckCRC count and that the map / float transfers skip a non-binary mode.
#include <cstdio>
#include <cstring>
#include <cstddef>

enum EMode { MODE_LOAD_TEXT = 0, MODE_SAVE_TEXT = 1, MODE_LOAD_BINARY = 2, MODE_SAVE_BINARY = 3, MODE_CLEAR = 4 };

static char g_log[1024];
static const char* g_base = 0;
static int g_badName = 0;

static void Log(const char* tag, const void* member)
{
    char item[32];
    std::sprintf(item, "%s%x;", tag, (unsigned)((const char*)member - g_base));
    std::strcat(g_log, item);
}

struct Vector12 { void* start; void* finish; void* endOfStorage; };
struct Map12 { void* header; unsigned long count; void* pad; };

class CPersistContext {
public:
    EMode Mode;
    int crcCalls;
    void CheckCRC(const char* name) { ++crcCalls; if (name[0] != 0) ++g_badName; }
    void TransferVector(Vector12& value)
    {
        CheckCRC("");
        switch (Mode) {
        case MODE_LOAD_BINARY: Log("Vi", &value); break;
        case MODE_SAVE_BINARY: Log("Vo", &value); break;
        default: break;                         // unreachable in retail
        }
    }
    void TransferMap(Map12& map)
    {
        CheckCRC("");
        switch (Mode) {
        case MODE_LOAD_BINARY: Log("Mi", &map); break;
        case MODE_SAVE_BINARY: Log("Mo", &map); break;
        }
    }
    void Transfer(float& value)
    {
        CheckCRC("");
        switch (Mode) {
        case MODE_LOAD_BINARY: Log("Fi", &value); break;
        case MODE_SAVE_BINARY: Log("Fo", &value); break;
        }
    }
};

class CHeroExperienceDef {
public:
    unsigned char base[0x28];                               // vtable + CSubDefClassBase
    Vector12 ExperienceLevelThresholds;                     // +0x28
    Vector12 HeroLevelTitles;                               // +0x34
    Vector12 FallbackMultipliers;                           // +0x40
    Map12 SoundIntensityThresholds;                         // +0x4c
    Map12 ComboMultiplierMap;                               // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
    float ExperienceMultiplierRegionChangeMultiplier;       // +0x68
    Map12 ComboMultiplierIncrease;                          // +0x6c
    Map12 ComboMultiplierLock;                              // +0x78
    Map12 ComboMultiplierLost;                              // +0x84
    Map12 ComboMultiplierRunningHitsIncrease;               // +0x90
    Map12 StatIncreasePerHitType;                           // +0x9c
    Vector12 AttackHistoryCombos;                           // +0xa8
    void Transfer(CPersistContext& context);
};

void CHeroExperienceDef::Transfer(CPersistContext& context)
{
    context.TransferVector(ExperienceLevelThresholds);
    context.TransferVector(HeroLevelTitles);
    context.TransferVector(FallbackMultipliers);
    context.TransferMap(SoundIntensityThresholds);
    context.TransferMap(ComboMultiplierMap);
    context.Transfer(ExperienceMultiplierParamPerUnitComboMultiplier);
    context.Transfer(ExperienceMultiplierRegionChangeMultiplier);
    context.TransferMap(StatIncreasePerHitType);
    context.TransferMap(ComboMultiplierIncrease);
    context.TransferMap(ComboMultiplierLock);
    context.TransferMap(ComboMultiplierLost);
    context.TransferMap(ComboMultiplierRunningHitsIncrease);
    context.TransferMap(StatIncreasePerHitType);
    context.TransferVector(AttackHistoryCombos);
}

static int Fail(const char* why)
{
    std::printf("006d8f10_TEST FAIL (%s) log=%s\n", why, g_log);
    return 1;
}

int main()
{
    static CHeroExperienceDef def;
    CPersistContext context;
    g_base = (const char*)&def;
    if (sizeof(CHeroExperienceDef) != 0xb4 || offsetof(CHeroExperienceDef, StatIncreasePerHitType) != 0x9c
        || offsetof(CHeroExperienceDef, AttackHistoryCombos) != 0xa8)
        return Fail("layout");

    g_log[0] = 0; context.Mode = MODE_LOAD_BINARY; context.crcCalls = 0;
    def.Transfer(context);
    if (std::strcmp(g_log, "Vi28;Vi34;Vi40;Mi4c;Mi58;Fi64;Fi68;Mi9c;Mi6c;Mi78;Mi84;Mi90;Mi9c;Via8;") != 0) return Fail("load order");
    if (context.crcCalls != 14) return Fail("load crc count");

    g_log[0] = 0; context.Mode = MODE_SAVE_BINARY; context.crcCalls = 0;
    def.Transfer(context);
    if (std::strcmp(g_log, "Vo28;Vo34;Vo40;Mo4c;Mo58;Fo64;Fo68;Mo9c;Mo6c;Mo78;Mo84;Mo90;Mo9c;Voa8;") != 0) return Fail("save order");
    if (context.crcCalls != 14) return Fail("save crc count");

    g_log[0] = 0; context.Mode = MODE_CLEAR; context.crcCalls = 0;      // maps and floats: key check only, no stream call
    context.TransferMap(def.StatIncreasePerHitType);
    context.TransferMap(def.ComboMultiplierLock);
    context.Transfer(def.ExperienceMultiplierRegionChangeMultiplier);
    if (g_log[0] != 0) return Fail("non-binary mode not skipped");
    if (context.crcCalls != 3) return Fail("non-binary crc count");
    if (g_badName != 0) return Fail("key not empty");

    std::printf("006d8f10_TEST PASS\n");
    return 0;
}