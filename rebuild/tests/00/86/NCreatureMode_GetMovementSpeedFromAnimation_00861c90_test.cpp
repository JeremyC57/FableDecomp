// Behaviour test for 0x00861C90 NCreatureMode::GetMovementSpeedFromAnimation(const CCharString&, ...) (chunk 402): the
// name resolves through GetAnimationFromCreature (null for the "DEFAULT" fallback, giving 0) and the entry's bank index
// goes to the `long` overload.
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __fastcall
#endif

class CCharString {
public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
};
class CAnimationEntry {
public:
    long BankIndex;
};
class CMeshDataBank {
public:
    float SpeedOfBank[8];
};
class CThingCreatureBase {
public:
    const char*     Names[3];
    CAnimationEntry Entries[3];
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long flags) const
    {
        (void)flags;
        for (int i = 0; i < 3; ++i)
            if (strcmp(Names[i], name.Text) == 0)
                return Entries[i];
        return Entries[0];
    }
};
static long gLastBankIndex = -1;
namespace NCreatureMode {
const CAnimationEntry* __fastcall GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature);
float __fastcall GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank);
float __fastcall GetMovementSpeedFromAnimation(const CAnimationEntry& anim, const CThingCreatureBase& creature, const CMeshDataBank& bank);
float __fastcall GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature, const CMeshDataBank& bank);
}

// stand-in for the `long` overload (0x00861950)
float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    (void)creature;
    gLastBankIndex = bankIndex;
    return bank.SpeedOfBank[bankIndex];
}

const CAnimationEntry* __fastcall NCreatureMode::GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature)
{
    const CAnimationEntry* fallback = &creature.GetAnimFromName("DEFAULT", 0);
    const CAnimationEntry* anim = &creature.GetAnimFromName(name, 0);
    if (fallback == anim)
        return 0;
    return anim;
}

inline float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(const CAnimationEntry& anim, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    return GetMovementSpeedFromAnimation(anim.BankIndex, creature, bank);
}

float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    const CAnimationEntry* anim = GetAnimationFromCreature(name, creature);
    if (!anim)
        return 0.0f;
    return GetMovementSpeedFromAnimation(*anim, creature, bank);
}

int main()
{
    int failures = 0;
    CThingCreatureBase creature;
    creature.Names[0] = "DEFAULT"; creature.Names[1] = "WALK"; creature.Names[2] = "RUN";
    creature.Entries[0].BankIndex = 1; creature.Entries[1].BankIndex = 4; creature.Entries[2].BankIndex = 6;
    CMeshDataBank bank;
    for (int i = 0; i < 8; ++i)
        bank.SpeedOfBank[i] = 0.5f * (float)i;
    if (NCreatureMode::GetMovementSpeedFromAnimation(CCharString("WALK"), creature, bank) != 2.0f || gLastBankIndex != 4) {
        printf("  WALK speed wrong\n"); ++failures;
    }
    if (NCreatureMode::GetMovementSpeedFromAnimation(CCharString("RUN"), creature, bank) != 3.0f || gLastBankIndex != 6) {
        printf("  RUN speed wrong\n"); ++failures;
    }
    gLastBankIndex = -1;
    if (NCreatureMode::GetMovementSpeedFromAnimation(CCharString("SWIM"), creature, bank) != 0.0f || gLastBankIndex != -1) {
        printf("  unknown name not 0\n"); ++failures;
    }
    if (NCreatureMode::GetMovementSpeedFromAnimation(CCharString("DEFAULT"), creature, bank) != 0.0f || gLastBankIndex != -1) {
        printf("  DEFAULT not 0\n"); ++failures;
    }
    if (failures == 0) {
        printf("00861c90_TEST PASS\n");
        return 0;
    }
    printf("00861c90_TEST FAIL\n");
    return 1;
}