#include <cstdio>

class CSystemManager { public: int Marker; };
CSystemManager GSystemManager;
CSystemManager* __fastcall GFGetSystemManager();

int main()
{
    if (GFGetSystemManager() == &GSystemManager && GFGetSystemManager() == GFGetSystemManager())
    {
        std::printf("009a4ec0_TEST PASS\n");
        return 0;
    }
    std::printf("009a4ec0_TEST FAIL\n");
    return 1;
}
