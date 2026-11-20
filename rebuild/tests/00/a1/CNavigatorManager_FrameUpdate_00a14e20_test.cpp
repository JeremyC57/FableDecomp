// Behaviour test for CNavigatorManager::FrameUpdate 0x00A14E20 (NF150: on engine/CNavigatorManager.h, links the row object).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CNAVIGATORMANAGER
#include "engine/CNavigatorManager.h"

int main()
{
    CNavigatorManager manager;
    std::memset(&manager, 0x5A, sizeof(manager));
    manager.NumberOfNodeExaminationsLeftThisFrame = 3;
    manager.FrameUpdate();
    unsigned char untouched[4];
    std::memset(untouched, 0x5A, sizeof(untouched));
    if (manager.NumberOfNodeExaminationsLeftThisFrame != 800 || std::memcmp(&manager.PInterface, untouched, 4) != 0) {
        std::printf("00a14e20_TEST FAIL\n");
        return 1;
    }
    manager.FrameUpdate();
    if (manager.NumberOfNodeExaminationsLeftThisFrame != 800) {
        std::printf("00a14e20_TEST FAIL\n");
        return 1;
    }
    std::printf("00a14e20_TEST PASS\n");
    return 0;
}
