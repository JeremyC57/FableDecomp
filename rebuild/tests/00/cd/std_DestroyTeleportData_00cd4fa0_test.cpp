// Behaviour test for std::_Destroy<TeleportData> 0x00CD4FA0: destroys one element in place (the implicit ~TeleportData: strings in
// reverse order) without freeing its storage
#include <cstdio>
#include <new>
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
namespace std_model {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
}
int main()
{
    typedef NScript::CQ_BountyHuntScript::TeleportData TeleportData;
    static double storage[2][(sizeof(TeleportData) + sizeof(double) - 1) / sizeof(double)];
    TeleportData* first = new (storage[0]) TeleportData;
    TeleportData* second = new (storage[1]) TeleportData;
    first->defName.Id = 1; first->scriptName.Id = 2; first->teleportTo.Id = 3; first->teleportMkr.Id = 4;
    second->defName.Id = 5; second->scriptName.Id = 6; second->teleportTo.Id = 7; second->teleportMkr.Id = 8;
    std_model::_Destroy(second);
    if (g_count != 4 || g_order[0] != 8 || g_order[1] != 7 || g_order[2] != 6 || g_order[3] != 5) {
        std::printf("00cd4fa0_TEST FAIL\n");
        return 1;
    }
    std_model::_Destroy(first);
    if (g_count != 8 || g_order[4] != 4 || g_order[7] != 1) {
        std::printf("00cd4fa0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd4fa0_TEST PASS\n");
    return 0;
}