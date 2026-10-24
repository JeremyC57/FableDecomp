// Behaviour test for 0x008618B0 NCreatureMode::GetAnimationFromCreature (chunk 402): the creature's entry for the name,
// or null when the lookup falls back to the "DEFAULT" entry (unknown names and "DEFAULT" itself); the converted
// "DEFAULT" string temporary is destroyed.
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int gStrings = 0;
class CCharString {
public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; ++gStrings; }
    CCharString(const CCharString& other) : Text(other.Text) { ++gStrings; }
    ~CCharString() { --gStrings; }
};
class CAnimationEntry {
public:
    long BankIndex;
};
class CThingCreatureBase {
public:
    const char*     Names[3];
    CAnimationEntry Entries[3];                         // Entries[0] is the "DEFAULT" fallback
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long flags) const
    {
        (void)flags;
        for (int i = 0; i < 3; ++i)
            if (strcmp(Names[i], name.Text) == 0)
                return Entries[i];
        return Entries[0];
    }
};
namespace NCreatureMode {
const CAnimationEntry* __fastcall GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature);
}

const CAnimationEntry* __fastcall NCreatureMode::GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature)
{
    const CAnimationEntry* fallback = &creature.GetAnimFromName("DEFAULT", 0);
    const CAnimationEntry* anim = &creature.GetAnimFromName(name, 0);
    if (fallback == anim)
        return 0;
    return anim;
}

int main()
{
    int failures = 0;
    CThingCreatureBase creature;
    creature.Names[0] = "DEFAULT"; creature.Names[1] = "WALK"; creature.Names[2] = "RUN";
    for (int i = 0; i < 3; ++i)
        creature.Entries[i].BankIndex = 100 + i;
    {
        CCharString run("RUN");
        if (NCreatureMode::GetAnimationFromCreature(run, creature) != &creature.Entries[2]) { printf("  RUN not found\n"); ++failures; }
        CCharString fly("FLY");
        if (NCreatureMode::GetAnimationFromCreature(fly, creature) != 0) { printf("  unknown name not null\n"); ++failures; }
        CCharString def("DEFAULT");
        if (NCreatureMode::GetAnimationFromCreature(def, creature) != 0) { printf("  DEFAULT not null\n"); ++failures; }
    }
    if (gStrings != 0) { printf("  %d strings leaked\n", gStrings); ++failures; }
    if (failures == 0) {
        printf("008618b0_TEST PASS\n");
        return 0;
    }
    printf("008618b0_TEST FAIL\n");
    return 1;
}