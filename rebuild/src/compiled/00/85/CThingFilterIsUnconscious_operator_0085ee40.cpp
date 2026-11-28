// Flags: 
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
