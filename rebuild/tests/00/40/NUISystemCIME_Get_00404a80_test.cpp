#include <cstdio>

namespace NUISystem {
class CIME {
public:
    unsigned char _body[0x54];
    static CIME& __fastcall Get();
};
}
unsigned char g_primitiveVTableOrData[0x54] = {};

int main()
{
    NUISystem::CIME& r = NUISystem::CIME::Get();
    if ((void*)&r == (void*)&g_primitiveVTableOrData[0]) {
        std::printf("CEngineInternalPrimitiveBase_00404a80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
