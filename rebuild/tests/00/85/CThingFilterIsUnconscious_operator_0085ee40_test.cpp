#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#include <cstdio>
// 0085ee40 NCreatureMode::IsUnconscious
// Leaf: ?IsUnconscious@NCreatureMode@@YI_NABVCThing@@@Z
#define FABLE_RETAIL_NCREATUREMODE_UNCONSCIOUS_FLASH_PASS30
#include "rebuild_abi.h"
#include "engine/NCreatureMode.h"
namespace NCreatureMode {
bool FABLE_FASTCALL IsUnconscious(const CThing& thing)
{
    return false;
}
}
class CThing { public: bool dead; };
int main()
{
    CThing thing;
    for (int i=0; i<2; ++i) {
        thing.dead = i != 0;
        if (NCreatureMode::IsUnconscious(thing)) return 1;
    }
    std::puts("0085ee40_TEST PASS");
    return 0;
}
