// 006bb860 CGameTimeManager::ConsoleSetTimeOfDay
// Leaf: ?ConsoleSetTimeOfDay@CGameTimeManager@@CIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGENERICVAR_PASS27
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#include "engine/CGenericVar.h"
#include "engine/CGameTimeManager.h"
#include "engine/GFLimit.h"

void FABLE_FASTCALL CGameTimeManager::ConsoleSetTimeOfDay(
    CGameTimeManager& owner, const std::vector<CGenericVar>& params)
{
    owner.Time = owner.PrevTime = params[0].GetFloat() / 24.0f;
    if (!owner.UsingVillageTime) {
        GFLimit(owner.Time, 0.0f, 1.0f);
    } else if (owner.VillageDay) {
        GFLimit(owner.Time, 9.0f / 24.0f, 17.0f / 24.0f);
    } else if (owner.Time > 5.5f / 24.0f && owner.Time < 19.5f / 24.0f) {
        owner.Time = 19.5f / 24.0f;
    }
}