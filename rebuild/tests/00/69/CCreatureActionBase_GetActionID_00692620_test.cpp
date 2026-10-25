#include <cstdio>
class CCreatureActionBase {
public:
    unsigned char _pad00[0x28];
    long ActionID;
    long GetActionID() const { return ActionID; }
};
int main()
{
    CCreatureActionBase a;
    a.ActionID = 0x12345678;
    if (sizeof(a._pad00) == 0x28 && a.GetActionID() == 0x12345678) { std::printf("00692620_TEST PASS\n"); return 0; }
    std::printf("00692620_TEST FAIL\n");
    return 1;
}
