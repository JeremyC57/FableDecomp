// Behaviour test for the implicit NScript::CQ_BountyHuntScript::TeleportData::~TeleportData 0x00CD4550: the four CCharStrings die
// in reverse declaration order (teleportMkr, teleportTo, scriptName, defName); appearanceSeed/health need no destruction
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CCharString {
public:
    CCharString() : Id(0) {}
    ~CCharString() { g_order[g_count++] = Id; }
    int Id;
};
namespace NScript {
class CQ_BountyHuntScript {
public:
    struct TeleportData {
        CCharString   defName;
        unsigned long appearanceSeed;
        float         health;
        CCharString   scriptName;
        CCharString   teleportTo;
        CCharString   teleportMkr;
    };
};
}
int main()
{
    {
        NScript::CQ_BountyHuntScript::TeleportData data;
        data.defName.Id = 1; data.appearanceSeed = 7; data.health = 1.0f;
        data.scriptName.Id = 2; data.teleportTo.Id = 3; data.teleportMkr.Id = 4;
    }
    if (g_count != 4 || g_order[0] != 4 || g_order[1] != 3 || g_order[2] != 2 || g_order[3] != 1) {
        std::printf("00cd4550_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd4550_TEST PASS\n");
    return 0;
}